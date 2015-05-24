#pragma once
#include "Entity.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Player :
	public Entity
{

	enum Direction
	{
		LEFT, RIGHT
	};

public:
	Player();
	~Player();

	void Update(float _elapsedTime);
	void Render(void);

	int GetType(void) const { return ENT_PLAYER; }

	void SwitchWeapons();

	// accessors
	Direction GetDirection() const { return m_dPlayerDir; }
	SGD::HAudio GetShotBazookaSfx() const { return m_hShotBazookaSfx; }
	SGD::HAudio GetShotLaserGunSfx() const { return m_hShotLaserGunSfx; }
	SGD::HAudio GetHurtSfx() const { return m_hHurtSfx; }
	SGD::HAudio GetMeleeSfx() const { return m_hMeleeSfx; }
	bool GetAlive() const { return m_bALive; }
	bool GetVictory() const { return m_bVictory; }
	float GetShotCooldown() const { return m_fShotCooldown; }
	unsigned int GetScore() const { return m_uiPlayerScore; }
	int GetNumLives() const { return m_uiLives; }
	int GetPlayerHP() const { return m_uiPlayerHP; }

	// mutators
	void SetDirection(Direction _dir) { m_dPlayerDir = _dir; }
	void SetAlive(bool _alive) { m_bALive = _alive; }
	void SetVictory(bool _victory) { m_bVictory = _victory; }
	void SetShotCooldown(float _cooldown) { m_fShotCooldown = _cooldown; }
	void SetScore(unsigned int _score) { m_uiPlayerScore = _score; }
	void SetNumLives(int _lives) { m_uiLives = _lives; }
	void SetPlayerHP(int _hp) { m_uiPlayerHP = _hp; }

	// helper functions
	void PlayerInBounds(void);

private:
	
	Direction m_dPlayerDir = RIGHT;

	SGD::Rectangle m_rRenderWithWeapon = SGD::Rectangle{ 0, 0, 0, 0 };

	SGD::HAudio m_hHurtSfx, m_hShotBazookaSfx, m_hShotLaserGunSfx, m_hMeleeSfx;

	bool m_bALive = true, m_bVictory = false;

	float m_fShotCooldown;
	unsigned int m_uiPlayerScore = 0;
	int m_uiLives = 3, m_uiPlayerHP = 100;
};

