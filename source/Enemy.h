#pragma once
#include "Entity.h"
class Enemy :
	public Entity
{
public:
	Enemy();
	~Enemy();

	int GetType(void) const { return ENT_ENEMY; }

	void Render(void);
	void Update(float elapsedTime);

};

