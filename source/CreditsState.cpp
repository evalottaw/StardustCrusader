#include "CreditsState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"


CreditsState::CreditsState()
{
}


CreditsState::~CreditsState()
{
}

CreditsState* CreditsState::GetInstance(void)
{
	static CreditsState s_instance;
	return &s_instance;
}

void CreditsState::Enter(void)
{

}
void CreditsState::Exit(void)
{

}
bool CreditsState::Update(float elapsedTime)
{

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
	}

	return true;
}
void CreditsState::Render(float elapsedTime)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetMenuBackground(), SGD::Point{ 0, 0 });
	BitmapFont* font = Game::GetInstance()->GetFont();
	font->Draw("Credits", SGD::Point{ 320, 100 }, 1.5f);
	font->Draw("Eva-Lotta Wahlberg", SGD::Point{ 320, 300 }, 0.6f, SGD::Color{ 255,255, 255, 255 });
	font->Draw("Structure of Game Design", SGD::Point{ 280, 380 }, 0.6f, SGD::Color{ 255, 255, 255, 255 });
	font->Draw("at Full Sail University", SGD::Point{ 280, 400 }, 0.6f, SGD::Color{ 255, 255, 255, 255 });


}