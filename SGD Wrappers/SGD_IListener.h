/***********************************************************************\
|																		|
|	File:			SGD_IListener.h										|
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

#ifndef SGD_LISTENER_H
#define SGD_LISTENER_H


namespace SGD
{
	// Forward declaration
	class Event;


	//*****************************************************************//
	// IListener
	//	- parent class for objects registered with the Event Manager
	//	- children classes override the HandleEvent method, which is called
	//	  when a registered Event is processed
	class IListener
	{
	public:
		IListener			( void )	= default;									// Default constructor
		virtual ~IListener	( void )	{	UnregisterFromEvent( nullptr );		}	// Destructor
		
		// EventManager Interactions:
		bool			RegisterForEvent	( const char* eventID );
		bool			UnregisterFromEvent	( const char* eventID = nullptr );

		// Listener Interface:
		virtual void	HandleEvent			( const SGD::Event* pEvent )	= 0;	// Callback function to process events

	};

}	// namespace SGD

#endif	//SGD_LISTENER_H
