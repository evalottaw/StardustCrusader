#pragma once
#include "IGameState.h"
class OptionsState :
	public IGameState
{
public:
	OptionsState();
	~OptionsState();

	static OptionsState* GetInstance(void);
	static void  DeleteInstance(void);

	void Enter(void);
	void Exit(void);

	bool Update(float elapsedTime);
	void Render(float elapsedTime);

	// access
	int GetMusicVolume() const { return m_uiMusicVol; }
	int GetSfxVolume() const { return m_uiSfxVol; }

	// mutate
	void SetMusicVolume(int _vol) { m_uiMusicVol = _vol; }
	void SetSfxVolume(int _vol) { m_uiSfxVol = _vol; }


private:

	int m_uiMusicVol = 25, m_uiSfxVol = 25;
	int m_iCursor = 0;
};

