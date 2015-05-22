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


	// esc puts cursor at Exit
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
		m_iCursor = 2;

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Down))
	{
		++m_iCursor;
		if (m_iCursor > 2)
			m_iCursor = 0;
	}
	else if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Up))
	{
		--m_iCursor;
		if (m_iCursor < 0)
			m_iCursor = 2;
	}

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Left))
	{
		if (m_iCursor == 0)
		{
			SetMusicVolume(GetMusicVolume() - 1);
			if (GetMusicVolume() <= 0)
				SetMusicVolume(0);
		}
	}

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Right))
	{
		if (m_iCursor == 1)
		{
			SetMusicVolume(GetMusicVolume() + 1);
			if (GetMusicVolume() >= 100)
				SetMusicVolume(100);
		}
	}

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter))
	{
		if (m_iCursor == 2)
		{
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
	font->Draw("Options", SGD::Point{ 360, 100 }, 1.5f);
	font->Draw("Music", SGD::Point{ 400, 300 }, 1.0f, SGD::Color{ 255, 255, 0 });
	//font->Draw((char*)(GetMusicVolume()), SGD::Point{ 500, 300 }, 1.0f, SGD::Color{ 255, 255, 0 });
	// no numbers? WAII?!
	font->Draw("Sound Effects", SGD::Point{ 400, 350 }, 1.0f, SGD::Color{ 255, 255, 0 });
	font->Draw("Exit", SGD::Point{ 400, 400 }, 1.0f, SGD::Color{ 255, 255, 0 });
	font->Draw("0", SGD::Point{ 360, 300.0f + 50 * m_iCursor }, 1.0f, SGD::Color{ 255, 255, 255 });

}