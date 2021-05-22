#include "Fluid.h"

Fluid::Fluid(Vector2f pos, const string& type)
	: type{type}
{
	this->body.setSize(Vector2f{ 1.f, 1.f });
	this->body.setPosition(pos);
	if (this->type == "water") this->body.setFillColor(Color::Blue);
}

void Fluid::drawFluids(RenderWindow& window, const vector<Fluid>& fluids)
{
	for (int i{ 0 }; i < fluids.size(); i++) window.draw(fluids[i].body);
}