//*********************************************************************//
//	File:		Game.h
//	Author:		
//	Course:		
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
	bool IsGameLost() const { return m_bisGameLost; }
	bool IsGameWon() const { return m_bisVictory; }

	int GetNumEnemies() const { return m_iNumEnemies; }


	void SetGameLost(bool _lost) { m_bisGameLost = _lost; }
	void SetVictory(bool _vic) { m_bisVictory = _vic; }
	void SetNumEnemies() {-- m_iNumEnemies; }
	
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


	bool m_bIsFullscreenToggled = true;
	bool m_bIsKeyPressed = false;
	bool m_bisGameLost = false;
	bool m_bisVictory = false;

	int m_iNumEnemies = 5;

};
