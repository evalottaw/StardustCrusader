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
		
			SetPosition(SGD::Point{ GetPosition().x - GetVelocity().x, GetPosition().y });
			if (GetPosition().x <= 0 + GetSize().width)
			{
				m_ptPosition.x = GetSize().width * 2.0f; 
				// IF IT GETS HERE THE GAME IS OVER
				// FIND A GOOD WAY TO MAKE IT PAUSE AND DISPLAY TEXT + AUDIO
				GameplayState::GetInstance()->SetGameLost(true);
			}

			if (GetNumHitsTaken() >= 3)
			{
 				SGD::Event* Event = new SGD::Event("ENEMY_DESTROYED", nullptr, this);
				SGD::EventManager::GetInstance()->QueueEvent(Event);
				DestroyEntityMessage* msg = new DestroyEntityMessage(this);
				msg->QueueMessage();
				SetNumHitsTaken(0);
			}

			KeepEnemyInBounds();
	}
}

void Enemy::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ENEMY_HIT")
	{
		SetNumHitsTaken(GetNumHitsTaken() + 1);
		SGD::AudioManager::GetInstance()->PlayAudio(GetEnemyHitSfx());
	}
	
}

void Enemy::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		SGD::Event* Event = new SGD::Event("GAME_OVER", nullptr, this);
		SGD::EventManager::GetInstance()->QueueEvent(Event);
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