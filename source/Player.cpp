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


#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"



#define PRIMARY_SHOT_DELAY 1.5
#define SECONDARY_SHOT_DELAY 0.35
#define MELEE_DELAY 1.0


Player::Player()
{
	SetImage(Game::GetInstance()->GetPlayerImg());
}


Player::~Player()
{
}

void Player::Update(float _elapsedTime)
{
	m_fShotCooldown += _elapsedTime;
	

	if (!GameplayState::GetInstance()->IsGamePaused())
	{
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::D))
		{
			m_ptPosition.x = GetPosition().x + GetVelocity().x;
			SetDirection(RIGHT);
		}
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::A))
		{
			m_ptPosition.x = GetPosition().x - GetVelocity().x;
			SetDirection(LEFT);
		}
		if ((SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::MouseLeft)
			&& m_fShotCooldown > SECONDARY_SHOT_DELAY) 
			|| (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::MouseLeft)
			&& m_fShotCooldown > SECONDARY_SHOT_DELAY))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(GetSecondarySfx(), false);
			m_fShotCooldown = 0.0f;
			CreateBulletMessage* message = new CreateBulletMessage(this);
			message->QueueMessage();
			
		}

		if (GetNumLives() <= 0)
		{
			SetAlive(false);
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
	if (GetPosition().y > Game::GetInstance()->GetScreenSize().height * 2.5f)
		m_ptPosition.y = Game::GetInstance()->GetScreenSize().height * 2.5f;
}

void Player::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "PLAYER_HIT")
	{
		SetNumLives(GetNumLives() - 1);

	}

	if (GetNumLives() <= 0)
	{
		SetAlive(false);
		SetSpeed(0.0f);
		SGD::Event* Event = new SGD::Event("PLAYER_DEAD", nullptr, this);
		SGD::EventManager::GetInstance()->QueueEvent(Event);
	}
}
