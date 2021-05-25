#include "Fluid.h"

Fluid::Fluid(Vector2f pos, const string& type)
	: type{type}, hasFluid{false}
{
	this->body.setSize(Vector2f{ 1.f, 1.f }); // Fluid size is literally a pixel.
	this->body.setPosition(pos);
	this->body.setFillColor(Color::Black); // Setting fluid to nothingness color.
}

void Fluid::drawFluids(RenderTexture& texture, const vector<vector<Fluid*>>& fluids)
{
	for (int i{ 0 }; i < fluids.size(); i++) // Draw all fluids which contain real fluid.
	{
		for (int j{ 0 }; j < fluids[i].size(); j++)
		{
			if (fluids[i][j]->hasFluid) texture.draw(fluids[i][j]->body);
		}
	}
}