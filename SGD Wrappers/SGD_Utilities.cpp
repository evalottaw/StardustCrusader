/***********************************************************************\
|																		|
|	File:			SGD_Utilities.h 									|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-12-31											|
|																		|
|	Purpose:		To provide debugging utilities						|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_Utilities.h"
		

// Uses MessageBox & OutputDebugString
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace SGD
{	
	//*****************************************************************//
	// ALERT
	void Alert( const char* message )
	{
		// Print message to Output window
		SGD::Print( message );
		SGD::Print( "\n" );
		
		// Display a message box (using the active window & its title)
		char title[ 128 ];
		::GetWindowTextA( ::GetActiveWindow(), title, 128 ); 
		::MessageBoxA( ::GetActiveWindow(), message, title, MB_OK | MB_ICONEXCLAMATION );
	}

	void Alert( const wchar_t* message )
	{
		// Print message to Output window
		SGD::Print( message );
		SGD::Print( "\n" );

		// Display a message box (using the active window & its title)
		wchar_t title[ 128 ];
		::GetWindowTextW( ::GetActiveWindow(), title, 128 ); 
		::MessageBoxW( ::GetActiveWindow(), message, title, MB_OK | MB_ICONEXCLAMATION );
	}
	//*****************************************************************//



	//*****************************************************************//
	// ASSERT
	void Assert( bool expression, const char* message )
	{
		// Did the expression fail?
		if( expression == false )
		{
			// Display a message box
			SGD::Alert( message );

			// Trigger a breakpoint
			__debugbreak();				// USE THE CALLSTACK TO DEBUG!
		}
	}

	void Assert( bool expression, const wchar_t* message )
	{
		// Did the expression fail?
		if( expression == false )
		{
			// Display a message box
			SGD::Alert( message );

			// Trigger a breakpoint
			__debugbreak();				// USE THE CALLSTACK TO DEBUG!
		}
	}
	//*****************************************************************//
	


	//*****************************************************************//
	// PRINT
	void Print( const char* message )
	{
		// Print message to Output window
		::OutputDebugStringA( message );
	}

	void Print( const wchar_t* message )
	{
		// Print message to Output window
		::OutputDebugStringW( message );
	}
	//*****************************************************************//


}	// namespace SGD
