//*********************************************************************//
//	File:		CreateBulletMessage.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	Creates a bullet message.
//*********************************************************************//
#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "Player.h"


class CreateBulletMessage :
	public SGD::Message
{
public:
	CreateBulletMessage(Entity* player);
	~CreateBulletMessage();

	// access
	Entity* GetBulletOwner() const { return m_tBulletOwner; }

private:

	Entity* m_tBulletOwner = nullptr;
};

