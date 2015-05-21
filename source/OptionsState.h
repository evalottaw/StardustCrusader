#pragma once
#include "IGameState.h"
class OptionsState :
	public IGameState
{
public:
	OptionsState();
	~OptionsState();

	static OptionsState* GetInstance(void);

	void Enter(void);
	void Exit(void);

	bool Update(float elapsedTime);
	void Render(float elapsedTime);
};

