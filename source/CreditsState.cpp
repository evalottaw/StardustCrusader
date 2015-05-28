//*********************************************************************//
//	File:		CreditsState.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the Credits State
//*********************************************************************//
#include "CreditsState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

CreditsState* CreditsState::s_pInstance = nullptr;


CreditsState* CreditsState::GetInstance(void)
{
	if (s_pInstance == nullptr)
		s_pInstance = new CreditsState;
	return s_pInstance;
}

void CreditsState::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

void CreditsState::Enter(void)
{
	
	
}
void CreditsState::Exit(void)
{
	CreditsState::GetInstance()->DeleteInstance();

}
bool CreditsState::Update(float elapsedTime)
{
	if (Game::GetInstance()->IsGameLost()
		|| Game::GetInstance()->IsGameWon())
	{
		m_fWait -= elapsedTime;
		if (m_fWait <= 0)
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
	}
	

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
	}

	return true;
}
void CreditsState::Render(float elapsedTime)
{
	m_fScrollCreditsAm += elapsedTime * 60;

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetMenuBackground(), SGD::Point{ 0, 0 });
	BitmapFont* font = Game::GetInstance()->GetFont();
	font->Draw("Credits", SGD::Point{ 320, 200 - m_fScrollCreditsAm }, 1.5f);
	font->Draw("Eva-Lotta Wahlberg", SGD::Point{ 320, 400 - m_fScrollCreditsAm }, 0.6f, SGD::Color{ 255, 255, 255, 255 });
	font->Draw("Structure of Game Design", SGD::Point{ 280, 480 - m_fScrollCreditsAm }, 0.6f, SGD::Color{ 255, 255, 255, 255 });
	font->Draw("at Full Sail University", SGD::Point{ 280, 500 - m_fScrollCreditsAm }, 0.6f, SGD::Color{ 255, 255, 255, 255 });


}