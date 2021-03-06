//*********************************************************************//
//	File:		HowToPlayState.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the HowToPlay State functionality
//*********************************************************************//
#pragma once
#include "IGameState.h"



class HowToPlayState :
	public IGameState
{
public:

	// singleton accessor
	static HowToPlayState* GetInstance(void);
	static void  DeleteInstance(void);



	void Enter(void);
	void Exit(void);

	bool Update(float elapsedTime);
	void Render(float elapsedTime);

private:
	static HowToPlayState* s_pInstance;

	HowToPlayState(void) = default;
	~HowToPlayState(void) = default;
	
	HowToPlayState(const HowToPlayState&) = delete;
	HowToPlayState& operator=(const HowToPlayState&) = delete;
};
