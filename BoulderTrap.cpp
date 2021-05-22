#include "BoulderTrap.h"

int BoulderTrap::IDcounter = 0;

BoulderTrap::BoulderTrap(Texture* tex, Vector2f pos, char dir, float speed, int whichSpawner)
	: Trap_Movable{ "BoulderTrap", tex, pos, 0.1f, IntRect{120, 40, 0, 40}, 8, true, dir, speed, true}, crashed{false}, crashedSound{false}, inSpawner{true},
	whichSpawner{whichSpawner}, ID{"B_" + to_string(BoulderTrap::IDcounter++)}
{
}

void BoulderTrap::gettingOutOfSpawner(int spawnerFrame)
{
	if (spawnerFrame >= 320)
	{
		this->frameWidth = 40;
		switch (this->dir)
		{
			case 'l':
				this->spriteFrame.width += 5;
				if (this->spriteFrame.width == 40) this->inSpawner = false;
				break;
			case 'r':
				this->spriteFrame.width += 5;
				this->spriteFrame.left -= 5;
				if (this->spriteFrame.left > 120) this->spriteFrame.left -= 120;
				if (this->spriteFrame.width == 40) this->inSpawner = false;
				break;
			case 'u':
				this->spriteFrame.height += 5;
				if (this->spriteFrame.height == 40) this->inSpawner = false;
				break;
			default:
				this->spriteFrame.height += 5;
				this->spriteFrame.top -= 5;
				if (this->spriteFrame.height == 40) this->inSpawner = false;
		}
	}
}