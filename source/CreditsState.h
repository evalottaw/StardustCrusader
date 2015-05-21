#pragma once
#include "IGameState.h"
class CreditsState :
	public IGameState
{
public:
	CreditsState();
	~CreditsState();

	static CreditsState* GetInstance(void);

	void Enter(void);
	void Exit(void);

	bool Update(float elapsedTime);
	void Render(float elapsedTime);
};

