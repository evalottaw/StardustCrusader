//*********************************************************************//
//	File:		OptionsState.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the Options State functionality
//*********************************************************************//
#pragma once
#include "IGameState.h"
class OptionsState :
	public IGameState
{
public:
	

	static OptionsState* GetInstance(void);
	static void  DeleteInstance(void);

	void Enter(void);
	void Exit(void);

	bool Update(float elapsedTime);
	void Render(float elapsedTime);

	
	void ReadInVolume();


private:

	OptionsState() = default;
	~OptionsState() = default;

	OptionsState(const OptionsState&) = delete;
	OptionsState& operator=(const OptionsState&) = delete;

	
	int m_iCursor = 0;


	static OptionsState* s_pInstance;

	// helper
	void ChangeSfxVolume(int vol);
	void ChangeMusicVolume(int vol);

	void DrawSfxVolume();
	void DrawMusicVolume();
	void DrawSoundBars();

	// writes out volume to txt file
	void OutputVolumeToFile();
};

