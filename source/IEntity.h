//*********************************************************************//
//	File:		IEntity.h
//	Author:		
//	Course:		
//	Purpose:	IEntity class is the base interface for all entities,
//				declaring the shared virtual methods
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"	// Rectangle type


//*********************************************************************//
// IEntity class
//	- interface base class:
//		- virtual methods for children classes to override
//		- no data members
class IEntity
{
public:
	//*****************************************************************//
	// Interface:
	//	- pure virtual methods MUST be overridden in the child class
	virtual void	Update			( float elapsedTime )		= 0;
	virtual void	Render			( void )					= 0;
	
	virtual int		GetType			( void )	const			= 0;
	virtual SGD::Rectangle GetRect	( void )	const			= 0;
	virtual void	HandleCollision	( const IEntity* pOther )	= 0;


	//*****************************************************************//
	// Reference Counting:
	//	- keep the object in memory as long as there is a pointer
	//	  to the object
	virtual void	AddRef			( void )					= 0;
	virtual void	Release			( void )					= 0;

protected:
	//*****************************************************************//
	// Destructor MUST be virtual
	IEntity( void )				= default;
	virtual ~IEntity( void )	= default;		// protected to force reference counting

};
