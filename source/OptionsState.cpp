#include "OptionsState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


OptionsState::OptionsState()
{
}


OptionsState::~OptionsState()
{
}

OptionsState* OptionsState::GetInstance(void)
{
	static OptionsState s_instance;
	return &s_instance;
}


void OptionsState::Enter(void)
{

}

void OptionsState::Exit(void)
{

}

bool OptionsState::Update(float elapsedTime)
{
	// esc exits back to main menu
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
	}

	return true;
}

void OptionsState::Render(float elapsedTime)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetMenuBackground(), SGD::Point{ 0, 0 });
	BitmapFont* font = Game::GetInstance()->GetFont();
	font->Draw("Options", SGD::Point{ 360, 100 }, 1.5f);
}