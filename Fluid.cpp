#include "Fluid.h"

Fluid::Fluid(Vector2f pos, const string& type)
	: type{type}, hasFluid{false}
{
	this->body.setSize(Vector2f{ 1.f, 1.f });
	this->body.setPosition(pos);
	this->body.setFillColor(Color::Black);
}

void Fluid::drawFluids(RenderWindow& window, const vector<vector<Fluid*>>& fluids)
{
	for (int i{ 0 }; i < fluids.size(); i++)
	{
		for (int j{0}; j < fluids[i].size(); j++) if (fluids[i][j]->hasFluid) window.draw(fluids[i][j]->body);
	}
}