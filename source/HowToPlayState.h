#pragma once
#include "IGameState.h"


class HowToPlayState :
	public IGameState
{
public:

	// singleton accessor
	static HowToPlayState* GetInstance(void);

	HowToPlayState();
	~HowToPlayState();

	void Enter(void);
	void Exit(void);

	bool Update(float elapsedTime);
	void Render(float elapsedTime);

private:

	

};

