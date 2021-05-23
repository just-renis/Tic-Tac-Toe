#pragma once
#include "Fluid.h"
class Chunk
{
public:
	Chunk(Vector2f pos);
	~Chunk();
	void drawChunk(RenderWindow& window);
	const Vector2f getPos() const { return this->pos; }
	void createFluid(const string& type, Vector2i pos);
	void changeChunkColor()
	{
		Color color{ static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256) };
		for (int i = 0; i < this->fluids.size(); i++)
		{
			for (int j = 0; j < this->fluids[i].size(); j++)
			{
				this->fluids[i][j]->setColor(color);
			}
		}
	}
private:
	Vector2f pos;
	vector<vector<Fluid*>> fluids;
	bool hasFluids;
};