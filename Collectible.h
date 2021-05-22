#pragma once
#include "VoidFunctions.h"
#include "SoundPlayer.h"

class Collectible
{
public:
	Collectible(const string& type, Texture* tex, Vector2f pos);
	static void Draw(RenderWindow& window, vector<Collectible>& collectibles);
	static void intersects(const Sprite& player, vector<Collectible>& collectibles, string& collectedItem, string& NewSound);
private:
	Sprite body;
	string type;
	Clock animationTimer;
	float FrameTime;
	int frameEnd;
	void Animation();
};