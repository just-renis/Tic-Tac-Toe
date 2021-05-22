#include "Spawner.h"

Spawner::Spawner(Texture* texSpawner, Vector2f pos, float spawnTrapSecs, char direction, float distance)
	: spawnTrapSecs{ spawnTrapSecs }, dir{ direction }, distance{ distance }, type{ "BoulderTrap" }, spawnAnimationSecs{0.1f}, spawningInProcess{false},
	spriteFrame{0, 0, 40, 40}, ready{false}
{
	this->body.setTexture(*texSpawner);
	this->body.setOrigin(20.f, 20.f);
	this->body.setPosition(pos);
	this->body.setScale(V2f(1.f, 1.f));
	if (this->dir == 'u') this->body.rotate(180.f);
	else if (this->dir == 'l') this->spriteFrame.left = 40, this->body.rotate(180.f);
	else if (this->dir == 'r') this->spriteFrame.left = 40;
	this->body.setTextureRect(this->spriteFrame);
}

void Spawner::Draw(const vector<Spawner>& spawners, RenderWindow& window)
{
	for (int i = 0; i < spawners.size(); i++)
	{
		window.draw(spawners[i].body);
	}
}

// -------------------------------------------------------------- PRIVATE FUNCTIONS -----------------------------------------------------------------------

bool Spawner::canSpawnTrap(Vector2f playerPos) // EPIC MATH.
{
	Vector2f pos{ this->body.getPosition() };
	switch (this->dir)
	{
		case 'u':
			if (playerPos.y + FOh(20.f) < pos.y && playerPos.y + FOh(49.f) >= pos.y - FOh(this->distance))
			{
				if (playerPos.x > pos.x - FOw(35.f) && playerPos.x - FOw(20.f) < pos.x) return true;
			}
			break;
		case 'd':
			if (playerPos.y - FOh(20.f) > pos.y && playerPos.y - FOh(20.f) <= pos.y + FOh(this->distance))
			{
				if (playerPos.x > pos.x - FOw(35.f) && playerPos.x - FOw(20.f) < pos.x) return true;
			}
			break;
		case 'l':
			if (playerPos.x + FOw(20.f) < pos.x && playerPos.x + FOw(35.f) >= pos.x - FOw(this->distance))
			{
				if (playerPos.y > pos.y - FOh(49.f) && playerPos.y - FOh(20.f) < pos.y) return true;
			}
			break;
		case 'r':
			if (playerPos.x - FOw(20.f) > pos.x && playerPos.x - FOw(20.f) <= pos.x + FOw(this->distance))
			{
				if (playerPos.y > pos.y - FOh(49.f) && playerPos.y - FOh(20.f) < pos.y) return true;
			}
	}
	return false;
}