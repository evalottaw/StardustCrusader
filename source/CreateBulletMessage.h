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

