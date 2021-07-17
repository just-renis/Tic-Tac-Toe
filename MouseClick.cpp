#include "MouseClick.h"

MouseClick::MouseClick(const string& type)
	: type{type}
{
	this->circle2 = // Circle pattern.
	{
		0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0
	};
}

void MouseClick::click(Vector2i pos, char size, vector<vector<Chunk*>>& chunks)
{
	switch (size)
	{
		case '1': // First click size.
		for (int i{ -1 }; i < 1; i++)
		{
			for (int j{ -1 }; j < 1; j++)
			{
				//fluid.push_back(Fluid{ Vector2f{pos.x + i, pos.y + j}, this->type});
			}
		}
		break;
		case '2': // Second click size.
		for (int i{ 0 }; i < 20; i++)
		{
			for (int j{ 0 }; j < 20; j++)
			{
				if (this->circle2[i * 20 + j]) // If circle pattern is at the right place then proceed.
				{
					Vector2i pixelPos{ pos.x + i - 10, pos.y + j - 10 }; // Circle middle will be at the mouse click position.
					if (pixelPos.x >= 0 && pixelPos.y >= 0) MouseClick::loopThroughChunks(chunks, pixelPos); // If pixel position is not negative, then find chunk where the pixel position is.
				}
			}
		}
		break;
	}
}

void MouseClick::loopThroughChunks(vector<vector<Chunk*>>& chunks, Vector2i pixelPos)
{
	for (int i{ 0 }; i < chunks.size(); i++)
	{
		for (int j{ 0 }; j < chunks[i].size(); j++)
		{
			if (chunks[i][j]->canCreateFluid(pixelPos)) // If in this chunk, fluid can be created then proceed.
			{
				chunks[i][j]->createFluid("water", Vector2i{ pixelPos.x % 20, pixelPos.y % 20 }); // Creating "water" fluid in this pixel position.
				return; // Breaking out of loops and function.
			}
		}
	}
}