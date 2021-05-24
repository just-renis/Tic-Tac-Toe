#include "Chunk.h"

Chunk::Chunk(Vector2f pos)
	: hasFluids{false}, pos{pos}
{
	for (int i{ 0 }; i < 20; i++)
	{
		fluids.push_back(vector<Fluid*>{}); // Creating row of fluids container which contain nothing.
		for (int j{ 0 }; j < 20; j++)
		{
			fluids[i].push_back(new Fluid{Vector2f{ pos.x + i, pos.y + j }, "nothing"}); // Creating columns of 'i' row fluids which contain nothing.
		}
	}
}

Chunk::~Chunk()
{
	for (int i{ 0 }; i < fluids.size(); i++) // When chunk is destroyed, then fluids are destroyed as well.
	{
		for (int j{ 0 }; j < fluids[i].size(); j++)
		{
			delete fluids[i][j];
		}
	}
}

void Chunk::drawChunk(RenderWindow& window)
{
	if (this->hasFluids) Fluid::drawFluids(window, this->fluids); // If this chunk has any fluids, draw those fluids.
}

// ---------------------------------------------------------------------- PRIVATE FUNCTIONS -----------------------------------------------------------------

bool Chunk::canCreateFluid(Vector2i pixelPos)
{
	if (this->pos.x <= pixelPos.x && this->pos.x + 20.f > pixelPos.x) // If pixel position is in chunk 'x' region - proceed.
	{
		if (this->pos.y <= pixelPos.y && this->pos.y + 20.f > pixelPos.y) // If pixel position is in chunk 'y' region - proceed.
		{
			return true;
		}
	}
	return false;
}

void Chunk::createFluid(const string& type, Vector2i pos)
{
	this->fluids[pos.x][pos.y]->setColor(Color::Blue); // Set water color for fluid.
	this->fluids[pos.x][pos.y]->setType("water"); // Set type of fluid to water.
	this->hasFluids = true; // This chunk now has fluids.
}