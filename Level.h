#pragma once
#include "Collectible.h"
#include "UI_ftt.h"
#include "Spawner.h"
#include "Trap_Movable.h"
#include "SpikeTrap.h"
#include "SpearTrap.h"
#include "BoulderTrap.h"

struct Traps
{
	vector<SpikeTrap*> spikes;
	vector<SpearTrap*> spears;
	vector<BoulderTrap*> boulders;
};

class Level
{
public:
	Level();
	void play(RenderWindow& window, G1_Player& player, bool& MainLoop, TexturesMap& Texs, SoundsMap& Audio, UI_ftt& UI);
	~Level();
private:
	vector<Obstacle*> obstacles;
	vector<Collectible> collectibles;
	Traps traps;
	deque<SoundPlayer> sounds;
	vector<Spawner> spawners;
	string newSound; // What sounds need to play.
	int level;

	void drawTraps(RenderWindow& window, const Traps& trap); // Draw all traps.
	void intersectsTrap(G1_Player& player, const Traps& traps); // Player gets damage if he intersects with any traps.
	void trapsLogic(const G1_Player& player, Time dt);
	void soundHandler(SoundsMap& Audio);
	void spawnerLogic(TexturesMap& Texs, Vector2f playerPos);

	template <class T> void ifCanSpawnTrap(T* trap, Vector2f playerPos, int which, TexturesMap& Texs)
	{
		trap = this->spawners[which].spawnTrap(&Texs[this->spawners[which].getType()], 1.f, playerPos, trap, which);
		if (trap != nullptr)
		{
			if (trap->getType() == "BoulderTrap") this->traps.boulders.push_back(trap);
		}
	}
};