//*********************************************************************//
//	File:		Player.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the Player entity
//*********************************************************************//
#pragma once
#include "Entity.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_IListener.h"

class Player :
	public Entity, public SGD::IListener
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
	void HandleEvent(const SGD::Event* pEvent);
	// accessors
	Direction GetDirection() const { return m_dPlayerDir; }
	SGD::HAudio GetShotBazookaSfx() const { return m_hShotBazookaSfx; }
	SGD::HAudio GetSecondarySfx() const { return m_hShotSecSfx; }
	SGD::HAudio GetHurtSfx() const { return m_hHurtSfx; }
	SGD::HAudio GetMeleeSfx() const { return m_hMeleeSfx; }
	bool GetAlive() const { return m_bALive; }
	bool GetVictory() const { return m_bVictory; }
	float GetShotCooldown() const { return m_fShotCooldown; }
	int GetScore() const { return m_uiPlayerScore; }
	int GetNumLives() const { return m_uiPlayerLives; }
	//int GetPlayerHP() const { return m_uiPlayerHP; }

	// mutators
	void SetDirection(Direction _dir) { m_dPlayerDir = _dir; }
	void SetAlive(bool _alive) { m_bALive = _alive; }
	void SetVictory(bool _victory) { m_bVictory = _victory; }
	void SetShotCooldown(float _cooldown) { m_fShotCooldown = _cooldown; }
	void SetScore(int _score) { m_uiPlayerScore = _score; }
	void SetNumLives(int _lives) { m_uiPlayerLives = _lives; }
//	void SetPlayerHP(int _hp) { m_uiPlayerHP = _hp; }
	void SetSecondarySfx(SGD::HAudio _sfx) { m_hShotSecSfx = _sfx; }

	// helper functions
	void PlayerInBounds(void);


private:
	
	Direction m_dPlayerDir = RIGHT;

	SGD::Rectangle m_rRenderWithWeapon = SGD::Rectangle{ 0, 0, 0, 0 };

	SGD::HAudio m_hHurtSfx, m_hShotBazookaSfx, m_hShotSecSfx, m_hMeleeSfx;

	bool m_bALive = true, m_bVictory = false;

	float m_fShotCooldown = 0.0f;
	int m_uiPlayerScore = 0;
	unsigned int m_uiPlayerLives = 1;
	float m_fincreaseCharge = 0.0f;

};

