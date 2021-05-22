#pragma once
#include "Program.h"
#include "VoidFunctions.h"

class G1_Tile
{
public:
	int Num;
	bool Full = false;
	int Contains = 0;
	RectangleShape Rectangle;
	G1_Tile(int number, Vector2f pos);
};