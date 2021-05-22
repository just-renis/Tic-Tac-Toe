#include "Power.h"

Power::Power(Vector2f pos, string type)
{
	Color color;
	this->type = type;
	if (type == "Freeze") color = Color::Cyan;
	else if (type == "2xSpeed") color = Color::Green;
	else if (type == "Push") color = Color(150, 75, 0); // Brown.
	else if (type == "2xCoin") color = Color::Yellow;
	else if (type == "Blind") color = Color::Black;
	else if (type == "Invisible") color = Color::White;
	else color = Color::Red;
	this->body = RectangleSetter(V2f(30.f, 30.f), V2f(pos.x, pos.y), color, Color::White, FOw(2.f));
}