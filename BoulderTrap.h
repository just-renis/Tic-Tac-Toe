#pragma once
#include "Trap_Movable.h"

class BoulderTrap : public Trap_Movable
{
public:
	BoulderTrap(Texture* tex, Vector2f pos, char dir, float speed, int whichSpawner);
	const bool isCrashed() const { return crashed; }
	const bool isCrashedSound() const { return crashedSound; }
	const bool isInSpawner() const { return inSpawner; }
	const int whichSpawnerFrom() const { return whichSpawner; }
	void setCrashedSoundFalse() { crashedSound = true; }
	void gettingOutOfSpawner(int spawnerFrame);
	const string& getID() const { return this->ID; }

	// ------------------------------------------------------------ TEMPLATE FUNCTIONS ---------------------------------------------------------------------
	template <class T> static void BouldersLogic(vector<BoulderTrap*>& boulders, const T& objects)
	{
		for (int i = 0; i < boulders.size(); i++)
		{
			if (!boulders[i]->crashed)
			{
				boulders[i]->collision(boulders[i], objects);
				Vector2f boulderPos{ boulders[i]->body.getPosition() };
				if (boulderPos.x <= -40.f || boulderPos.x >= WINDOW_WIDTH || boulderPos.y <= -40.f || boulderPos.y >= WINDOW_HEIGHT) boulders[i]->crashed = true;
			}
			else
			{
				if (boulders[i]->crashedTimer.getElapsedTime().asSeconds() >= 0.2f)
				{
					boulders[i]->spriteFrame.left += boulders[i]->frameWidth;
					boulders[i]->body.setTextureRect(boulders[i]->spriteFrame);
					boulders[i]->crashedTimer.restart();
					if (boulders[i]->spriteFrame.left == 160)
					{
						delete boulders[i];
						boulders.erase(boulders.begin() + i);
					}
				}
			}
		}
	}
private:
	bool crashed;
	Clock crashedTimer;
	bool crashedSound;
	bool inSpawner;
	int whichSpawner;
	string ID;
	static int IDcounter;

	// ------------------------------------------------------------------- TEMPLATE FUNCTIONS ----------------------------------------------------------------
	template <class T> void collision(BoulderTrap* boulder, const T& objects)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (boulder->getID() != objects[i]->getID())
			{
				if (boulder->body.getGlobalBounds().intersects(objects[i]->getBounds()))
				{
					boulder->crashed = true;
					boulder->damageOff = true;
					boulder->spriteFrame.top = 80, boulder->spriteFrame.left = 0, boulder->spriteFrame.height = 40, boulder->spriteFrame.width = 40; // Set to crashing boulder frames.
					boulder->body.setTextureRect(boulder->spriteFrame);
					boulder->crashedTimer.restart();
				}
			}
		}
	}
};