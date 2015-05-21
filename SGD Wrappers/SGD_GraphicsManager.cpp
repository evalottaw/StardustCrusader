/***********************************************************************\
|																		|
|	File:			SGD_GraphicsManager.cpp								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-12-31											|
|																		|
|	Purpose:		To render simple geometry and						|
|					load and render image files							|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_GraphicsManager.h"


// Uses Win32 API for window
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Uses cin, cout, clog, cerr for console
#include <iostream>

// Uses _snprintf_s & _wcsdup
#include <cstring>
#include <cstdio>

// Uses Direct3D9 for rendering
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

// Uses HandleManager for storing data
#include "SGD_HandleManager.h"

// Uses SGD_ASSERT for debug breaks
#include "SGD_Utilities.h"

// Window Class Descriptor ID
#define SGD_WINDOW_CLASS_NAME		L"SGD Graphics Manager Window"


namespace SGD
{
	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// TextureInfo
		//	- stores info for the texture file: name, buffer, reference count
		struct TextureInfo
		{
			wchar_t*				wszFilename;		// file name
			unsigned int			unRefCount;			// reference count
			IDirect3DTexture9*		texture;			// texture
			float					fWidth;				// width
			float					fHeight;			// height
		};
		//*************************************************************//



		//*************************************************************//
		// GraphicsManager
		//	- concrete class for rendering simple geometry and image files
		//	- supports .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga files
		//	- texture dimensions will be rounded up to the nearest power of 2 (e.g. 2,4,8,16,32,64, etc.)
		//	- uses IHandleManager to store texture data
		class GraphicsManager : public SGD::GraphicsManager
		{
		public:
			// SINGLETON
			static	GraphicsManager*	GetInstance		( void );
			static	void				DeleteInstance	( void );


			virtual	bool		Initialize				( bool bVsync )		override;
			virtual	bool		Initialize				( const wchar_t* title, Size size, bool vsync )		override;
			virtual	bool		Update					( void )			override;
			virtual	bool		Terminate				( void )			override;


			virtual bool		SetClearColor			( Color color )					override;
			virtual bool		SetPixelatedMode		( bool pixelated )				override;
			virtual bool		ShowCursor				( bool show )					override;
			virtual bool		ShowConsoleWindow		( bool show )					override;
			virtual bool		Resize					( Size size, bool windowed )	override;
			virtual bool		IsForegroundWindow		( void )						override;


			virtual bool		DrawString				( const wchar_t* text, Point position,  Color color )						override;
			virtual bool		DrawString				( const char* text, Point position,  Color color )							override;
			virtual bool		DrawLine				( Point position1, Point position2, Color color, unsigned int width )		override;
			virtual bool		DrawRectangle			( Rectangle rect, Color fillColor, Color lineColor, unsigned int width )	override;


			virtual	HTexture	LoadTexture				( const wchar_t* filename, Color colorKey )		override;
			virtual	HTexture	LoadTexture				( const char* filename, Color colorKey )		override;
			virtual	bool		DrawTexture				( HTexture handle, Point position, float rotation, Vector rotationOffset, Color color, Size scale )						override;
			virtual	bool		DrawTextureSection		( HTexture handle, Point position, Rectangle section, float rotation, Vector rotationOffset, Color color, Size scale )	override;
			virtual	bool		UnloadTexture			( HTexture& handle )							override;

		private:
			// SINGLETON
			static	GraphicsManager*		s_Instance;		// the ONE instance

			GraphicsManager					( void )					= default;		// Default constructor
			virtual	~GraphicsManager		( void )					= default;		// Destructor

			GraphicsManager					( const GraphicsManager& )	= delete;		// Copy constructor
			GraphicsManager&	operator=	( const GraphicsManager& )	= delete;		// Assignment operator


			// Wrapper Status
			enum EGraphicsManagerStatus
			{	
				E_UNINITIALIZED,
				E_INITIALIZED,
				E_DESTROYED
			};

			EGraphicsManagerStatus		m_eStatus			= E_UNINITIALIZED;			// wrapper initialization status

			HandleManager< TextureInfo > m_HandleManager;								// data storage

			IDirect3D9*					m_pDirect3D			= nullptr;					// Direct3D api
			IDirect3DDevice9*			m_pDevice			= nullptr;					// device
			ID3DXSprite*				m_pSprite			= nullptr;					// sprite
			ID3DXFont*					m_pFont				= nullptr;					// font
			IDirect3DTexture9*			m_pTexture			= nullptr;					// rectangle texture
			D3DPRESENT_PARAMETERS		m_PresentParams		= D3DPRESENT_PARAMETERS{};	// device presentation parameters
			
			HWND						m_hWnd				= NULL;						// window handle
			bool						m_bWindowOwned		= false;					// is the window owned/updated internally
			LONG						m_lWindowStyle		= 0;						// starting window style flags
			Color						m_ClearColor		= Color{0, 0, 0};			// background clear color
			bool						m_bPixelated		= true;						// texel sample state
			Size						m_WindowSize		= Size{};					// window size
			Size						m_DesiredSize		= Size{};					// desired resolution (for fullscreen adjustment)
			bool						m_bCursorClipped	= false;					// is the cursor clipped to a region

			wchar_t*					m_pwszBuffer		= nullptr;					// output buffer storage (preallocated to hasten ASCII -> UTF16 conversion)
			int							m_nBufferSize		= 0;						// size (in wchar_t) of output buffer


			// CLEAR SCREEN HELPER METHOD
			bool			ClearScreen( void );


			// TEXTURE REFERENCE HELPER METHOD
			struct SearchInfo
			{
				const wchar_t*	filename;	// input
				TextureInfo*	texture;	// output
				HTexture		handle;		// output
			};
			static	bool	FindTextureByName( Handle handle, TextureInfo& data, SearchInfo* extra );


			// WINDOW INITIALIZATION HELPER METHODS
			HWND InitializeWindow( const wchar_t* title, LONG width, LONG height );

			// WINDOW MESSAGE PROC HELPER METHODS
			static LRESULT CALLBACK WindowMessageProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
			bool HandleMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		};
		//*************************************************************//

	}	// namespace SGD_IMPLEMENTATION



	//*****************************************************************//
	// Interface singleton accessor
	/*static*/ GraphicsManager* GraphicsManager::GetInstance( void )
	{
		return (SGD::GraphicsManager*)SGD_IMPLEMENTATION::GraphicsManager::GetInstance();
	}

	// Interface singleton destructor
	/*static*/ void GraphicsManager::DeleteInstance( void )
	{
		return SGD_IMPLEMENTATION::GraphicsManager::DeleteInstance();
	}
	//*****************************************************************//



	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//	
		// SINGLETON

		// Instantiate static pointer to null (no instance yet)
		/*static*/ GraphicsManager* GraphicsManager::s_Instance = nullptr;

		// Singleton accessor
		/*static*/ GraphicsManager* GraphicsManager::GetInstance( void )
		{
			// Allocate singleton on first use
			if( GraphicsManager::s_Instance == nullptr )
				GraphicsManager::s_Instance = new GraphicsManager;

			// Return the singleton
			return GraphicsManager::s_Instance;
		}

		// Singleton destructor
		/*static*/ void GraphicsManager::DeleteInstance( void )
		{
			// Deallocate singleton
			delete GraphicsManager::s_Instance;
			GraphicsManager::s_Instance = nullptr;
		}
		//*************************************************************//



		//*************************************************************//
		// INITIALIZE
		bool GraphicsManager::Initialize( bool bVsync )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_UNINITIALIZED, "GraphicsManager::Initialize - wrapper has already been initialized" );
			if( m_eStatus != E_UNINITIALIZED )
				return false;


			// Get the thread's window handle
			HWND hWnd = GetActiveWindow();

			// Do we need to create a new window?
			if( hWnd == nullptr )
			{
				// Get the console title
				wchar_t title[ 256 ] = L"SGD Application";
				GetConsoleTitleW( title, 256 );

				// Create a window
				hWnd = InitializeWindow( title, 1024, 768 );
				if( hWnd == nullptr )
				{
					// MESSAGE
					char szBuffer[ 64 ];
					_snprintf_s( szBuffer, 64, _TRUNCATE, "!!! GraphicsManager::Initialize - invalid window handle !!!\n" );
					Alert( szBuffer );
					//OutputDebugStringA( szBuffer );

					return false;
				}
			}


			// When using a COM object, the thread should be CoInitialized
			CoInitializeEx( nullptr, COINIT_MULTITHREADED );


			// Attempt to create the Direct3D interface
			HRESULT hResult = S_OK;
			m_pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
			if( m_pDirect3D == nullptr )
			{
				CoUninitialize();

				// MESSAGE
				char szBuffer[ 64 ];
				_snprintf_s( szBuffer, 64, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D !!!\n" );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}

			
			// Get the window dimensions
			RECT rWindow;
			GetClientRect( hWnd, &rWindow );
			m_WindowSize = { (float)rWindow.right, (float)rWindow.bottom };
			m_DesiredSize = m_WindowSize;


			// Setup device presentation parameters
			m_PresentParams.BackBufferWidth				= rWindow.right;
			m_PresentParams.BackBufferHeight			= rWindow.bottom;
			m_PresentParams.BackBufferFormat			= D3DFMT_UNKNOWN;
			m_PresentParams.BackBufferCount				= 1;
			m_PresentParams.MultiSampleType				= D3DMULTISAMPLE_NONE;
			m_PresentParams.MultiSampleQuality			= 0;
			m_PresentParams.SwapEffect					= D3DSWAPEFFECT_COPY;
			m_PresentParams.hDeviceWindow				= hWnd;
			m_PresentParams.Windowed					= TRUE;
			m_PresentParams.EnableAutoDepthStencil		= FALSE;
			m_PresentParams.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
			m_PresentParams.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
			m_PresentParams.PresentationInterval		= ( (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE );


			// Attempt to create the device
			hResult = m_pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_PresentParams, &m_pDevice );
			if( FAILED( hResult ) )
			{
				m_pDirect3D->Release();
				m_pDirect3D = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D device (0x%X) !!!\n", hResult );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}


			// Attempt to create the sprite library
			hResult = D3DXCreateSprite( m_pDevice, &m_pSprite );
			if( FAILED( hResult ) )
			{
				m_pDevice->Release();
				m_pDevice = nullptr;

				m_pDirect3D->Release();
				m_pDirect3D = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D sprite (0x%X) !!!\n", hResult );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}


			// Attempt to create the font
			hResult = D3DXCreateFontW( m_pDevice, 23, 0, FW_SEMIBOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"ARIAL", &m_pFont );
			if( FAILED( hResult ) )
			{
				m_pSprite->Release();
				m_pSprite = nullptr;

				m_pDevice->Release();
				m_pDevice = nullptr;

				m_pDirect3D->Release();
				m_pDirect3D = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D font (0x%X) !!!\n", hResult );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}


			// Attempt to create the rect texture
			hResult = D3DXCreateTexture( m_pDevice, 1, 1, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &m_pTexture );
			if( FAILED( hResult ) )
			{
				m_pFont->Release();
				m_pFont = nullptr;

				m_pSprite->Release();
				m_pSprite = nullptr;

				m_pDevice->Release();
				m_pDevice = nullptr;

				m_pDirect3D->Release();
				m_pDirect3D = nullptr;

				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Initialize - failed to initialize Direct3D rect (0x%X) !!!\n", hResult );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}
			

			// Fill the rect texture
			D3DLOCKED_RECT area;
			m_pTexture->LockRect( 0, &area, 0, D3DLOCK_DISCARD );
			((DWORD*)area.pBits)[ 0 ] = 0xFFFFFFFF;
			m_pTexture->UnlockRect( 0 );


			// Preload font characters for speed.
			m_pFont->PreloadCharacters( '0', '9' );
			m_pFont->PreloadCharacters( 'A', 'Z' );
			m_pFont->PreloadCharacters( 'a', 'z' );


			// Store the window and its style flags
			m_hWnd			= hWnd;
			m_lWindowStyle	= GetWindowLong( m_hWnd, GWL_STYLE );


			// Begin the new scene (graphics are always in an active state)
			m_pDevice->Clear( 0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB( m_ClearColor.red, m_ClearColor.green, m_ClearColor.blue ), 1.0f, 0 );

			hResult = m_pDevice->BeginScene();
			if( SUCCEEDED( hResult ) )
			{
				hResult = m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );

				if( SUCCEEDED( hResult ) )
				{
					hResult = m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
					hResult = m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
				}
			}

			// Preallocate output buffer
			m_nBufferSize		= 16;
			m_pwszBuffer		= new wchar_t[ m_nBufferSize ];


			// Success!
			m_eStatus = E_INITIALIZED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// INITIALIZE
		bool GraphicsManager::Initialize( const wchar_t* title, Size size, bool vsync )
		{			
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_UNINITIALIZED, "GraphicsManager::Initialize - wrapper has already been initialized" );
			if( m_eStatus != E_UNINITIALIZED )
				return false;


			// Create a new window
			HWND hWnd = InitializeWindow( title, (LONG)size.width, (LONG)size.height );
			if( hWnd == nullptr )
			{
				// MESSAGE
				char szBuffer[ 64 ];
				_snprintf_s( szBuffer, 64, _TRUNCATE, "!!! GraphicsManager::Initialize - invalid window handle !!!\n" );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}


			// Run the default Initialize method to configure Direct3D
			return Initialize( vsync );
		}
		//*************************************************************//



		//*************************************************************//
		// UPDATE
		bool GraphicsManager::Update( void )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::Update - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			
			// Centered output onto fullscreen display?
			float offsetX = (m_WindowSize.width - m_DesiredSize.width) / 2;
			float offsetY = (m_WindowSize.height - m_DesiredSize.height) / 2;
			if( offsetX > 0 || offsetY > 0 )
			{	
				// Store original transform
				HRESULT result;
				D3DXMATRIX original, transform;
				m_pSprite->GetTransform( &original );
				D3DXMatrixIdentity( &transform );
				
				Rectangle sides[ 4 ] = 
				{
					{ 0, 0, offsetX, m_WindowSize.height },	// left
					{ m_WindowSize.width - offsetX, 0, m_WindowSize.width, m_WindowSize.height }, // right
					
					{ 0, 0, m_WindowSize.width, offsetY },	// top
					{ 0, m_WindowSize.height - offsetY, m_WindowSize.width, m_WindowSize.height }, // bottom
				};

				for( int i = 0; i < 4; i++ )
				{
					transform.m[0][0] = (FLOAT)(sides[i].right  - sides[i].left);
					transform.m[1][1] = (FLOAT)(sides[i].bottom - sides[i].top);

					transform.m[3][0] = (FLOAT)(sides[i].left);
					transform.m[3][1] = (FLOAT)(sides[i].top);

					// Apply the transform
					m_pSprite->SetTransform(&transform);

					// Draw the rectangle
					result = m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, D3DCOLOR_XRGB( 0, 0, 0 ) );
				}


				// Restore the original transform
				m_pSprite->SetTransform(&original);
				
				

				// Clip cursor only if the window is active
				if( m_hWnd == GetActiveWindow() )
				{
					RECT clip = { (LONG)offsetX, (LONG)offsetY, (LONG)(m_WindowSize.width-offsetX), (LONG)(m_WindowSize.height-offsetY) };
					ClipCursor( &clip );
					m_bCursorClipped = true;
				}
				else if( m_bCursorClipped == true )
				{
					// Unclip the cursor
					ClipCursor( nullptr );
					m_bCursorClipped = false;
				}
			}
			else if( m_bCursorClipped == true )
			{
				// Unclip the cursor
				ClipCursor( nullptr );
				m_bCursorClipped = false;
			}


			// End sprite rendering
			HRESULT hResult = m_pSprite->End();
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to end Direct3D sprite scene (0x%X) !!!\n", hResult );
				Print( szBuffer );
				//OutputDebugStringA( szBuffer );

				//return false;
			}

			// End the scene
			hResult = m_pDevice->EndScene();
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to end Direct3D scene (0x%X) !!!\n", hResult );
				Print( szBuffer );
				//OutputDebugStringA( szBuffer );

				//return false;
			}


			// Check for lost device (could happen from an ALT+TAB or ALT+ENTER)
			hResult = m_pDevice->TestCooperativeLevel();
			if( hResult == D3DERR_DEVICENOTRESET )
			{
				m_pFont->OnLostDevice();
				m_pSprite->OnLostDevice();
				m_pDevice->Reset( &m_PresentParams );
				m_pSprite->OnResetDevice();
				m_pFont->OnResetDevice();
			}
			else if( hResult == D3D_OK )
			{
				// Present the current frame to the screen
				hResult = m_pDevice->Present( nullptr, nullptr, nullptr, nullptr );

				// Could fail if the fullscreen application loses focus,
				// but that can be ignored
				if( FAILED( hResult ) )
				{
					// MESSAGE
					char szBuffer[ 128 ];
					_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to present Direct3D scene (0x%X) !!!\n", hResult );
					Print( szBuffer );
					//OutputDebugStringA( szBuffer );

					//return false;
				}
			}


			// Clear the new frame
			m_pDevice->Clear( 0, nullptr, D3DCLEAR_TARGET, (D3DCOLOR)m_ClearColor, 1.0f, 0 );


			// Begin the new scene
			hResult = m_pDevice->BeginScene();
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to begin Direct3D scene (0x%X) !!!\n", hResult );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}

			// Begin sprite rendering
			hResult = m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Update - failed to begin Direct3D sprite scene (0x%X) !!!\n", hResult );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}

			
			// Pixelated sampler state?
			if( m_bPixelated == true )
			{
				hResult = m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
				hResult = m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
			}



			// Run the message loop
			if( m_bWindowOwned == true )
			{
				MSG msg = { };
				while( PeekMessageW( &msg, NULL, 0, 0, PM_REMOVE ) == TRUE )
				{ 
					// Quit the application?
					if( msg.message == WM_QUIT )
						return false;
		
					// Send the message to the window proc
					DispatchMessageW( &msg );
				}
			}

			
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// TERMINATE
		bool GraphicsManager::Terminate( void )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::Terminate - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			
			// Unclip cursor
			if( m_bCursorClipped == true )
			{
				ClipCursor( nullptr );
				m_bCursorClipped = false;
			}


			// Deallocate output buffer
			delete[] m_pwszBuffer;
			m_pwszBuffer = nullptr;
			m_nBufferSize = 0;


			// Clear handles
			m_HandleManager.Clear();


			// Release resources
			m_pTexture->Release();
			m_pTexture = nullptr;

			m_pFont->Release();
			m_pFont = nullptr;

			m_pSprite->Release();
			m_pSprite = nullptr;

			m_pDevice->Release();
			m_pDevice = nullptr;

			// Release Direct3D
			m_pDirect3D->Release();
			m_pDirect3D = nullptr;

			CoUninitialize();

			// Destroy the window
			if( m_bWindowOwned == true )
			{
				DestroyWindow( m_hWnd );
				
				// Unregister the window class
				UnregisterClassW( SGD_WINDOW_CLASS_NAME, (HINSTANCE)GetModuleHandle( nullptr ) );
			}


			m_eStatus = E_DESTROYED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// SET CLEAR COLOR
		bool GraphicsManager::SetClearColor( Color color )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::SetClearColor - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Store the parameter (and force opacity)
			m_ClearColor = color;
			m_ClearColor.alpha = 255;

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// SET PIXELATED MODE
		bool GraphicsManager::SetPixelatedMode( bool pixelated )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::SetPixelatedMode - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Store the parameter
			m_bPixelated = pixelated;

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// SHOW CURSOR		
		bool GraphicsManager::ShowCursor( bool show )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::ShowCursor - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Set Win32 flag
			if( show == true )
				while( ::ShowCursor( (BOOL)show ) < 0 );	// increase counter to visible state
			else
				while( ::ShowCursor( (BOOL)show ) >= 0 );	// decrease counter to invisible state
				
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// SHOW CONSOLE WINDOW
		bool GraphicsManager::ShowConsoleWindow( bool show )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::ShowConsoleWindow - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Show/create the console window?
			if( show == true )
			{
				// Attempt to create a new console window (only 1 at a time)
				AllocConsole();
	
				
				// Get the window title
				wchar_t title[ 256 ] = L"SGD Application";
				GetWindowTextW( m_hWnd, title, 256 );

				// Set the console title
				SetConsoleTitleW( title );

				// Attach stdio handles to new window
				FILE* ignored = nullptr;
				freopen_s( &ignored, "CONIN$",  "r", stdin  );
				freopen_s( &ignored, "CONOUT$", "w", stdout );
				freopen_s( &ignored, "CONOUT$", "w", stderr );
				std::ios::sync_with_stdio();
		
				// Disable the close button (which terminates the program)	
				Sleep( 10 );
				HWND hConsole = GetConsoleWindow();
				HMENU hMenu = GetSystemMenu( hConsole, FALSE );
				EnableMenuItem( hMenu, SC_CLOSE, MF_GRAYED );
			}
			else 
			{
				// Remove console window
				FreeConsole();
			}


			// success!
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// RESIZE
		bool GraphicsManager::Resize( Size size, bool windowed )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::Resize - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Round the size
			int width	= (int)(size.width  + 0.5f);
			int height	= (int)(size.height + 0.5f);

			
			// Supported format
			D3DFORMAT format = D3DFMT_UNKNOWN;

			// Verify the resolution for full screen
			if( windowed == false )
			{
				D3DFORMAT larger = D3DFMT_UNKNOWN;
				int nLargerWidth  = INT_MAX;
				int nLargerHeight = INT_MAX;


				// Search the device types for a supported color format
				D3DFORMAT formats[ ] = { D3DFMT_X8R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5 };
				for( int i = 0; i < _countof(formats) && format == D3DFMT_UNKNOWN; i++ )
				{	
					HRESULT hResult = m_pDirect3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, formats[ i ], formats[ i ], FALSE );
					if( FAILED( hResult ) )
						continue;

					// Search the adapter modes for a matching resolution
					int numFormats = m_pDirect3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, formats[ i ] );
					for( int j = 0; j < numFormats; j++ )
					{
						D3DDISPLAYMODE mode = { };
						m_pDirect3D->EnumAdapterModes( D3DADAPTER_DEFAULT, formats[ i ], j, &mode );

						// Exact resolution?
						if( (int)mode.Width == width && (int)mode.Height == height )
						{
							format = formats[ i ];
							break;
						}
						// Larger?
						else if( (int)mode.Width >= width && (int)mode.Height >= height )
						{
							// Smaller than the existing larger-size?
							if( (int)mode.Width <= nLargerWidth && (int)mode.Height <= nLargerHeight )
							{
								// But not the exact same size as the existing larger-size?
								if( (int)mode.Width != nLargerWidth || (int)mode.Height != nLargerHeight )
								{
									larger = formats[ i ];
									nLargerWidth  = (int)mode.Width;
									nLargerHeight = (int)mode.Height;
								}
							}
						}
					}
				}

				// Use a larger resolution?
				if( format == D3DFMT_UNKNOWN )
				{
					if( larger != D3DFMT_UNKNOWN )	// larger resolution exists
					{
						// MESSAGE
						char szBuffer[ 128 ];
						_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Resize - Fullscreen resolution %ix%i is enlarged to %ix%i !!!\n", width, height, nLargerWidth, nLargerHeight );
						Print( szBuffer );
						//OutputDebugStringA( szBuffer );
						

						format = larger;
						width  = nLargerWidth;
						height = nLargerHeight;
					}
					else							// larger resolution DOES NOT exist
					{
						// MESSAGE
						char szBuffer[ 128 ];
						_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::Resize - Failed to go fullscreen with resolution %ix%i !!!\n", width, height );
						Alert( szBuffer );
						//OutputDebugStringA( szBuffer );

						return false;
					}
				}

			}


			// Set the new device presentation parameters
			m_PresentParams.BackBufferFormat	= format;
			m_PresentParams.Windowed			= windowed;
			m_PresentParams.BackBufferWidth		= width;
			m_PresentParams.BackBufferHeight	= height;

			// Reset the device
			m_pFont->OnLostDevice();
			m_pSprite->OnLostDevice();
			m_pDevice->Reset( &m_PresentParams );
			m_pSprite->OnResetDevice();
			m_pFont->OnResetDevice();


			// Reset the window
			if( windowed == true )
			{
				// Set window style flags
				SetWindowLong( m_hWnd, GWL_STYLE, m_lWindowStyle );

				// Setup the desired client area size
				RECT rWindow;
				rWindow.left	= 0;
				rWindow.top		= 0;
				rWindow.right	= width;
				rWindow.bottom	= height;

				// Get the window dimensions that have the desired client area within
				AdjustWindowRectEx( &rWindow, (DWORD)m_lWindowStyle, FALSE, WS_EX_APPWINDOW );

				// Calculate the width/height of that window's dimensions
				int windowWidth		= rWindow.right - rWindow.left;
				int windowHeight	= rWindow.bottom - rWindow.top;

				// Center the window on the screen
				SetWindowPos( m_hWnd, HWND_NOTOPMOST, (GetSystemMetrics(SM_CXSCREEN) - windowWidth)/2, (GetSystemMetrics(SM_CYSCREEN) - windowHeight)/2, windowWidth, windowHeight, SWP_SHOWWINDOW );
			}
			else
			{
				// Set window style flags
				SetWindowLong( m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP );
				
				// Refresh the window
				SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
			}


			// Store the new dimensions
			m_DesiredSize = size;
			m_WindowSize = { (float)width, (float)height };

			// Center output onto fullscreen display
			float offsetX = (m_WindowSize.width - m_DesiredSize.width) / 2;
			float offsetY = (m_WindowSize.height - m_DesiredSize.height) / 2;
			if( offsetX > 0 || offsetY > 0 )
			{
				D3DXMATRIX transform;
				D3DXMatrixTranslation( &transform, offsetX, offsetY, 0.0f );
				
				m_pSprite->SetTransform( &transform );
			}
			else 
			{
				D3DXMATRIX identity;
				D3DXMatrixIdentity( &identity );
				
				m_pSprite->SetTransform( &identity );
			}

			// Begin the new scene
			HRESULT hResult = m_pDevice->BeginScene();

			if( SUCCEEDED( hResult ) )
			{
				hResult = m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
							
				// Pixelated sampler state?
				if( SUCCEEDED( hResult ) && m_bPixelated == true )
				{
					hResult = m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
					hResult = m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
				}
			}

			return true;
		}
		//*************************************************************//
		


		//*************************************************************//
		// IS FOREGROUND WINDOW
		bool GraphicsManager::IsForegroundWindow( void )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::IsForegroundWindow - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Is the window in the foreground (top-most)
			return (::GetForegroundWindow() == m_hWnd);
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW STRING
		bool GraphicsManager::DrawString( const wchar_t* text, Point position, Color color )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::DrawString - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Sanity-check the text parameter
			SGD_ASSERT( text != nullptr, "GraphicsManager::DrawString - text cannot be null" );
			if( text == nullptr || text[0] == L'\0' )
				return false;


			RECT region = { (LONG)position.x, (LONG)position.y };
			
			int result = m_pFont->DrawTextW( m_pSprite, text, -1, &region, DT_NOCLIP, (D3DCOLOR)color );
			if( result == 0 )
			{
				// MESSAGE
				wchar_t wszBuffer[ 256 ];
				_snwprintf_s( wszBuffer, 256, _TRUNCATE, L"!!! GraphicsManager::DrawString - failed to draw Direct3D text (\"%ws\") !!!", text );
				Alert( wszBuffer );
				//OutputDebugStringW( wszBuffer );
				//OutputDebugStringA( "\n" );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW STRING
		bool GraphicsManager::DrawString( const char* text, Point position, Color color )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::DrawString - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Sanity-check the text parameter
			SGD_ASSERT( text != nullptr, "GraphicsManager::DrawString - text cannot be null" );
			if( text == nullptr || text[0] == '\0' )
				return false;


			// Reallocate output buffer?
			int size = MultiByteToWideChar( CP_UTF8, 0, text, -1, nullptr, 0 );
			if( size > m_nBufferSize )
			{
				delete[] m_pwszBuffer;
				m_nBufferSize = size;
				m_pwszBuffer = new wchar_t[ m_nBufferSize ];
			}

			// Convert to UTF16
			int ret = MultiByteToWideChar( CP_UTF8, 0, text, -1, m_pwszBuffer, m_nBufferSize );			
			if( ret == 0 )
			{
				// MESSAGE
				char szBuffer[ 256 ];
				_snprintf_s( szBuffer, 256, _TRUNCATE, "!!! GraphicsManager::DrawString - invalid string (\"%hs\") !!!", text );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );
				//OutputDebugStringA( "\n" );

				return false;
			}

			
			// Use the UTF16 draw
			return DrawString( m_pwszBuffer, position, color );
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW LINE
		bool GraphicsManager::DrawLine( Point position1, Point position2, Color color, unsigned int lineWidth )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::DrawLine - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Calculate the difference in positions
			float dX = position2.x - position1.x;
			float dY = position2.y - position1.y;

			// Is the line too small?
			if( dX == 0 && dY == 0 || lineWidth <= 0 )
				return false;

			
			// Store original transform
			D3DXMATRIX original, world;
			m_pSprite->GetTransform( &original );

			// Create transform matrix
			D3DXMATRIX transform;
			D3DXMatrixIdentity( &transform );

			if( dX == 0.0f )
			{
				// Vertical line
				transform.m[ 0 ][ 0 ] = (FLOAT)(lineWidth);
				transform.m[ 3 ][ 0 ] = (FLOAT)(position1.x - (lineWidth*0.5f) + 0.5f);
				if( dY > 0.0f )
				{
					transform.m[ 1 ][ 1 ] = (FLOAT)(dY + 1);
					transform.m[ 3 ][ 1 ] = (FLOAT)(position1.y);
				}
				else 
				{
					transform.m[ 1 ][ 1 ] = (FLOAT)(-dY + 1);
					transform.m[ 3 ][ 1 ] = (FLOAT)(position2.y);
				}
			}
			else if( dY == 0.0f )
			{
				// Horizontal line
				transform.m[ 1 ][ 1 ] = (FLOAT)(lineWidth);
				transform.m[ 3 ][ 1 ] = (FLOAT)(position1.y - (lineWidth*0.5f) + 0.5f);
				if( dX > 0.0f )
				{
					transform.m[ 0 ][ 0 ] = (FLOAT)(dX + 1);
					transform.m[ 3 ][ 0 ] = (FLOAT)(position1.x);
				}
				else 
				{
					transform.m[ 0 ][ 0 ] = (FLOAT)(-dX + 1);
					transform.m[ 3 ][ 0 ] = (FLOAT)(position2.x);
				}
			}
			else 
			{
				// Adjust for counter-clockwise rotation
				if( dY < 0 )
				{
					position1 = position2;		// partial swap, as pt2 is not used again
					dX = -dX;
					dY = -dY;
				}


				// Calculate the length and direction
				float length = sqrtf( dX * dX + dY * dY );
				float angle  = acos( dX / length );			// abbreviated DotProduct against <1, 0>
				
				// Indefinite angle is treated as 0 ...
				if( _isnan( angle ) != 0 )
					angle = 0.0f;


				float cos = cosf( angle );
				float sin = sinf( angle );


				// The positions are inclusive: (0, 0) -> (1, 0) should be 2 pixels
				length = length + 1.0f;
				float halfWidth = (float)(lineWidth * 0.5f);



				// Rotation & scale values
				transform.m[ 0 ][ 0 ] = (FLOAT)(length    *  cos);
				transform.m[ 0 ][ 1 ] = (FLOAT)(length    *  sin);
				transform.m[ 1 ][ 0 ] = (FLOAT)(lineWidth * -sin);
				transform.m[ 1 ][ 1 ] = (FLOAT)(lineWidth *  cos);

				// Translate and rotate around (0, halfWidth)
				if( dX < 0.0f || dY < 0.0f )
				{
					transform.m[ 3 ][ 0 ] = (FLOAT)( position1.x + (halfWidth *  sin) + 1.0f );	// offset ...
					transform.m[ 3 ][ 1 ] = (FLOAT)( position1.y + (halfWidth * -cos) );
				}
				else 
				{
					transform.m[ 3 ][ 0 ] = (FLOAT)( position1.x + (halfWidth *  sin) );
					transform.m[ 3 ][ 1 ] = (FLOAT)( position1.y + (halfWidth * -cos) );
				}
			}

			

			// Apply the transform
			D3DXMatrixMultiply( &world, &transform, &original );
			m_pSprite->SetTransform( &world );


			// Draw the rectangle
			HRESULT result = m_pSprite->Draw( m_pTexture, nullptr, nullptr, nullptr, (D3DCOLOR)color );


			// Restore the transform
			m_pSprite->SetTransform( &original );

			if( FAILED( result ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::DrawLine - failed to draw line (0x%X) !!!\n", result );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW RECTANGLE
		bool GraphicsManager::DrawRectangle( Rectangle rect, Color fillColor, Color lineColor, unsigned int lineWidth )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::DrawRectangle - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Is the rectangle inverted?
			SGD_ASSERT( rect.IsEmpty() == false, "GraphicsManager::DrawRectangle - rectangle is empty" );
			if( rect.IsEmpty() == true )
				return false;


			HRESULT result = 0;
			
			// Store original transform
			D3DXMATRIX original, world;
			m_pSprite->GetTransform( &original );

			// Create transform matrix
			D3DXMATRIX transform;
			D3DXMatrixIdentity( &transform );


			// Clamp the rectangle to pixel coordinates
			int width	= (int)(rect.right  - rect.left + 0.5f);
			int height	= (int)(rect.bottom - rect.top  + 0.5f);
			int left	= (int)(rect.left + (rect.left > 0 ? 0.5f : -0.5f) );
			int top		= (int)(rect.top  + (rect.top  > 0 ? 0.5f : -0.5f) );
			int right	= left + width;
			int bottom	= top  + height;
			

			// Should the frame be drawn?
			if( lineColor.alpha > 0 && lineWidth > 0 )
			{
				// Calculate the size
				int smallLineHalf = (int)(lineWidth / 2);
				int largeLineHalf = (int)(lineWidth - smallLineHalf);


				// Calculate the D3D color
				DWORD dwColor = (D3DCOLOR)lineColor;


				// Set transform for top line
				transform.m[ 0 ][ 0 ] = (FLOAT)(width + (largeLineHalf - smallLineHalf));
				transform.m[ 1 ][ 1 ] = (FLOAT)lineWidth;

				transform.m[ 3 ][ 0 ] = (FLOAT)(left - largeLineHalf);
				transform.m[ 3 ][ 1 ] = (FLOAT)(top  - largeLineHalf);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the top line
				m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, dwColor );


				// Set transform for bottom line
				transform.m[ 3 ][ 0 ] = (FLOAT)(left   + smallLineHalf);
				transform.m[ 3 ][ 1 ] = (FLOAT)(bottom - smallLineHalf);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the bottom line
				m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, dwColor );
				

				
				// Set transform for left line
				transform.m[ 0 ][ 0 ] = (FLOAT)lineWidth;
				transform.m[ 1 ][ 1 ] = (FLOAT)(height + (largeLineHalf - smallLineHalf));

				transform.m[ 3 ][ 0 ] = (FLOAT)(left - largeLineHalf);
				transform.m[ 3 ][ 1 ] = (FLOAT)(top  + smallLineHalf);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the left line
				m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, dwColor );


				// Set transform for right line
				transform.m[ 3 ][ 0 ] = (FLOAT)(right - smallLineHalf);
				transform.m[ 3 ][ 1 ] = (FLOAT)(top   - largeLineHalf);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the right line
				result = m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, dwColor );


				
				// Should the rect within be filled?
				if( fillColor.alpha > 0 )
				{
					// Set the transform for the fill rect (shrunk within the frame)
					transform.m[0][0] = (FLOAT)(width  - (smallLineHalf * 2));
					transform.m[1][1] = (FLOAT)(height - (smallLineHalf * 2));

					transform.m[3][0] = (FLOAT)(left + smallLineHalf);
					transform.m[3][1] = (FLOAT)(top  + smallLineHalf);

					// Apply the transform
					D3DXMatrixMultiply( &world, &transform, &original );
					m_pSprite->SetTransform( &world );

					// Draw the rectangle
					result = m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, (D3DCOLOR)fillColor );
				}
			}
			else if( fillColor.alpha > 0 ) // Just the rect?
			{
				// Set the transform for the full fill rect
				transform.m[0][0] = (FLOAT)(right  - rect.left);
				transform.m[1][1] = (FLOAT)(bottom - rect.top);

				transform.m[3][0] = (FLOAT)(left);
				transform.m[3][1] = (FLOAT)(top);

				// Apply the transform
				D3DXMatrixMultiply( &world, &transform, &original );
				m_pSprite->SetTransform( &world );

				// Draw the rectangle
				result = m_pSprite->Draw( m_pTexture, NULL, NULL, NULL, (D3DCOLOR)fillColor );
			}


			// Restore the transform
			m_pSprite->SetTransform( &original );

			
			if( FAILED( result ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::DrawRectangle - failed to draw rectangle (0x%X) !!!\n", result );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// LOAD TEXTURE
		HTexture GraphicsManager::LoadTexture( const wchar_t* filename, Color colorKey )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::LoadTexture - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return SGD::INVALID_HANDLE;

			SGD_ASSERT( filename != nullptr && filename[0] != L'\0', "GraphicsManager::LoadTexture - invalid filename" );
			if( filename == nullptr || filename[0] == L'\0' )
				return SGD::INVALID_HANDLE;


			// Attempt to find the texture in the Handle Manager
			SearchInfo search = { filename, nullptr, SGD::INVALID_HANDLE };
			m_HandleManager.ForEach( &GraphicsManager::FindTextureByName, &search );

			// If it was found, increase the reference & return the existing handle
			if( search.texture != NULL )
			{
				search.texture->unRefCount++;
				return search.handle;
			}


			// Could not find texture in the Handle Manager
			TextureInfo data = { };
			D3DXIMAGE_INFO info = { };
			D3DSURFACE_DESC surface = { };


			// Attempt to load from file
			HRESULT hResult = D3DXCreateTextureFromFileExW( m_pDevice, filename, 0, 0, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, (D3DCOLOR)colorKey, &info, nullptr, &data.texture );
			if( FAILED( hResult ) )
			{
				// MESSAGE
				wchar_t wszBuffer[ 256 ];
				_snwprintf_s( wszBuffer, 256, _TRUNCATE, L"!!! GraphicsManager::LoadTexture - failed to load texture file \"%ws\" (0x%X) !!!", filename, hResult );
				Alert( wszBuffer );
				//OutputDebugStringW( wszBuffer );
				//OutputDebugStringA( "\n" );
				//MessageBoxW( m_hWnd, wszBuffer, L"GraphicsManager::LoadTexture", MB_OK );

				return SGD::INVALID_HANDLE;
			}


			// Texture loaded successfully
			data.wszFilename = _wcsdup( filename );
			data.unRefCount = 1;


			// Compare surface description to the original image
			data.texture->GetLevelDesc( 0, &surface );

			if( surface.Width != info.Width || surface.Height != info.Height )
			{
				// MESSAGE
				wchar_t wszBuffer[ 256 ];
				_snwprintf_s( wszBuffer, 256, _TRUNCATE, L"!!! GraphicsManager::LoadTexture - Texture file \"%ws\" is stretched from %ux%u to %ux%u !!!\n", filename, info.Width, info.Height, surface.Width, surface.Height );
				Alert( wszBuffer );
				//OutputDebugStringW( wszBuffer );
			}


			// Store the buffer size
			data.fWidth  = (float)surface.Width;
			data.fHeight = (float)surface.Height;


			// Store texture into the Handle Manager
			return m_HandleManager.StoreData( data );
		}
		//*************************************************************//



		//*************************************************************//
		// LOAD TEXTURE
		HTexture GraphicsManager::LoadTexture( const char* filename, Color colorKey )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::LoadTexture - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return SGD::INVALID_HANDLE;

			SGD_ASSERT( filename != nullptr && filename[0] != '\0', "GraphicsManager::LoadTexture - invalid filename" );
			if( filename == nullptr || filename[0] == '\0' )
				return SGD::INVALID_HANDLE;


			// Convert the filename to UTF16
			wchar_t widename[ MAX_PATH * 4 ];
			int ret = MultiByteToWideChar( CP_UTF8, 0, filename, -1, widename, MAX_PATH * 4 );

			if( ret == 0 )
			{
				// MESSAGE
				char szBuffer[ 256 ];
				_snprintf_s( szBuffer, 256, _TRUNCATE, "!!! GraphicsManager::LoadTexture - invalid filename \"%hs\" (0x%X) !!!", filename, GetLastError() );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );
				//OutputDebugStringA( "\n" );

				return SGD::INVALID_HANDLE;
			}


			// Use the UTF16 load
			return LoadTexture( widename, colorKey );
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW TEXTURE
		bool GraphicsManager::DrawTexture( HTexture handle, Point position, float rotation, Vector rotationOffset, Color color, Size scale )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::DrawTexture - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			SGD_ASSERT( handle != SGD::INVALID_HANDLE, "GraphicsManager::DrawTexture - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the texture info from the handle manager
			TextureInfo* data = m_HandleManager.GetData( handle );
			SGD_ASSERT( data != nullptr, "GraphicsManager::DrawTexture - handle has expired" );
			if( data == nullptr )
				return false;

			
			// Store original transform
			D3DXMATRIX original, world;
			m_pSprite->GetTransform( &original );

			// Calculate transform matrix		
			D3DXMATRIX scaled;
			D3DXMATRIX rotated;
			D3DXMATRIX translated;

			D3DXMATRIX transform;
			D3DXMatrixIdentity( &transform );

			// Scale
			D3DXMatrixScaling( &scaled, scale.width, scale.height, 1.0f );
			transform *= scaled;

			// Rotate around the local point
			D3DXMatrixTranslation( &translated, -rotationOffset.x * scale.width, -rotationOffset.y * scale.height, 0.0f );
			transform *= translated;
			D3DXMatrixRotationZ( &rotated, rotation );
			transform *= rotated;
			D3DXMatrixTranslation( &translated, rotationOffset.x * scale.width, rotationOffset.y * scale.height, 0.0f );
			transform *= translated;

			// Translate
			D3DXMatrixTranslation( &translated, position.x, position.y, 0.0f );
			transform *= translated;


			// Apply the transform
			D3DXMatrixMultiply( &world, &transform, &original );
			m_pSprite->SetTransform( &world );


			// Draw the texture
			HRESULT result = m_pSprite->Draw( data->texture, nullptr, nullptr, nullptr, (D3DCOLOR)color );


			// Restore the transform
			m_pSprite->SetTransform( &original );


			if( FAILED( result ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::DrawTexture - failed to draw texture (0x%X) !!!\n", result );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// DRAW TEXTURE SECTION
		bool GraphicsManager::DrawTextureSection( HTexture handle, Point position, Rectangle section, float rotation, Vector rotationOffset, Color color, Size scale )
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::DrawTextureSection - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			SGD_ASSERT( handle != SGD::INVALID_HANDLE, "GraphicsManager::DrawTextureSection - invalid handle" );
			if( handle == SGD::INVALID_HANDLE )
				return false;


			// Get the texture info from the handle manager
			TextureInfo* data = m_HandleManager.GetData( handle );
			SGD_ASSERT( data != nullptr, "GraphicsManager::DrawTextureSection - handle has expired" );
			if( data == nullptr )
				return false;

			
			// Is the section inverted?
			SGD_ASSERT( section.IsEmpty() == false, "GraphicsManager::DrawTextureSection - section rectangle is empty" );
			if( section.IsEmpty() == true )
				return false;

		
			// Store original transform
			D3DXMATRIX original, world;
			m_pSprite->GetTransform( &original );

			// Calculate transform matrix
			D3DXMATRIX scaled;
			D3DXMATRIX rotated;
			D3DXMATRIX translated;

			D3DXMATRIX transform;
			D3DXMatrixIdentity( &transform );

			// Scale
			D3DXMatrixScaling( &scaled, scale.width, scale.height, 1.0f );
			transform *= scaled;

			// Rotate around the local point
			D3DXMatrixTranslation( &translated, -rotationOffset.x * scale.width, -rotationOffset.y * scale.height, 0.0f );
			transform *= translated;
			D3DXMatrixRotationZ( &rotated, rotation );
			transform *= rotated;
			D3DXMatrixTranslation( &translated, rotationOffset.x * scale.width, rotationOffset.y * scale.height, 0.0f );
			transform *= translated;

			// Translate
			D3DXMatrixTranslation( &translated, position.x, position.y, 0.0f );
			transform *= translated;


			// Apply the transform
			D3DXMatrixMultiply( &world, &transform, &original );
			m_pSprite->SetTransform( &world );


			// Draw the texture
			RECT source = { (LONG)section.left, (LONG)section.top, (LONG)section.right, (LONG)section.bottom };
			HRESULT result = m_pSprite->Draw( data->texture, &source, nullptr, nullptr, (D3DCOLOR)color );


			// Restore the transform
			m_pSprite->SetTransform( &original );

			
			if( FAILED( result ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! GraphicsManager::DrawTexture - failed to draw texture (0x%X) !!!\n", result );
				Alert( szBuffer );
				//OutputDebugStringA( szBuffer );

				return false;
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UNLOAD TEXTURE
		bool GraphicsManager::UnloadTexture( HTexture& handle )	
		{
			// Sanity-check the wrapper's status
			SGD_ASSERT( m_eStatus == E_INITIALIZED, "GraphicsManager::UnloadTexture - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Quietly ignore bad handles
			if( handle == INVALID_HANDLE )
				return false;


			// Get the texture info from the handle manager
			TextureInfo* data = m_HandleManager.GetData( handle );
			if( data == nullptr )
				return false;

			// Release a reference
			data->unRefCount--;

			// Is this the last reference?
			if( data->unRefCount == 0 )
			{
				// Release the texture
				data->texture->Release();

				// Deallocate the name
				delete[] data->wszFilename;

				// Remove the audio info from the handle manager
				m_HandleManager.RemoveData( handle, nullptr );
				data = nullptr;
			}


			// Invalidate the handle
			handle = INVALID_HANDLE;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// FIND TEXTURE BY NAME
		/*static*/ bool GraphicsManager::FindTextureByName( Handle handle, TextureInfo& data, SearchInfo* extra )
		{		
			// Compare the names
			if( wcscmp( data.wszFilename, extra->filename ) == 0 )
			{
				// Texture does exist!
				extra->texture	= &data;
				extra->handle	= handle;
				return false;
			}

			// Did not find yet
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// INITIALIZE WINDOW
		HWND GraphicsManager::InitializeWindow( const wchar_t* title, LONG width, LONG height )
		{
			// Get the application instance
			HINSTANCE hInstance = (HINSTANCE)GetModuleHandle( nullptr );

			// Window class description
			WNDCLASSEXW	winClassEx;

			// Set window class attributes
			winClassEx.cbSize			= sizeof(winClassEx);
			winClassEx.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
			winClassEx.lpfnWndProc		= &GraphicsManager::WindowMessageProc;
			winClassEx.cbClsExtra		= 0;
			winClassEx.cbWndExtra		= 0;
			winClassEx.hInstance		= hInstance;
			winClassEx.hIcon			= LoadIconW( NULL, (LPWSTR)IDI_APPLICATION );
			winClassEx.hIconSm			= LoadIconW( NULL, (LPWSTR)IDI_APPLICATION );
			winClassEx.hCursor			= LoadCursorW( NULL, (LPWSTR)IDC_ARROW );
			winClassEx.hbrBackground	= (HBRUSH)(1+COLOR_WINDOW);
			winClassEx.lpszMenuName		= NULL; 
			winClassEx.lpszClassName	= SGD_WINDOW_CLASS_NAME;

			// Register the window class
			ATOM atomClass = RegisterClassExW( &winClassEx );
			if( atomClass == 0 )
				return 0;


			// Setup window style flags
			DWORD dwWindowStyleFlags = WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			// Setup the desired client area size
			RECT rWindow = { 0, 0, width, height };

			// Get the dimensions of a window that will have a client rect that
			// will really be the resolution we're looking for.
			AdjustWindowRectEx( &rWindow, dwWindowStyleFlags, FALSE, WS_EX_APPWINDOW );
	
			// Calculate the the window's dimensions
			int nWindowWidth	= rWindow.right  - rWindow.left;
			int nWindowHeight	= rWindow.bottom - rWindow.top;


			// Create the window
			HWND hWnd = CreateWindowExW(
							WS_EX_APPWINDOW,										// extended style flags
							SGD_WINDOW_CLASS_NAME,									// window class name
							title,													// title
							dwWindowStyleFlags,										// window style flags
							(GetSystemMetrics(SM_CXSCREEN) - nWindowWidth)/2,		// position (x, y)
							(GetSystemMetrics(SM_CYSCREEN) - nWindowHeight)/2,		//	- centered on the desktop
							nWindowWidth,											// width
							nWindowHeight,											// height
							HWND_DESKTOP,											// parent window handle
							NULL,													// menu handle
							hInstance,												// application instance
							NULL													// creation parameters
						);

			// Did that fail?
			if( hWnd == 0 )
				UnregisterClassW( SGD_WINDOW_CLASS_NAME, hInstance );
			else 
			{
				m_bWindowOwned = true;

				// Configure the console window
				HWND hConsole = GetConsoleWindow();
				if( hConsole != nullptr )
				{
					// Set title
					SetConsoleTitleW( title );
		
					// Disable the close button (which terminates the program)	
					Sleep( 10 );
					HMENU hMenu = GetSystemMenu( hConsole, FALSE );
					EnableMenuItem( hMenu, SC_CLOSE, MF_GRAYED );
				}
			}
			
			// Return the handle to the new window
			return hWnd;
		}
		//*************************************************************//



		//*************************************************************//
		// WINDOW MESSAGE PROC
		/*static*/ LRESULT CALLBACK GraphicsManager::WindowMessageProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
		{
			// Inform the singleton
			if( s_Instance != nullptr )
				if( s_Instance->HandleMessage( hWnd, msg, wParam, lParam ) == true )
					return 0;

			// Send unhandled messages to the default window proc
			return DefWindowProcW( hWnd, msg, wParam, lParam );
		}
		//*************************************************************//



		//*************************************************************//
		// HANDLE MESSAGE
		bool GraphicsManager::HandleMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
		{
			// What is the message?
			switch( msg )
			{
			case WM_CREATE:			// Window constructed
				return true;
	
			case WM_CLOSE:			// Window closed
				DestroyWindow( hWnd );	// completely destroy the window
				return true;

			case WM_DESTROY:		// Window destroyed
				PostQuitMessage( 0 );	// completely quit the application
				return true;

			case WM_SYSKEYUP:		// System menu (ALT pop-up)
			case WM_SYSCHAR:
				return true;

			case WM_ACTIVATE:		// Window activated / deactivated
				if( LOWORD( wParam ) != WA_INACTIVE )	//	gaining focus (unpause)
				{
				}
				else									//	losing focus (pause)
				{
				}
				return true;

			case WM_PAINT:			// Window needs repainting
				ValidateRect( hWnd, nullptr );	// ignore painting
				return true;

			case WM_INPUT_DEVICE_CHANGE:	// raw input hardware change
			case WM_INPUT:					// raw input
			case WM_DEVICECHANGE:			// hardware change
				PostMessageW( hWnd, WM_USER | msg, wParam, lParam );
				return true;
	
			default:				// Any unhandled messages
				return false;
			}
		}
		//*************************************************************//

		
	}	// namespace SGD_IMPLEMENTATION

}	// namespace SGD
