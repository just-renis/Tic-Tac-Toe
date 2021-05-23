#pragma once
#include "PerfectCollision.h"
#include "VoidFunctions.h"
#include "Chunk.h"

class MouseClick
{
public:
	MouseClick(const string& type);
	void click(Vector2f pos, char size, vector<vector<Chunk*>>& chunks);
private:
	string type;
	vector<bool> circle2;
};