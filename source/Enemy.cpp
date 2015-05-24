#include "Enemy.h"
#include "Game.h"
#include "GameplayState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


Enemy::Enemy()
{
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
	if (!GameplayState::GetInstance()->IsGamePaused())
	{
		if (GetPosition().x <= Game::GetInstance()->GetScreenSize().width / 2)
		{
			SetVelocity(SGD::Vector{ 0, 0 });
		}
		else
		{
			SetPosition(SGD::Point{ GetPosition().x - GetVelocity().x, GetPosition().y });
		}
	}
}