#include "Projectile.h"
#include "DestroyEntityMessage.h"
#include "GameplayState.h"
#include "Game.h"
#include "Player.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"



Projectile::Projectile()
{
}


Projectile::~Projectile()
{
	m_etProjectileOwner->Release();
}

void Projectile::Render(void)
{
	SGD::Point projPoint = SGD::Point{ GetPosition().x, GetPosition().y };
	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), projPoint);
}

void Projectile::Update(float elapsedTime)
{

	if (GetPosition().x < 0
		|| GetPosition().x >= Game::GetInstance()->GetScreenSize().width
		|| GetPosition().y < 0
		|| GetPosition().y >= Game::GetInstance()->GetScreenSize().height)
	{
		DestroyEntityMessage* destroyMsg = new DestroyEntityMessage(this);
		destroyMsg->QueueMessage();
	}

	Entity::Update(elapsedTime);
}

void Projectile::SetProjectileOwner(Entity* _owner)
{
	if (m_etProjectileOwner != nullptr)
		m_etProjectileOwner->Release();
	if (_owner != nullptr)
		m_etProjectileOwner = _owner;
	_owner->AddRef();
}

void Projectile::HandleCollision(const IEntity* pOther)
{
	if (GetProjectileOwner() != pOther)
	{
		SGD::Event* Event = new SGD::Event("ENEMY_HIT", nullptr, this);
		SGD::EventManager::GetInstance()->QueueEvent(Event, pOther);
		DestroyEntityMessage* destroyMsg = new DestroyEntityMessage(this);
		destroyMsg->QueueMessage();

		/*if (GetProjectileOwner()->GetType() == ENT_ENEMY)
		{
			SGD::Event* Event = new SGD::Event("PLAYER_HIT", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(Event, pOther);
			DestroyEntityMessage* destroyMsg = new DestroyEntityMessage(this);
			destroyMsg->QueueMessage();
		}
		else if (GetProjectileOwner()->GetType() == ENT_PLAYER)
		{
			SGD::Event* Event = new SGD::Event("ENEMY_HIT", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(Event, pOther);
			DestroyEntityMessage* destroyMsg = new DestroyEntityMessage(this);
			destroyMsg->QueueMessage();
		}*/
	}

}