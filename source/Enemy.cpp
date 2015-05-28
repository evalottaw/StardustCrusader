//*********************************************************************//
//	File:		Enemy.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the Enemy entity
//*********************************************************************//
#include "Enemy.h"
#include "Game.h"
#include "GameplayState.h"
#include "Player.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"


Enemy::Enemy()
{
	RegisterForEvent("ENEMY_HIT");
	RegisterForEvent("ENEMY_DESTROYED");
}


Enemy::~Enemy()
{
}

void Enemy::Render(void)
{
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(GetImage(), 
		SGD::Point{GetPosition()},
		SGD::Rectangle{ 0, 0, 64, 80 }, 0.0f, {}, {}, SGD::Size{ -2.0f, 2.0f });
}

void Enemy::Update(float elapsedTime)
{
	if (!GameplayState::GetInstance()->IsGamePaused()
		&& !GameplayState::GetInstance()->IsGameLost())
	{
		
			SetPosition(SGD::Point{ GetPosition().x - GetVelocity().x * elapsedTime * 60, GetPosition().y });
			if (GetPosition().x <= 0 + GetSize().width * 1.5f)
			{
				m_ptPosition.x = GetSize().width * 1.5f; 
				SGD::Event* Event = new SGD::Event("GAME_OVER", nullptr, this);
				SGD::EventManager::GetInstance()->QueueEvent(Event);
			}

			if (GetNumHitsTaken() >= 3)
			{
 				SGD::Event* Event = new SGD::Event("ENEMY_DESTROYED", nullptr, this);
				SGD::EventManager::GetInstance()->QueueEvent(Event);
				DestroyEntityMessage* msg = new DestroyEntityMessage(this);
				msg->QueueMessage();
				Game::GetInstance()->SetNumEnemies(Game::GetInstance()->GetNumEnemies() - 1);
				if (Game::GetInstance()->GetNumEnemies() <= 0)
					Game::GetInstance()->SetVictory(true);
				SetNumHitsTaken(0);
			}

			

			KeepEnemyInBounds();
	}
}

void Enemy::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ENEMY_HIT")
	{
		if (GameplayState::GetInstance()->DoubleDmg())
		{
			SetNumHitsTaken(GetNumHitsTaken() + 2);
			GameplayState::GetInstance()->SetDoubleDmg(false);
		}
			
		else
			SetNumHitsTaken(GetNumHitsTaken() + 1);
		SGD::AudioManager::GetInstance()->PlayAudio(GetEnemyHitSfx());
	}
	
}


void Enemy::KeepEnemyInBounds()
{
	if (GetPosition().x <= 0)
		m_ptPosition.x = 0;
	if (GetPosition().x > Game::GetInstance()->GetScreenSize().width - GetSize().width * 2.0f)
		m_ptPosition.x = Game::GetInstance()->GetScreenSize().width - GetSize().width * 2.0f;
	if (GetPosition().y <= 0)
		m_ptPosition.y = 0;
	if (GetPosition().y > Game::GetInstance()->GetScreenSize().height - GetSize().height * 2.0f)
		m_ptPosition.y = Game::GetInstance()->GetScreenSize().height - GetSize().height * 2.0f;
}