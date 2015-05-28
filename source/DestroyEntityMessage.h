//*********************************************************************//
//	File:		DestroyEntityMessage.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	DestroyEntityMessage class stores the Entity to remove
//				from the Entity Manager
//				This message will be queued into the Message Manager
//				and processed by the Game.
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Message.h"
class Entity;


//*********************************************************************//
// DestroyEntityMessage class
//	- stores the entity to be removed from the Entity Manager
class DestroyEntityMessage : public SGD::Message
{
public:
	//*****************************************************************//
	// Constructor MUST be given the entity to destroy!
	DestroyEntityMessage( Entity* ptr );
	virtual ~DestroyEntityMessage();
	
	//*****************************************************************//
	// Accessor:
	Entity*		GetEntity( void ) const	{	return m_pEntity;	}

private:
	//*****************************************************************//
	// Entity to remove from the Entity Manager
	Entity*		m_pEntity	= nullptr;
};

