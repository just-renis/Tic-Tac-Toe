#include "Fluid.h"
#include <iostream>
using namespace std;

Fluid::Fluid(Vector2f pos, const string& type)
	: type{type}, hasFluid{false}, isSteady{false}
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

bool Fluid::fluidsLogic(vector<vector<Fluid*>>& fluids)
{
	bool needToDraw{ false };
	for (int i{ 19 }; i > 0; i--)
	{
		for (int j{ 19 }; j >= 0; j--)
		{
			if (!fluids[i][j]->isSteady)
			{
				if (i != 19 && fluids[i][j]->body.getFillColor() == Color::Blue && fluids[i + 1][j]->body.getFillColor() == Color::Black)
				{
					if (fluids[i - 1][j]->body.getFillColor() != Color::Black)
					{
						fluids[i][j]->type = fluids[i - 1][j]->type;
						fluids[i][j]->setColor(fluids[i - 1][j]->body.getFillColor());
						needToDraw = true;
					}
				}
				else fluids[i][j]->isSteady = true;
			}
		}
	}
	return needToDraw;
}