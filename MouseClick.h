#pragma once
#include "PerfectCollision.h"
#include "VoidFunctions.h"
#include "Chunk.h"

class MouseClick
{
public:
	MouseClick(const string& type);
	void click(Vector2i pos, char size, vector<vector<Chunk*>>& chunks); // When mouse left button is clicked.
private:
	string type; // What type of fluid every click will create.
	vector<bool> circle2; // What figure will make of fluids one mouse click.
	static void loopThroughChunks(vector<vector<Chunk*>>& chunks, Vector2i pixelPos); // Looping through all the chunks and checking stuff for it function.
};