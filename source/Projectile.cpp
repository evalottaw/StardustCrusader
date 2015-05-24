#include "Projectile.h"


Projectile::Projectile()
{
}


Projectile::~Projectile()
{
}

void Projectile::Render(void)
{

}

void Projectile::Update(float elapsedTime)
{
	
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

	}

}