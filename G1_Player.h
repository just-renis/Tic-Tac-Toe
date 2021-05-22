#pragma once
#include "Obstacle.h"
#include "Collectible.h"

class G1_Player
{
public:
	G1_Player(Texture& playerTex, const SoundBuffer& walkingSound);
	void drawPlayer(RenderWindow& window);
	void movement(char Dir, const vector<Obstacle*>& obstacles);
	void idle();
	void walking();
	bool checkMovement();
	void collectedItem(string& collectible);
	void usePotion(string& newSound);
	void getDamage(int damage, string& newSound);

	// ------------------------------------------------------------------------ GETTERS --------------------------------------------------------------------------
	const Sprite getBody() const { return this->body; }
	const int getHP() const { return this->hp; }
	const int getPotions() const { return this->potions; }
	const int getReborns() const { return this->reborns; }
private:
	Sprite body;
	int hp, maxHp, potions, reborns, hits;
	char previousDirection;
	bool isMoving, isAlive;
	Sound walkingS;
	SoundBuffer walkingB;
	Clock movementClock, hurtClock;
	void movementDerivative(float x, float y, char dir, int leftEnd, int leftStart, int sprWidth, const vector<Obstacle*>& obstacles);
	void reborn(string& NewSound);
};