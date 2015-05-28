//*********************************************************************//
//	File:		main.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Application entry point
//*********************************************************************//

#include <vld.h>			// Visual Leak Detector
#include "Game.h"			// Game singleton class


//*********************************************************************//
// main
//	- application entry point
int main( void )
{
	// Initialize game:
	if( Game::GetInstance()->Initialize( ) == false )
		return -1;	// failure!!!

	
	// Run game:
	int exitCode = 0;
	do
		exitCode = Game::GetInstance()->Update();
	while( exitCode == 0 );

	
	// Terminate game:
	Game::GetInstance()->Terminate();
	Game::DeleteInstance();


	// Exit (+success, -failure)
	return exitCode;
}
