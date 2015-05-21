/***********************************************************************\
|																		|
|	File:			SGD_IListener.cpp  									|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-12-31											|
|																		|
|	Purpose:		To declare the interface used by the EventManager	|
|					to send events to registered listeners				|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_IListener.h"


// Uses Event Manager
#include "SGD_EventManager.h"


namespace SGD
{
	//*****************************************************************//
	// ILISTENER METHODS
	
	// EventManager Interactions:
	bool IListener::RegisterForEvent( const char* eventID )
	{
		return EventManager::GetInstance()->RegisterForEvent( this, eventID );
	}

	bool IListener::UnregisterFromEvent( const char* eventID )
	{
		return EventManager::GetInstance()->UnregisterFromEvent( this, eventID );
	}
	//*****************************************************************//


}	// namespace SGD
