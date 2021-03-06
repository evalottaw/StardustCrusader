//*********************************************************************//
//	File:		CreditsState.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the Credits State
//*********************************************************************//
#pragma once
#include "IGameState.h"
class CreditsState :
	public IGameState
{
public:


	static CreditsState* GetInstance(void);
	static void  DeleteInstance(void);

	void Enter(void);
	void Exit(void);

	bool Update(float elapsedTime);
	void Render(float elapsedTime);

private:
	static CreditsState* s_pInstance;

	float m_fScrollCreditsAm = 0;
	float m_fWait = 8.0f;

	CreditsState(void) = default;
	~CreditsState(void) = default;

	CreditsState(const CreditsState&) = delete;
	CreditsState& operator=(const CreditsState&) = delete;
};

