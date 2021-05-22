#include "G2_Tile.h"

G2_Tile::G2_Tile(Texture *tex, Vector2f pos)
{
	type = 60;
	Checked = false;
	Now.setTexture(*tex);
	Now.setTextureRect(IntRect(0, 0, 20, 20));
	Now.setPosition(pos);
}