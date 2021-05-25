#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Fluid
{
public:
	Fluid(Vector2f pos, const string& type);
	static void drawFluids(RenderTexture& texture, const vector<vector<Fluid*>>& fluids); // Drawing all pixels which contain fluids.
	const string& getType() const { return this->type; } // Getter of fluid type.
	void setColor(Color color) { this->body.setFillColor(color); } // Setter of fluid color.
	void setType(const string& type) { this->type = type, this->hasFluid = true; } // Setter of fluid type.
protected:

private:
	bool hasFluid; // If fluid has a real fluid.
	RectangleShape body; // Fluid body.
	string type; // Fluid type.
};