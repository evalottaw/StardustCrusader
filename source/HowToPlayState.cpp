#include "HowToPlayState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "Player.h"
#include "GameplayState.h"

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


HowToPlayState::HowToPlayState()
{
}


HowToPlayState::~HowToPlayState()
{
}

HowToPlayState* HowToPlayState::GetInstance(void)
{
	static HowToPlayState s_Instance;
	return &s_Instance;
}


void HowToPlayState::Enter(void)
{
	m_pPlayer = new Player;
}

void HowToPlayState::Exit(void)
{
	delete m_pPlayer;
	m_pPlayer = nullptr;
}
	 
bool HowToPlayState::Update(float elapsedTime)
{
	
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());


	return true;
}

void HowToPlayState::Render(float elapsedTime)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetMenuBackground(), SGD::Point{ 0, 0 });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_pPlayer->GetImage(), SGD::Point{ 240, 200 }, SGD::Rectangle{ 0, 0, 32, 32 }, 0, {}, {}, SGD::Size{ 2.0f, 2.0f });

	BitmapFont* font = Game::GetInstance()->GetFont();

	font->Draw("How To Play", SGD::Point{ 260, 100 }, 1.5f);
	font->Draw("You are playing as Mr Fox", SGD::Point{ 330, 230 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("A moves Mr Fox to the left", SGD::Point{ 260, 280 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("D moves Mr Fox to the right", SGD::Point{ 260, 300 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("Space makes Mr Fox jump", SGD::Point{ 260, 320 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("1, 2, 3 swaps your weapons", SGD::Point{ 260, 340 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("Use the mouse cursor to aim and shoot", SGD::Point{ 260, 360 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("You are the only survivor after an", SGD::Point{ 260, 420 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("alien virus has spread across Planet Earth.", SGD::Point{ 260, 440 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("Your job is to fight your way through ", SGD::Point{ 260, 460 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("the Milky Way, and destroy the evil", SGD::Point{ 260, 480 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("alien Helios that resides at the Sun.", SGD::Point{ 260, 500 }, 0.5f, SGD::Color{ 255, 255, 0 });



	
	// draw the rest of the instructions here later
}