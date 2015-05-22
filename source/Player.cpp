#include "Player.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "Game.h"
#include "GameplayState.h"

#define SHOT_TIMER 0.75

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