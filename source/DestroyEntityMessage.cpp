//*********************************************************************//
//	File:		DestroyEntityMessage.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	DestroyEntityMessage class stores the Entity to remove
//				from the Entity Manager
//				This message will be queued into the Message Manager
//				and processed by the Game.
//*********************************************************************//

#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_Utilities.h"
#include "Entity.h"
#include "MessageID.h"


//*********************************************************************//
// CONSTRUCTOR
//	- store the entity parameter
DestroyEntityMessage::DestroyEntityMessage( Entity* ptr )
	: Message( MessageID::MSG_DESTROY_ENTITY )
{
	// Validate the parameter
	SGD_ASSERT( ptr != nullptr,
		"DestroyEntityMessage - parameter cannot be null" );

	// Store the entity
	m_pEntity = ptr;
	m_pEntity->AddRef();
}

//*********************************************************************//
// DESTRUCTOR
//	- release the entity
DestroyEntityMessage::~DestroyEntityMessage()
{
	m_pEntity->Release();
	m_pEntity = nullptr;
}
