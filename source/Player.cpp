//*********************************************************************//
//	File:		Player.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the Player entity
//*********************************************************************//
#include "Player.h"
#include "Game.h"
#include "GameplayState.h"
#include "AnchorPointAnimation.h"
#include "MessageID.h"
#include "CreateBulletMessage.h"
#include "Enemy.h"


#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"



#define SECONDARY_SHOT_DELAY 0.35


Player::Player()
{
	SetImage(Game::GetInstance()->GetPlayerImg());
	RegisterForEvent("ENEMY_DESTROYED");
	RegisterForEvent("GAME_OVER");
}


Player::~Player()
{
}

void Player::Update(float _elapsedTime)
{
	m_fShotCooldown += _elapsedTime;
	

	if (!GameplayState::GetInstance()->IsGamePaused()
		&& !Game::GetInstance()->IsGameWon()
		&& !Game::GetInstance()->IsGameLost())
	{
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::D))
		{
			m_ptPosition.x = GetPosition().x + GetVelocity().x * _elapsedTime * 60;
			SetDirection(RIGHT);
		}
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::A))
		{
			m_ptPosition.x = GetPosition().x - GetVelocity().x * _elapsedTime * 60;
			SetDirection(LEFT);
		}
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::W))
			m_ptPosition.y = GetPosition().y - GetVelocity().y * _elapsedTime * 60;
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::S))
			m_ptPosition.y = GetPosition().y + GetVelocity().y * _elapsedTime * 60;

		if ((SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Spacebar)
			&& m_fShotCooldown > SECONDARY_SHOT_DELAY) 
			|| (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Spacebar)
			&& m_fShotCooldown > SECONDARY_SHOT_DELAY))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(GetSecondarySfx(), false);
			m_fShotCooldown = 0.0f;
			CreateBulletMessage* message = new CreateBulletMessage(this);
			message->QueueMessage();
			
		}

		

		
	}

	PlayerInBounds();
}

void Player::Render(void)
{

	if (GetDirection() == RIGHT)
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(GetImage(),
		m_ptPosition, SGD::Rectangle{ 0, 0, 32, 32 }, {}, {}, {}, SGD::Size{ 2.5f, 2.5f }); 
	else
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(GetImage(),
		SGD::Point{ m_ptPosition.x + GetSize().width * 2.5f, m_ptPosition.y }, SGD::Rectangle{ 0, 0, 32, 32 }, {}, {}, {}, SGD::Size{ -2.5f, 2.5f });

	
}

void Player::PlayerInBounds(void)
{
	if (GetPosition().x <= 0)
		m_ptPosition.x = 0;
	if (GetPosition().x > Game::GetInstance()->GetScreenSize().width - GetSize().width * 2.5f)
		m_ptPosition.x = Game::GetInstance()->GetScreenSize().width - GetSize().width * 2.5f;
	if (GetPosition().y <= 0)
		m_ptPosition.y = 0;
	if (GetPosition().y > Game::GetInstance()->GetScreenSize().height - GetSize().height * 2.5f)
		m_ptPosition.y = Game::GetInstance()->GetScreenSize().height - GetSize().height * 2.5f;
}

void Player::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ENEMY_DESTROYED")
	{
		SetScore(GetScore() + 50);
	}
	else if (pEvent->GetEventID() == "GAME_OVER")
	{
		SetAlive(false);
		GameplayState::GetInstance()->SetGameLost(true);
	}
	
}

