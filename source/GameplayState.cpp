//*********************************************************************//
//	File:		GameplayState.cpp
//	Author:		
//	Course:		
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#include "GameplayState.h"

#include "Game.h"
#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Utilities.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "BitmapFont.h"

#include "EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <cassert>


//*********************************************************************//
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ GameplayState* GameplayState::GetInstance( void )
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- reset game
//	- load resources / assets
//	- allocate & initialize game entities
/*virtual*/ void GameplayState::Enter( void )	/*override*/
{
	// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize( &GameplayState::MessageProc );

	m_hLevel1Background = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/ELW_LevelCut.png");
	m_EnemyImgL1 = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/ELW_EnemyLvl1.png", SGD::Color{ 255, 255, 255 });

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;


	Entity* player = CreatePlayer();
	Entity* enemy = CreateLvl1Enemy();
	

	m_pEntities->AddEntity(player, 0);
	m_pEntities->AddEntity(enemy, 1);

	
	player->Release();
	enemy->Release();
}


//*********************************************************************//
// Exit
//	- deallocate entities
//	- unload resources / assets
/*virtual*/ void GameplayState::Exit( void )	/*override*/
{




	// Release game entities
	if( m_pEntities != nullptr )
	{
		m_pEntities->RemoveAll();
		delete m_pEntities;
		m_pEntities = nullptr;
	}
	
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hLevel1Background);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_EnemyImgL1);



	// Terminate & deallocate the SGD wrappers
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();

	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();
}


//*********************************************************************//
// Update
//	- handle input & update entities
/*virtual*/ bool GameplayState::Update( float elapsedTime )	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	
	// Press Escape to return to MainMenuState
	if( pInput->IsKeyPressed( SGD::Key::Escape ) == true )
	{
		// ChangeState is VERY VOLATILE!!!
		//	- can only be safely called by a game state's
		//	  Update or Render methods!
		m_iCursor = 0;

		m_bisGamePaused = !m_bisGamePaused;
		m_bisKeyPressed = true;  
		
		// Exit this state immediately
		//return true;	// keep playing in the new state
	}
	// pause menu shows up 
	if (m_bisGamePaused)
	{
		if (Pause())
		{
			return true;
		}
	}
		
	
	
	
	// Update the entities
	m_pEntities->UpdateAll( elapsedTime );
	

	
	// Process the Event Manager
	//	- all the events will be sent to the registered IListeners' HandleEvent methods
	SGD::EventManager::GetInstance()->Update();

	// Process the Message Manager
	//	- all the messages will be sent to our MessageProc
	SGD::MessageManager::GetInstance()->Update();

	return true;	// keep playing
}


//*********************************************************************//
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render( float elapsedTime )	/*override*/
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hLevel1Background, SGD::Point{ 0, 0 });

	BitmapFont* font = Game::GetInstance()->GetFont();

	font->Draw("Level", SGD::Point{ 2, 2 }, 1.0f, SGD::Color{ 255, 0, 0, 255 });
	font->Draw("1", SGD::Point{ 170, 2 }, 1.0f, SGD::Color{ 255, 0, 0, 255 });
	font->Draw("Score", SGD::Point{ 250, 2 }, 1.0f, SGD::Color{ 255, 0, 0, 255 });

	

	// Render the entities
	m_pEntities->RenderAll();
	// draws the pause menu
	if (m_bisGamePaused)
	{
		font->Draw("Pause", SGD::Point{ 300, 250 }, 1.5f);
		font->Draw("Resume", SGD::Point{ 300, 400 }, 1.0f);
		font->Draw("Quit", SGD::Point{ 300, 450 }, 1.0f);
		font->Draw("0", SGD::Point{ 250, 400.0f + 50.0f * m_iCursor }, 1.0f);
	}

	

}


//*********************************************************************//
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD!!!
//		- no invoking object!
//		- MUST USE THE SINGLETON
/*static*/ void GameplayState::MessageProc( const SGD::Message* pMsg )
{
/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )


	// What type of message?
	switch( pMsg->GetMessageID() )
	{




	default:
	case MessageID::MSG_UNKNOWN:
		SGD_PRINT( L"GameplayState::MessageProc - unknown message id\n" );
		break;
	}


/* Restore previous warning levels */
#pragma warning( pop )
}

Entity* GameplayState::CreatePlayer(void)
{
	Player* player = new Player;
	player->SetAlive(true);
	player->SetImage(Game::GetInstance()->GetPlayerImg());
	player->SetVictory(false);
	player->SetPosition(SGD::Point{ 0, Game::GetInstance()->GetScreenSize().height - 105});
	player->SetSize(SGD::Size{ 32, 32 });
	player->SetVelocity(SGD::Vector{ 0.7f, 0 });
	

	return player;
}

Entity* GameplayState::CreateLvl1Enemy(void)
{
	Enemy* enemy = new Enemy;
	enemy->SetImage(m_EnemyImgL1);
	enemy->SetSize(SGD::Size{ 64, 80 });
	enemy->SetVelocity(SGD::Vector{ 0.2f, 0 });
	enemy->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width,
		Game::GetInstance()->GetScreenSize().height - 180 });
	return enemy;
}
SGD::HTexture GameplayState::GetLevelBackground(int _level)
{
	SGD::HTexture temp;
	if (_level == 1)
	{
		temp = m_hLevel1Background;
	}

	return temp;
}

bool GameplayState::Pause(void)
{
	// changes menu alternatives
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Down))
	{
		++m_iCursor;
		if (m_iCursor > 1)
			m_iCursor = 0;
	}

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Up))
	{
		--m_iCursor;
		if (m_iCursor < 0)
			m_iCursor = 1;
	}

	// not working! should resume game =(
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape)
		&& !m_bisKeyPressed)
	{
		m_bisGamePaused = false;
		m_bisKeyPressed = true;
		return true;
	}
	
	m_bisKeyPressed = false;

	
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter))
	{
		 
		if (m_iCursor == 0)// resumes game
		{
			m_bisGamePaused = false;
			return false;
		}
		else if (m_iCursor == 1)// quits to main menu
		{
			m_bisGamePaused = false;
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}

	}

	return false;
}
