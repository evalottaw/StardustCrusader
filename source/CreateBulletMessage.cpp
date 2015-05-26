//*********************************************************************//
//	File:		CreateBulletMessage.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Creates a bullet message.
//*********************************************************************//

#include "CreateBulletMessage.h"
#include "MessageID.h"
#include "Player.h"

CreateBulletMessage::CreateBulletMessage(Entity* player) : Message(MessageID::MSG_CREATE_BULLET)
{
	m_tBulletOwner = player;
	player->AddRef();
}


CreateBulletMessage::~CreateBulletMessage()
{
	m_tBulletOwner->Release();
}

