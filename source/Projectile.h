//*********************************************************************//
//	File:		Projectile.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the Projectile entity
//*********************************************************************//
#pragma once
#include "Entity.h"
class Projectile :
	public Entity
{
public:
	Projectile();
	~Projectile();

	int GetType() const { return ENT_PROJECTILE; }

	void Render(void);
	void Update(float elapsedTime);

	Entity* GetProjectileOwner() const { return m_etProjectileOwner; }

	void SetProjectileOwner(Entity* _owner);

	void HandleCollision(const IEntity* pOther);

private:

	Entity* m_etProjectileOwner = nullptr;
};

