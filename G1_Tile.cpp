#include "G1_Tile.h"
G1_Tile::G1_Tile(int number, Vector2f pos)
{
	Num = number;
	Rectangle.setPosition(pos);
	Rectangle.setSize(V2f(198.f, 198.f));
	Rectangle.setFillColor(Color::White);
	Rectangle.setOutlineColor(Color::Black);
	Rectangle.setOutlineThickness(FOw(2.f));
}