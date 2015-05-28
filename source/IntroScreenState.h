//*********************************************************************//
//	File:		IntroScreenState.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	handles the intro screen
//*********************************************************************//
#pragma once
#include "IGameState.h"
class IntroScreenState :
	public IGameState
{
public:

	static IntroScreenState* GetInstance(void);
	static void  DeleteInstance(void);

	void Enter(void);
	void Exit(void);

	bool Update(float elapsedTime);
	void Render(float elapsedTime);
private:

	static IntroScreenState* s_pInstance;


	IntroScreenState() = default;
	~IntroScreenState() = default;
	IntroScreenState(const IntroScreenState&) = delete;
	IntroScreenState& operator=(const IntroScreenState&) = delete;


	float opacity = 255.0f;
};

