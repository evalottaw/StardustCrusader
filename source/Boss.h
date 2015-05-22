#pragma once
#include "Entity.h"
class Boss :
	public Entity
{
public:
	Boss();
	~Boss();

	int GetType() const { return ENT_ENEMY_BOSS; }

	void Render(void);
	void Update(float elapsedTime);

	
};

