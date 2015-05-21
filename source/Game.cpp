//*********************************************************************//
//	File:		Game.cpp
//	Author:		
//	Course:		
//	Purpose:	Game class initializes the SGD Wrappers
//				and runs the game state machine
//*********************************************************************//

#include "Game.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Utilities.h"

#include "BitmapFont.h"
#include "IGameState.h"
#include "MainMenuState.h"

#include <ctime>
#include <cstdlib>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


//*********************************************************************//
// SINGLETON
//	- instantiate the static member
/*static*/ Game* Game::s_pInstance = nullptr;

// GetInstance
//	- allocate the singleton if necessary
//	- return the singleton
/*static*/ Game* Game::GetInstance( void )
{
	if( s_pInstance == nullptr )
		s_pInstance = new Game;

	return s_pInstance;
}

// DeleteInstance
//	- deallocate the singleton
/*static*/ void Game::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


//*********************************************************************//
// Initialize
//	- initialize the SGD wrappers
//	- start in the MainMenuState
bool Game::Initialize( void )
{
	// Seed First!
	srand( (unsigned int)time( nullptr ) );
	rand();


	// Try to initialize the wrappers
	// (Graphics Manager MUST be first!)
	if( SGD::GraphicsManager::GetInstance()->Initialize( L"Stardust Crusader", m_szScreenSize, false ) == false
		|| SGD::InputManager::GetInstance()->Initialize() == false 
		|| SGD::AudioManager::GetInstance()->Initialize() == false )
		return false;	// failure!!!

	m_hMainMenuBackground = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/ELW_TitleScreen1.png");
	m_PlayerImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/ELW_Character1Sprite.png", SGD::Color{ 255, 255, 255, 255 });

	
// Hide the console window
#if !defined( DEBUG ) && !defined( _DEBUG )
	SGD::GraphicsManager::GetInstance()->ShowConsoleWindow( false );
#endif

	
	// Change the background color
	SGD::GraphicsManager::GetInstance()->SetClearColor( { 0, 0, 0 } );	// black


	

	// Allocate & Initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize();

	
	// Start in the MainMenuState
	ChangeState( MainMenuState::GetInstance() );
	

	// Store the starting time
	m_ulGameTime = GetTickCount();
	return true;	// success!
}

//*********************************************************************//
// Update
//	- update the SGD wrappers
//	- update & render the current state
int	Game::Update( void )
{
	// Try to update the wrappers
	if( SGD::GraphicsManager::GetInstance()->Update() == false 
		|| SGD::InputManager::GetInstance()->Update() == false 
		|| SGD::AudioManager::GetInstance()->Update() == false )
		return +1;	// exit when window is closed

	
	// Calculate the elapsed time between frames
	unsigned long now = GetTickCount();
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;
	m_ulGameTime = now;
	
	// Cap the elapsed time to 1/8th of a second
	if( elapsedTime > 0.125f )
		elapsedTime = 0.125f;


	// Update & Render the current state
	if( m_pCurrState->Update( elapsedTime ) == false )
		return +1;	// exit success

	m_pCurrState->Render( elapsedTime );

	return 0;		// keep running
}

//*********************************************************************//
// Terminate
//	- exit the current state
//	- terminate the SGD wrappers
void Game::Terminate( void )
{
	// Exit the current state
	ChangeState( nullptr );


	// Terminate & Deallocate the font
	if( m_pFont != nullptr )
	{
		m_pFont->Terminate();
		delete m_pFont;
	}

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hMainMenuBackground);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_PlayerImg);


	// Terminate the SGD wrappers (in reverse order)
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();
	
	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();
	
	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();
}


//*********************************************************************//
// ChangeState
//	- unload the old state
//	- load the new state
void Game::ChangeState( IGameState* pNextState )
{
	// Exit the current state (if it exists)
	if( m_pCurrState != nullptr )
		m_pCurrState->Exit();

	// Store the new state
	m_pCurrState = pNextState;

	// Enter the new state (if it exists)
	if( m_pCurrState != nullptr )
		m_pCurrState->Enter();
}

