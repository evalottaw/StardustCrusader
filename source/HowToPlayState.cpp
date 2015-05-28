//*********************************************************************//
//	File:		HowToPlayState.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Handles the HowToPlay State functionality
//*********************************************************************//
#include "HowToPlayState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "Player.h"
#include "GameplayState.h"
#include "Enemy.h"

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_EventManager.h"


HowToPlayState* HowToPlayState::s_pInstance = nullptr;


HowToPlayState* HowToPlayState::GetInstance(void)
{
	if (s_pInstance == nullptr)
		s_pInstance = new HowToPlayState;

	return s_pInstance;
}

void HowToPlayState::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

void HowToPlayState::Enter(void)
{
	
}

void HowToPlayState::Exit(void)
{
	HowToPlayState::GetInstance()->DeleteInstance();
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
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(Game::GetInstance()->GetPlayerImg(), SGD::Point{ 180, 200 }, SGD::Rectangle{ 0, 0, 32, 32 }, 0, {}, {}, SGD::Size{ 2.0f, 2.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(Game::GetInstance()->GetEnemyImg() , SGD::Point{ 120, 500 }, SGD::Rectangle{ 0, 0, 64, 80 }, 0, {}, {}, SGD::Size{ 2.0f, 2.0f });


	BitmapFont* font = Game::GetInstance()->GetFont();

	font->Draw("How To Play", SGD::Point{ 260, 100 }, 1.5f);
	font->Draw("You are playing as Mr Fox", SGD::Point{ 260, 230 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("A moves Mr Fox to the left", SGD::Point{ 260, 280 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("D moves Mr Fox to the right", SGD::Point{ 260, 300 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("W and S moves Mr Fox up and down", SGD::Point{ 260, 320 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("C charges your shot for double damage", SGD::Point{ 260, 340 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("Use the Space bar shoot", SGD::Point{ 260, 360 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("You are the only survivor after an", SGD::Point{ 260, 420 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("alien virus has spread across Planet Earth.", SGD::Point{ 260, 440 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("Your job is to fight your way through ", SGD::Point{ 260, 460 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("the Milky Way. Don't let anything get past you.", SGD::Point{ 260, 480 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("This is the enemy you have to overcome.", SGD::Point{ 260, 550 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("Simply shoot them to advance in the level.", SGD::Point{ 260, 570 }, 0.5f, SGD::Color{ 255, 255, 0 });
	font->Draw("Hint: They need headshots to die.", SGD::Point{ 260, 600 }, 0.5f, SGD::Color{ 255, 255, 0 });




	
	// draw the rest of the instructions here later
}