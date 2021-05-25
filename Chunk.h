#pragma once
#include "Fluid.h"
class Chunk
{
public:
	Chunk(Vector2f pos);
	~Chunk();
	void drawChunk(RenderTexture& texture);
	void changeChunkColor()
	{
		Color color{ static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256) };
		for (int i{ 0 }; i < this->fluids.size(); i++)
		{
			for (int j{ 0 }; j < this->fluids[i].size(); j++)
			{
				this->fluids[i][j]->setColor(color);
				this->hasFluids = true;
			}
		}
	}
	// ------------------------------------------------------------------- GETTERS/SETTERS ------------------------------------------------------
	const Vector2f getPos() const { return this->pos; } // Getter of chunk position.
	friend class MouseClick; // Chunk is a friend of MouseClick class.
private:
	Vector2f pos; // Chunk position.
	vector<vector<Fluid*>> fluids; // Fluids container.
	bool hasFluids; // Is this chunk has any fluids?
	bool canCreateFluid(Vector2i pixelPos); // Checks boundaries if can create a fluid in this chunk.
	void createFluid(const string& type, Vector2i pos); // Creating fluid function.
};