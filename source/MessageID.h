//*********************************************************************//
//	File:		MessageID.h
//	Author:		
//	Course:		
//	Purpose:	MessageID enum class defines the message types
//*********************************************************************//

#pragma once


//*********************************************************************//
// MessageID enum class
//	- unique identifiers for messages sent to Game::MessageProc
enum class MessageID	
{
	MSG_CREATE_BULLET,
	MSG_DESTROY_ENTITY
};
