#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_IListener.h"
class Enemy :
	public Entity, public SGD::IListener
{
public:
	Enemy();
	~Enemy();

	int GetType(void) const { return ENT_ENEMY; }

	void Render(void);
	void Update(float elapsedTime);

	bool DidEnemyCrossLine();

	int GetNumHitsTaken() const { return m_iNumHitsTaken; }
	SGD::HAudio GetEnemyHitSfx() const { return m_hEnemyHitSfx; }

	void SetNumHitsTaken(int _hits) { m_iNumHitsTaken = _hits; }
	void SetEnemyHitSfx(SGD::HAudio _sfx) { m_hEnemyHitSfx = _sfx; }

	void HandleEvent(const SGD::Event* pEvent);

	void KeepEnemyInBounds();
private:

	int m_iNumHitsTaken = 0;

	SGD::HAudio m_hEnemyHitSfx = SGD::INVALID_HANDLE;
};

