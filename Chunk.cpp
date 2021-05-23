#include "Chunk.h"

Chunk::Chunk(Vector2f pos)
	: hasFluids{false}, pos{pos}
{
	for (int i{ 0 }; i < 20; i++)
	{
		fluids.push_back(vector<Fluid*>{});
		for (int j{ 0 }; j < 20; j++)
		{
			fluids[i].push_back(new Fluid{Vector2f{ pos.x + i, pos.y + j }, "nothing"});
		}
	}
}

Chunk::~Chunk()
{
	for (int i{ 0 }; i < fluids.size(); i++)
	{
		for (int j{ 0 }; j < fluids[i].size(); j++)
		{
			delete fluids[i][j];
		}
	}
}

void Chunk::drawChunk(RenderWindow& window)
{
	if (this->hasFluids) Fluid::drawFluids(window, this->fluids);
}

void Chunk::createFluid(const string& type, Vector2i pos)
{
	this->fluids[pos.x][pos.y]->setColor(Color::Blue);
	this->fluids[pos.x][pos.y]->setType("water");
	this->hasFluids = true;
}