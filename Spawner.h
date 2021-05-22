#pragma once
#include "VoidFunctions.h"
#include "BoulderTrap.h"

class Spawner
{
public:
	Spawner(Texture* texSpawner, Vector2f pos, float spawnTrapSecs, char direction, float distance);
	static void Draw(const vector<Spawner>& spawners, RenderWindow& window);
	const string& getType() const { return this->type; }
	const Sprite& getBody() const { return this->body; }
	const int getSpawnerFrame() const { return this->spriteFrame.left; }
	const bool isReady() const { return this->ready; }

	// --------------------------------------------------------------------- TEMPLATE FUNCTIONS -------------------------------------------------------
	template <class T> T* spawnTrap(Texture* tex, float speed, Vector2f playerPos, const T* t, int whichSpawner)
	{
		if (this->spawningInProcess)
		{
			if (this->spawnTrapTimer.getElapsedTime().asSeconds() >= this->spawnAnimationSecs)
			{
				this->spriteFrame.left += 40;
				this->ready = true;
				if (this->spriteFrame.left == 640) // should be 600 but thanks to shit boulder its 640.
				{
					this->spawningInProcess = false;
					if (this->dir == 'l' || this->dir == 'r') this->spriteFrame.left = 40;
					else this->spriteFrame.left = 0;
					this->spriteFrame.top = 0;
				}
				if (this->spriteFrame.left != 600) this->body.setTextureRect(this->spriteFrame); // just a shit if.
				this->spawnTrapTimer.restart();
			}
			else this->ready = false;
		}
		else if (this->spawnTrapTimer.getElapsedTime().asSeconds() >= this->spawnTrapSecs)
		{
			if (canSpawnTrap(playerPos))
			{
				this->spawnTrapTimer.restart();
				this->spawningInProcess = true;
				if (this->type == "BoulderTrap")
				{
					this->ready = false;
					if (this->dir == 'l' || this->dir == 'r') this->spriteFrame.top = 40;
					else this->spriteFrame.top = 80;
					this->body.setTextureRect(this->spriteFrame);
					if (this->dir == 'r') return new BoulderTrap{ tex, Vector2f{this->body.getPosition().x + FOw(20.f), this->body.getPosition().y - FOh(20.f)}, this->dir, speed, whichSpawner };
					else if (this->dir == 'l') return new BoulderTrap{ tex, Vector2f{this->body.getPosition().x + FOw(10.f), this->body.getPosition().y - FOh(20.f)}, this->dir, speed, whichSpawner };
					else if (this->dir == 'u') return new BoulderTrap{ tex, Vector2f{this->body.getPosition().x - FOw(20.f), this->body.getPosition().y + FOh(15.f)}, this->dir, speed, whichSpawner };
					return new BoulderTrap{ tex, Vector2f{this->body.getPosition().x - FOw(20.f), this->body.getPosition().y + FOh(20.f)}, this->dir, speed, whichSpawner };
				}
			}
		}
		return nullptr;
	}
private:
	Sprite body;
	Clock spawnTrapTimer;
	float spawnTrapSecs;
	string type;
	char dir;
	float distance; // Distance from spawner to player to spawn a trap.
	float spawnAnimationSecs;
	bool spawningInProcess;
	IntRect spriteFrame;
	bool ready; // That boulder can add +5 left or +5 height frame over boulder spawning frame change.
	bool canSpawnTrap(Vector2f playerPos); // Can spawner spawn a trap?.
};