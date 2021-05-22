#pragma once
#include "VoidFunctions.h"
#include "Player.h"

class Power
{
public:
	string type;
	RectangleShape body;
	Power(Vector2f pos, string type);
};
