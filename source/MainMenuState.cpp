//*********************************************************************//
//	File:		MainMenuState.cpp
//	Author:		
//	Course:		
//	Purpose:	MainMenuState class handles the main menu
//*********************************************************************//

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "HowToPlayState.h"
#include "OptionsState.h"
#include "CreditsState.h"

MainMenuState* MainMenuState::s_pInstance = nullptr;

//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ MainMenuState* MainMenuState::GetInstance( void )
{
	
	if (s_pInstance == nullptr)
		s_pInstance = new MainMenuState;
	return s_pInstance;
}

void MainMenuState::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}
//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void MainMenuState::Enter( void )		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	//m_nCursor = 0;
	Game::GetInstance()->SetGameLost(false);
	Game::GetInstance()->SetVictory(false);
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void MainMenuState::Exit( void )		/*override*/
{
	MainMenuState::GetInstance()->DeleteInstance();

}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool MainMenuState::Update( float elapsedTime )	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
		m_nCursor = 4;


	// Move the cursor?
	if( pInput->IsKeyPressed( SGD::Key::Down ) == true )
	{
		// next option
		m_nCursor++;

		// wrap around
		if( m_nCursor > 4 )
			m_nCursor = 0;
	}
	else if( pInput->IsKeyPressed( SGD::Key::Up ) == true ) 
	{
		// prev option
		m_nCursor--;

		// wrap around
		if( m_nCursor < 0 )
			m_nCursor = 4;
	}



	// Select an option?
	if( pInput->IsKeyPressed( SGD::Key::Enter ) == true
		&& pInput->IsKeyDown(SGD::Key::Alt) == false)
	{
		// Which option is chosen?
		if( m_nCursor == 0 )
		{
			// ChangeState is VERY VOLATILE!!!
			//	- can only be safely called by a game state's
			//	 Update or Render methods!
			Game::GetInstance()->ChangeState( GameplayState::GetInstance() );

			// Exit this state immediately
			return true;	// keep playing in the new state
		}
		else if (m_nCursor == 1)
		{
			Game::GetInstance()->ChangeState(OptionsState::GetInstance());
			return true;
		}
		else if (m_nCursor == 2)
		{
			Game::GetInstance()->ChangeState(HowToPlayState::GetInstance());
			return true;
		}
		else if (m_nCursor == 3)
		{
			Game::GetInstance()->ChangeState(CreditsState::GetInstance());
			return true;
		}
		else //m_nCursor == 4
		{
			return false;	// quit the game
		}
	}

	
	return true;	// keep playing
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void MainMenuState::Render( float elapsedTime )		/*override*/
{

	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetMenuBackground(), SGD::Point{ 0, 0 });


	// Display the game title centered at 4x scale
	pFont->Draw( "STARDUST", { (width - (4 * 32 * 4.0f))/2, 50 }, 
				 2.0f, { 255, 255, 255 } );
	pFont->Draw("CRUSADER", { (width - (4 * 32 * 4.0f)) / 2, 130 },
		2.0f, { 255, 255, 255 });

	// Display the menu options centered at 1x scale
	pFont->Draw( "Play", { (width - (4 * 32))/2 - 50, 300 }, 
		1.0f, { 255, 255, 0 });
	pFont->Draw( "Options", { (width - (4 * 32))/2 - 50, 350 }, 
		1.0f, { 255, 255, 0 });
	pFont->Draw("How To Play", { (width - (4 * 32)) / 2 - 50, 400 },
		1.0f, { 255, 255, 0 });
	pFont->Draw("Credits", { (width - (4 * 32)) / 2 - 50, 450 },
		1.0f, { 255, 255, 0 });
	pFont->Draw("Exit", { (width - (4 * 32)) / 2 - 50, 500 },
		1.0f, { 255, 255, 0 });
	// Display the cursor next to the option
	pFont->Draw("0", { (width - (7 * 32)) / 2 - 50, 300.0f + 50 * m_nCursor },
		1.0f, { 255, 255, 255 });
}
