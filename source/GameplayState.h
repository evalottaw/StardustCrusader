//*********************************************************************//
//	File:		GameplayState.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#pragma once

#include "IGameState.h"							// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Declarations.h"	// uses Message
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Player.h"

//*********************************************************************//
// Forward class declaration
//	- tells the compiler that the type exists
//	- can make pointers or references to the type
//	- MUST include their headers in the .cpp to dereference
class Entity;
class EntityManager;
class Player;


//*********************************************************************//
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static GameplayState* GetInstance( void );
	static void  DeleteInstance(void);

	
	//*****************************************************************//
	// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources / reset data
	virtual void	Exit	( void )				override;	// unload resources
													
	virtual bool	Update	( float elapsedTime )	override;	// handle input & update game entities
	virtual void	Render	( float elapsedTime )	override;	// render game entities / menus


	Entity* CreatePlayer(void);
	Entity* CreateLvl1Enemy(int _y);
	Entity* CreateProjectile(Entity* entity);

	SGD::HTexture GetLevelBackground(int _level);
	SGD::HTexture GetEnemyImg(void) const { return m_hEnemyImgL1; }

	

	bool IsGameLost() const { return m_bGameLost; }
	void SetGameLost(bool _lost) { m_bGameLost = _lost; }
	bool IsGamePaused() const { return m_bisGamePaused; }
	bool Pause(void);

	bool DoubleDmg() { return m_bisDoubleDmg; }
	void SetDoubleDmg(bool _double) { m_bisDoubleDmg = _double; }


private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	GameplayState( void )			= default;	// default constructor
	virtual ~GameplayState( void )	= default;	// destructor

	GameplayState( const GameplayState& )				= delete;	// copy constructor
	GameplayState& operator= ( const GameplayState& )	= delete;	// assignment operator
		
	static GameplayState* s_pInstance;

	//*****************************************************************//
	// Game Entities
	EntityManager*	m_pEntities			= nullptr;
	

	//*****************************************************************//
	// Message Callback Procedure
	static void MessageProc( const SGD::Message* pMsg );

	
	SGD::HTexture m_hLevel1Background = SGD::INVALID_HANDLE;
	SGD::HTexture m_hEnemyImgL1 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hProjectileSecImage = SGD::INVALID_HANDLE;

	


	Entity* m_pPlayer;
	bool m_bisGamePaused = false, m_bisKeyPressed = false;
	bool playBackgMus = true, m_bGameLost = false, m_bPlayGameOverSfx = true, m_bPlayWinSfx = true;
	bool m_bisDoubleDmg = false;
	int m_iCursor = 0;
	float m_fWait = 5.0f;

	

	// helper
	void DrawPlayerScore(void);
	void DrawEnemiesLeft(void);
	void HoldEnemyCreation(int _level);
	bool GameIsLost(float time);
	bool GameIsWon(float time);

};
