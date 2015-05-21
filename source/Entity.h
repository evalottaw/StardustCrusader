//*********************************************************************//
//	File:		Entity.h
//	Author:		
//	Course:		
//	Purpose:	Entity class is the base moving-object entity,
//				storing shared members for children classes
//*********************************************************************//

#pragma once

#include "IEntity.h"						// IEntity type
#include "../SGD Wrappers/SGD_Handle.h"		// HTexture type
#include "../SGD Wrappers/SGD_Geometry.h"	// Point & Vector type


//*********************************************************************//
// Entity class
//	- parent class of all game entities, stores the shared data members
//	- velocity-based movement
class Entity : public IEntity
{
	//*****************************************************************//
	// Destructor MUST be virtual for upcasted objects
	//		Entity* ptr = new Ship;
	//		delete ptr;
public:		Entity( void )			= default;
protected:	virtual ~Entity( void )	= default;		// protected to force reference counting
	

public:
	//*****************************************************************//
	// Entity Types:
	enum EntityType { ENT_BASE, ENT_PLAYER, ENT_ENEMY, ENT_ENEMY_BOSS};


	//*****************************************************************//
	// Interface:
	//	- virtual functions for children classes to override
	virtual void	Update		( float elapsedTime )	override;
	virtual void	Render		( void )				override;

	virtual int		GetType			( void )	const			override	{	return ENT_BASE;	}
	virtual SGD::Rectangle GetRect	( void )	const			override;
	virtual void	HandleCollision	( const IEntity* pOther )	override;
	

	//*****************************************************************//
	// Reference Counting:
	virtual void	AddRef		( void )				final;
	virtual void	Release		( void )				final;


	//*****************************************************************//
	// Accessors:
	SGD::HTexture	GetImage	( void ) const			{	return m_hImage;		}
	SGD::Point		GetPosition	( void ) const			{	return m_ptPosition;	}
	SGD::Vector		GetVelocity	( void ) const			{	return m_vtVelocity;	}
	SGD::Vector GetAcceleration(void) const { return m_vtAcceleration; }
	SGD::Size		GetSize		( void ) const			{	return m_szSize;		}
	float			GetRotation	( void ) const			{	return m_fRotation;		}
	
	// Mutators:
	void			SetImage	( SGD::HTexture	img  )	{	m_hImage		= img;	}
	void			SetPosition	( SGD::Point	pos  ) 	{	m_ptPosition	= pos;	}
	void			SetVelocity	( SGD::Vector	vel	 ) 	{	m_vtVelocity	= vel;	}
	void			SetSize		( SGD::Size		size ) 	{	m_szSize		= size;	}
	void			SetRotation	( float			rad	 )	{	m_fRotation		= rad;	}
	void SetAcceleration(SGD::Vector _acc) { m_vtAcceleration = _acc; }

protected:
	//*****************************************************************//
	// Shared members:
	SGD::HTexture	m_hImage		= SGD::INVALID_HANDLE;	// image handle
	SGD::Point		m_ptPosition	= SGD::Point{ 0, 0 };	// 2D position
	SGD::Vector		m_vtVelocity	= SGD::Vector{ 0, 0 };	// 2D velocity
	SGD::Size		m_szSize		= SGD::Size{ 0, 0 };	// 2D size
	float			m_fRotation		= 0.0f;

	SGD::Vector m_vtAcceleration = SGD::Vector{ 0, 0 };

private:
	//*****************************************************************//
	// reference count
	unsigned int	m_unRefCount	= 1;	// calling new gives the 'prime' reference

};
