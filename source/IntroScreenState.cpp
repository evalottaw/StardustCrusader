//*********************************************************************//
//	File:		IntroScreenState.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	handles the intro screen
//*********************************************************************//
#include "IntroScreenState.h"
#include "BitmapFont.h"
#include "Game.h"
#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

IntroScreenState* IntroScreenState::s_pInstance = nullptr;

IntroScreenState* IntroScreenState::GetInstance(void)
{
	if (s_pInstance == nullptr)
		s_pInstance = new IntroScreenState;
	return s_pInstance;
}
void  IntroScreenState::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}
void IntroScreenState::Enter(void)
{

}

void IntroScreenState::Exit(void)
{
	IntroScreenState::GetInstance()->DeleteInstance();
}

bool IntroScreenState::Update(float elapsedTime)
{
	if (elapsedTime / 5 == 0)
		opacity = -opacity;
	

	if (SGD::InputManager::GetInstance()->IsAnyKeyPressed())
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

	return true;
}

void IntroScreenState::Render(float elapsedTime)
{

	SGD::GraphicsManager::GetInstance()->SetClearColor(SGD::Color{ 0, 0, 0 });

	BitmapFont* font = Game::GetInstance()->GetFont();
	float width = Game::GetInstance()->GetScreenSize().width;
		font->Draw("STARDUST", { (width - (4 * 32 * 4.5f)) / 2, 50 },
			2.5f, { (unsigned char)opacity, 255, 255, 255 });
		font->Draw("CRUSADER", { (width - (4 * 32 * 4.5f)) / 2, 130 },
			2.5f, { (unsigned char)opacity, 255, 255, 255 });

		font->Draw("Press any key to continue...", { (width - (4 * 32 * 2.5f) - 100) / 2, 700 },
			0.5f, { 255, 255, 255, 255 });

}

