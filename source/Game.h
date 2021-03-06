//*********************************************************************//
//	File:		Game.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Game class initializes the SGD Wrappers
//				and runs the game state machine
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

//*********************************************************************//
// Forward class declarations
class BitmapFont;
class IGameState;
class Player;
class EntityManager;



//*********************************************************************//
// Game class
//	- handles the SGD wrappers
//	- runs the game state machine
//	- SINGLETON
//		- only ONE instance can be created
//		- global access to THE instance
class Game
{
public:
	//*****************************************************************//
	// SINGLETON!
	//	- static accessor to get the singleton object
	static Game* GetInstance( void );
	static void  DeleteInstance( void );

	
	//*****************************************************************//
	// Setup, Play, Cleanup
	bool	Initialize	( void );
	int		Update		( void );
	void	Terminate	( void );
	
	
	//*****************************************************************//
	// Screen Accessors
	SGD::Size	GetScreenSize	( void ) const	{	return m_szScreenSize;	}

	// Font Accessor (#include "BitmapFont.h" to use!)
	BitmapFont*	GetFont			( void ) const	{	return	m_pFont;		}


	//*****************************************************************//
	// Game State Mutator:
	void	ChangeState( IGameState* pNextState );


	SGD::HTexture GetMenuBackground() const { return m_hMainMenuBackground; }
	SGD::HTexture GetPlayerImg() const { return m_hPlayerImg; }
	SGD::HTexture GetEnemyImg() const { return m_hEnemyImg; }

	SGD::HAudio GetMenuChangeSfx() const { return m_hMenuChangeSfx; }
	SGD::HAudio GetSecShotSfx(void) const { return m_hProjectileSecSfx; }
	SGD::HAudio GetGameOverSfx(void) const { return m_hGameOverSfx; }
	SGD::HAudio GetGameWinSfx(void) const { return m_hGameWinSfx; }
	SGD::HAudio GetEnemyHitSfx(void) const { return m_hEnemyHitSfx; }
	SGD::HAudio GetBackgroundMus(void) const { return m_hBackgroundMus; }

	bool IsGameLost() const { return m_bisGameLost; }
	bool IsGameWon() const { return m_bisVictory; }

	int GetNumEnemies() const { return m_iNumEnemies; }


	void SetGameLost(bool _lost) { m_bisGameLost = _lost; }
	void SetVictory(bool _vic) { m_bisVictory = _vic; }
	void SetNumEnemies(int _num) { m_iNumEnemies = _num; }

	// access
	int GetMusicVolume() const { return m_uiMusicVol; }
	int GetSfxVolume() const { return m_uiSfxVol; }

	// mutate
	void SetMusicVolume(int _vol) { m_uiMusicVol = _vol; }
	void SetSfxVolume(int _vol) { m_uiSfxVol = _vol; }

	

	
private:
	//*****************************************************************//
	// SINGLETON!
	//	- static member to hold the singleton object
	//	- prevent access to constructors / destructor / =op
	static Game* s_pInstance;

	Game( void )	= default;					// default constructor
	~Game( void )	= default;					// destructor

	Game( const Game& )				= delete;	// disabled copy constructor
	Game& operator=( const Game& )	= delete;	// disabled assignment operator


	//*****************************************************************//
	// Screen Size
	SGD::Size		m_szScreenSize		= SGD::Size{ 1024, 768 };


	
	// Font
	BitmapFont*		m_pFont				= nullptr;


	//*****************************************************************//
	// Active Game State
	IGameState*		m_pCurrState		= nullptr;
	

	//*****************************************************************//
	// Game Time
	unsigned long	m_ulGameTime	= 0;


	SGD::HTexture m_hMainMenuBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPlayerImg = SGD::INVALID_HANDLE;
	SGD::HTexture m_hEnemyImg = SGD::INVALID_HANDLE;

	SGD::HAudio m_hMenuChangeSfx = SGD::INVALID_HANDLE;
	SGD::HAudio m_hProjectileSecSfx = SGD::INVALID_HANDLE;
	SGD::HAudio m_hBackgroundMus = SGD::INVALID_HANDLE;
	SGD::HAudio m_hEnemyHitSfx = SGD::INVALID_HANDLE;
	SGD::HAudio m_hGameOverSfx = SGD::INVALID_HANDLE;
	SGD::HAudio m_hGameWinSfx = SGD::INVALID_HANDLE;

	bool m_bIsFullscreenToggled = true;
	bool m_bIsKeyPressed = false;
	bool m_bisGameLost = false;
	bool m_bisVictory = false;


	int m_iNumEnemies = 5;
	int m_uiMusicVol = 100, m_uiSfxVol = 100;

	void CreateIntroScreenUpd(float elapsedTime);
	void CreateIntroScreenRend(float elapsedTime);


	float opacity = 255.0f;
};
