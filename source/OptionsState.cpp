//*********************************************************************//
//	File:		OptionsState.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the Options State functionality
//*********************************************************************//
#include "OptionsState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include <string>
#include <iostream>
#include <fstream>


OptionsState* OptionsState::s_pInstance = nullptr;


OptionsState* OptionsState::GetInstance(void)
{
	if (s_pInstance == nullptr)
		s_pInstance = new OptionsState;

	return s_pInstance;
}

void  OptionsState::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

void OptionsState::Enter(void)
{
	SGD::AudioManager::GetInstance()->PlayAudio(Game::GetInstance()->GetBackgroundMus(), true);
}

void OptionsState::Exit(void)
{
	OutputVolumeToFile();
	OptionsState::GetInstance()->DeleteInstance();

}

bool OptionsState::Update(float elapsedTime)
{
	
	// esc puts cursor at Exit
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(Game::GetInstance()->GetMenuChangeSfx());
		m_iCursor = 2;
	}
		

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Down))
	{
		++m_iCursor;
		SGD::AudioManager::GetInstance()->PlayAudio(Game::GetInstance()->GetMenuChangeSfx());

		if (m_iCursor > 2)
			m_iCursor = 0;
	}
	else if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Up))
	{
		--m_iCursor;
		SGD::AudioManager::GetInstance()->PlayAudio(Game::GetInstance()->GetMenuChangeSfx());

		if (m_iCursor < 0)
			m_iCursor = 2;
	}

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Left))
	{
		if (m_iCursor == 0)
		{
			
			if (Game::GetInstance()->GetMusicVolume() <= 0)
				Game::GetInstance()->SetMusicVolume(0);
			else
				Game::GetInstance()->SetMusicVolume(Game::GetInstance()->GetMusicVolume() - 5);
			ChangeMusicVolume(Game::GetInstance()->GetMusicVolume());
			
		}
		else if (m_iCursor == 1)
		{
			
			if (Game::GetInstance()->GetSfxVolume() <= 0)
				Game::GetInstance()->SetSfxVolume(0);
			else
				Game::GetInstance()->SetSfxVolume(Game::GetInstance()->GetSfxVolume() - 5);
			ChangeSfxVolume(Game::GetInstance()->GetSfxVolume());
			SGD::AudioManager::GetInstance()->PlayAudio(Game::GetInstance()->GetMenuChangeSfx());
		}
	}

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Right))
	{
		if (m_iCursor == 0)
		{
			
			if (Game::GetInstance()->GetMusicVolume() >= 100)
				Game::GetInstance()->SetMusicVolume(100);
			else
				Game::GetInstance()->SetMusicVolume(Game::GetInstance()->GetMusicVolume() + 5);
			ChangeMusicVolume(Game::GetInstance()->GetMusicVolume());
		}
		else if (m_iCursor == 1)
		{
			
			if (Game::GetInstance()->GetSfxVolume() >= 100)
				Game::GetInstance()->SetSfxVolume(100);
			else 
				Game::GetInstance()->SetSfxVolume(Game::GetInstance()->GetSfxVolume() + 5);

			ChangeSfxVolume(Game::GetInstance()->GetSfxVolume());
			SGD::AudioManager::GetInstance()->PlayAudio(Game::GetInstance()->GetMenuChangeSfx());
		}
	}

		
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter))
	{
		
		if (m_iCursor == 2)
		{
			SGD::AudioManager::GetInstance()->PlayAudio(Game::GetInstance()->GetMenuChangeSfx());
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
			return true;
		}
	}

	return true;
}

void OptionsState::Render(float elapsedTime)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetMenuBackground(), SGD::Point{ 0, 0 });
	BitmapFont* font = Game::GetInstance()->GetFont();
	font->Draw("Options", SGD::Point{ 335, 100 }, 1.5f);
	font->Draw("Music Volume", SGD::Point{ 350, 300 }, 0.7f, SGD::Color{ 235, 255, 0 });
	//font->Draw((char*)(GetMusicVolume()), SGD::Point{ 500, 300 }, 1.0f, SGD::Color{ 255, 255, 0 });
	// no numbers? WAII?!
	font->Draw("Sound Effects Volume", SGD::Point{ 350, 380 }, 0.7f, SGD::Color{ 235, 255, 0 });
	font->Draw("Exit", SGD::Point{ 350, 460 }, 0.7f, SGD::Color{ 235, 255, 0 });

	font->Draw("0", SGD::Point{ 310, 300.0f + 80 * m_iCursor }, 0.7f, SGD::Color{ 235, 255, 255 });

	DrawMusicVolume();
	DrawSfxVolume();
	DrawSoundBars();
}

void OptionsState::ChangeSfxVolume(int vol)
{
	// changes volume of all Sfx
	SGD::AudioManager::GetInstance()->SetAudioVolume(Game::GetInstance()->GetMenuChangeSfx(), vol);
	SGD::AudioManager::GetInstance()->SetAudioVolume(Game::GetInstance()->GetGameOverSfx(), vol);
	SGD::AudioManager::GetInstance()->SetAudioVolume(Game::GetInstance()->GetSecShotSfx(), vol);
	SGD::AudioManager::GetInstance()->SetAudioVolume(Game::GetInstance()->GetEnemyHitSfx(), vol);
	SGD::AudioManager::GetInstance()->SetAudioVolume(Game::GetInstance()->GetGameWinSfx(), vol);

}

void OptionsState::ChangeMusicVolume(int vol)
{
	SGD::AudioManager::GetInstance()->SetAudioVolume(Game::GetInstance()->GetBackgroundMus(), vol);

}

void OptionsState::DrawSfxVolume()
{
	BitmapFont* font = Game::GetInstance()->GetFont();
	std::string tempString = std::to_string(Game::GetInstance()->GetSfxVolume());
	const char* vol = tempString.c_str();
	font->Draw(vol, SGD::Point{ 450, 420 }, 0.7f);
}

void OptionsState::DrawMusicVolume()
{
	BitmapFont* font = Game::GetInstance()->GetFont();
	std::string tempString = std::to_string(Game::GetInstance()->GetMusicVolume());
	const char* vol = tempString.c_str();
	font->Draw(vol, SGD::Point{ 450, 340 }, 0.7f);
}

void OptionsState::OutputVolumeToFile()
{


	std::ofstream fout;
	fout.open("soundVolume.txt", std::ios::out| std::ios::trunc);

	if (fout.is_open())
	{
		
		fout << Game::GetInstance()->GetMusicVolume();
		fout << "\n";
		fout << Game::GetInstance()->GetSfxVolume();
		fout << "\n";

	}

	fout.close();

}

void OptionsState::ReadInVolume()
{
	std::ifstream fin;
	fin.open("soundVolume.txt", std::ios::in);

	int musicVol = 0, sfxVol = 0;

	if (fin.is_open())
	{
		fin >> musicVol;
		fin >> sfxVol;
	}

	fin.close();

	Game::GetInstance()->SetMusicVolume(musicVol);
	ChangeMusicVolume(musicVol);
	Game::GetInstance()->SetSfxVolume(sfxVol);
	ChangeSfxVolume(sfxVol);
}
void OptionsState::DrawSoundBars()
{
	SGD::GraphicsManager::GetInstance()->DrawLine(SGD::Point{ 355, 365 },
		SGD::Point{ 355 + (float)(Game::GetInstance()->GetMusicVolume() * 2.5f), 365 },
		SGD::Color{ 0, 255, 0 });

	SGD::GraphicsManager::GetInstance()->DrawLine(SGD::Point{ 355, 445 }, 
		SGD::Point{ 355 + (float)(Game::GetInstance()->GetSfxVolume() * 2.5f), 445 },
		SGD::Color{ 0, 255, 0 });

}