#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Fluid
{
public:
	Fluid(Vector2f pos, const string& type);
	static void drawFluids(RenderWindow& window, const vector<Fluid>& fluids);
protected:

private:
	RectangleShape body;
	string type;
};