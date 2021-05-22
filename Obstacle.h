#pragma once
#include "VoidFunctions.h"

class Obstacle
{
public:
	Obstacle(Texture* tex, Vector2f pos);
	static void Draw(RenderWindow& window, const vector<Obstacle*>& obstacles);
	static bool intersects(const Sprite& player, const vector<Obstacle*>& obstacles);
	const FloatRect getBounds() const { return this->body.getGlobalBounds(); }
	const string& getID() const { return this->ID; }
private:
	Sprite body;
	static int IDcounter;
	string ID;
};