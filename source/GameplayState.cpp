//*********************************************************************//
//	File:		GameplayState.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#include "GameplayState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "MessageID.h"
#include "BitmapFont.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Projectile.h"
#include "CreateBulletMessage.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <string.h>

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
	// loads textures
	m_hLevel1Background = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/ELW_LevelCut.png");
	m_hEnemyImgL1 = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/ELW_EnemyLvl1.png", SGD::Color{ 255, 255, 255 });
	m_hProjectileSecImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/ELW_ProjectileSec.png", SGD::Color{ 255, 255, 255 });

	// loads sfx + background music
	m_hProjectileSecSfx = SGD::AudioManager::GetInstance()->LoadAudio(L"./resource/audio/ELW_SecondaryShotSfx.wav");
	m_hBackgroundMus = SGD::AudioManager::GetInstance()->LoadAudio(L"./resource/audio/ELW_BackgroundMusic.xwm");
	m_hEnemyHitSfx = SGD::AudioManager::GetInstance()->LoadAudio(L"./resource/audio/ELW_EnemyHitSfx.wav");
	// plays background music looping
	SGD::AudioManager::GetInstance()->PlayAudio(m_hBackgroundMus, playBackgMus);

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;

	
	m_pPlayer = CreatePlayer();
	m_pEntities->AddEntity(m_pPlayer, 0);
	
	// made switch function for future levels
	HoldEnemyCreation(1);

}


//*********************************************************************//
// Exit
//	- deallocate entities
//	- unload resources / assets
/*virtual*/ void GameplayState::Exit( void )	/*override*/
{

	m_pPlayer->Release();


	// Release game entities
	if( m_pEntities != nullptr )
	{
		m_pEntities->RemoveAll();
		delete m_pEntities;
		m_pEntities = nullptr;
	}
	
	// unloads textures
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hLevel1Background);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hEnemyImgL1);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hProjectileSecImage);
	// unloads audio
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hProjectileSecSfx);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hBackgroundMus);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hEnemyHitSfx);

	



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
	m_pEntities->CheckCollisions(0, 1);

	
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

	font->Draw("Kill all enemies to advance", SGD::Point{ 2, 2 }, 0.7f, SGD::Color{ 255, 0, 0, 255 });
	DrawPlayerScore();
	

	// Render the entities
	m_pEntities->RenderAll();
	// draws the pause menu
	if (m_bisGamePaused)
	{
		font->Draw("Pause", SGD::Point{ 400, 250 }, 1.5f);
		font->Draw("Resume", SGD::Point{ 400, 400 }, 1.0f);
		font->Draw("Quit", SGD::Point{ 400, 450 }, 1.0f);
		font->Draw("0", SGD::Point{ 350, 400.0f + 50.0f * m_iCursor }, 1.0f);
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
	case MessageID::MSG_CREATE_BULLET:
		{
			const CreateBulletMessage* message = dynamic_cast<const CreateBulletMessage*>(pMsg);
			Entity* enemy = message->GetBulletOwner();
			Entity* entity = GameplayState::GetInstance()->CreateProjectile(enemy);
			GameplayState::GetInstance()->m_pEntities->AddEntity(entity, 1);
			entity->Release(); GameplayState::GetInstance()->m_pEntities->AddEntity(entity, 2);
		}
		break;
	case MessageID::MSG_DESTROY_ENTITY:
		{
		const DestroyEntityMessage* destroy = dynamic_cast<const DestroyEntityMessage*>(pMsg);
		Entity* entity = destroy->GetEntity();
		GameplayState::GetInstance()->m_pEntities->RemoveEntity(entity);
		}
		break;
	default:
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
	player->SetVelocity(SGD::Vector{ 0.4f, 0.4f });
	player->SetSecondarySfx(m_hProjectileSecSfx);
	player->SetScore(0);
	player->SetNumLives(1);
	
	

	return player;
}

Entity* GameplayState::CreateLvl1Enemy(void)
{
	Enemy* enemy = new Enemy;
	enemy->SetImage(m_hEnemyImgL1);
	enemy->SetSize(SGD::Size{ 64, 80 });
	enemy->SetVelocity(SGD::Vector{ 0.1f, 0 });
	enemy->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width, 
		(float)(rand() % (int)(Game::GetInstance()->GetScreenSize().height)) });
	enemy->SetNumHitsTaken(0);
	enemy->SetEnemyHitSfx(m_hEnemyHitSfx);
	return enemy;
}

Entity* GameplayState::CreateProjectile(Entity* entity)
{
	Projectile* projectile = new Projectile;
	projectile->SetImage(m_hProjectileSecImage);
	projectile->SetProjectileOwner(entity);
	projectile->SetSize(SGD::Size{ 32, 32 });
	projectile->SetPosition(SGD::Point{ entity->GetPosition().x + entity->GetSize().width * 2.5f, entity->GetPosition().y + entity->GetSize().height / 2});
	projectile->SetRotation(entity->GetRotation());

	SGD::Vector vel = SGD::Vector{ 1, 0 };
	if (entity->GetSpeed() > 0)
		vel *= 250 + entity->GetSpeed();
	else
		vel *= 250;

	vel.Rotate(projectile->GetRotation());
	projectile->SetVelocity(vel);
	return projectile;

	
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

	// resumes game when exit is pressed second time
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
			// stops the music from playing in the menu screen
			SGD::AudioManager::GetInstance()->StopAudio(m_hBackgroundMus);
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

			return true;
		}

	}

	return false;
}

void GameplayState::DrawPlayerScore(void)
{
	

	BitmapFont* font = Game::GetInstance()->GetFont();
	std::string tempString = std::to_string(dynamic_cast<Player*>(m_pPlayer)->GetScore());
	const char* ch = tempString.c_str();
	font->Draw("Score: ", SGD::Point{ 370, 35 }, 0.8f);
	font->Draw(ch, SGD::Point{ 530, 35 }, 0.8f);


}

void GameplayState::HoldEnemyCreation(int _level)
{
	switch (_level)
	{
	case 1:
	{
		for (size_t i = 0; i < 6; i++)
		{
			Entity* enemy = CreateLvl1Enemy();
			m_pEntities->AddEntity(enemy, 0);
			enemy->Release();
		}
	
	}
	default:
		break;
	}
	
}