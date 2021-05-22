#pragma once
#include "Program.h"
class G2_Tile
{
public:
	int type;
	Sprite Now;
	bool Checked;
	G2_Tile(Texture *tex, Vector2f pos);
};