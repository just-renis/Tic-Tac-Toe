#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Fluid
{
public:
	Fluid(Vector2f pos, const string& type);
	static void drawFluids(RenderWindow& window, const vector<vector<Fluid*>>& fluids);
	const string& getType() const { return this->type; }
	void setColor(Color color) { this->body.setFillColor(color); }
	void setType(const string& type) { this->type = type, this->hasFluid = true; }
protected:

private:
	bool hasFluid;
	RectangleShape body;
	string type;
};