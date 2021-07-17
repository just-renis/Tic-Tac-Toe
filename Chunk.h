#pragma once
#include "Fluid.h"
class Chunk
{
public:
	Chunk(Vector2f pos, Vector2i whichChunk);
	~Chunk();
	void drawChunk(RenderTexture& texture);
	void logic();
	void changeChunkColor(bool pixelColor)
	{
		if (pixelColor)
		{
			Color color{ static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256) };
			for (int i{ 0 }; i < this->fluids.size(); i++)
			{
				for (int j{ 0 }; j < this->fluids[i].size(); j++)
				{
					this->fluids[i][j]->setColor(color);
					this->needToBeDrawed = true;
				}
			}
		}
		else
		{
			for (int i{ 0 }; i < this->fluids.size(); i++)
			{
				for (int j{ 0 }; j < this->fluids[i].size(); j++)
				{
					this->fluids[i][j]->setColor(Color{ static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256) });
					this->needToBeDrawed = true;
				}
			}
		}
	}
	// ------------------------------------------------------------------- GETTERS/SETTERS ------------------------------------------------------
	const Vector2f getPos() const { return this->pos; } // Getter of chunk position.
	friend class MouseClick; // Chunk is a friend of MouseClick class.
private:
	Vector2f pos; // Chunk position.
	vector<vector<Fluid*>> fluids; // Fluids container.
	bool needToBeDrawed; // Is this chunk need to be drawed?
	bool hasAnyMovingFluids; // Is this chunk has any moving fluids?
	bool canCreateFluid(Vector2i pixelPos); // Checks boundaries if can create a fluid in this chunk.
	void createFluid(const string& type, Vector2i pos); // Creating fluid function.
};