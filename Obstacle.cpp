#include "Obstacle.h"

int Obstacle::IDcounter = 0;

Obstacle::Obstacle(Texture* tex, Vector2f pos)
	: ID{"O_" + to_string(Obstacle::IDcounter++)}
{
	this->body.setTexture(*tex);
	this->body.setScale(V2f(1.f, 1.f));
	this->body.setPosition(pos);
}

void Obstacle::Draw(RenderWindow& window, const vector<Obstacle*>& obstacles)
{
	for (int i = 0; i < obstacles.size(); i++) window.draw(obstacles[i]->body);
}

bool Obstacle::intersects(const Sprite& player, const vector<Obstacle*>& obstacles)
{
	for (int i = 0; i < obstacles.size(); i++)
	{
		if (Collision::PixelPerfectTest(obstacles[i]->body, player)) return false;
	}
	return true;
}