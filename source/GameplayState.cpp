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

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;


	Entity* player = CreatePlayer();

	m_pEntities->AddEntity(player, 0);

	
	player->Release();
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
		Game::GetInstance()->ChangeState( MainMenuState::GetInstance() );
		
		// Exit this state immediately
		return true;	// keep playing in the new state
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
SGD::HTexture GameplayState::GetLevelBackground(int _level)
{
	SGD::HTexture temp;
	if (_level == 1)
	{
		temp = m_hLevel1Background;
	}

	return temp;
}
