#pragma once
#include "Fluid.h"
#include "PerfectCollision.h"

class MouseClick
{
public:
	MouseClick(const string& type);
	void click(Vector2f pos, char size, vector<Fluid>& fluid);
private:
	string type;
};