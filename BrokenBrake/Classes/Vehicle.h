#pragma once

#include "cocos2d.h"
#include "Defines.h"

class Vehicle
{
public:
	Vehicle();
	virtual ~Vehicle();

	void release();

	cocos2d::Sprite* body;
};