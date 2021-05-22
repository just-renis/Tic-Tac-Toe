#include "Trap_nStop.h"

Trap_nStop::Trap_nStop(const string& type, Texture* tex, Vector2f pos, float frameSecs, float cooldownSecs, int frameStop, const IntRect& img, int damage, bool DmgAlways)
	: Trap{ type, tex, pos, frameSecs, img, damage, DmgAlways}, cooldownSecs{ cooldownSecs }, wait{ false }, firstActivation{ true }, frameStop{frameStop}
{

}

void Trap_nStop::waitFunction()
{
	if (this->animationTimer.getElapsedTime().asSeconds() > this->cooldownSecs)
	{
		this->wait = false;
		this->ChangingFrame(this->body.getTextureRect().left);
		this->animationTimer.restart();
		this->firstActivation = false;
	}
}

void Trap_nStop::Logic(Vector2f playerPos, string& NewSound)
{
	if (this->wait) waitFunction();
	else if (TimerToChangeFrame())
	{
		int AtFrame = this->spriteFrame.left; // At what frame the trap is.
		if (this->CanAttack(playerPos)) // If trap is in range of player.
		{
			if (AtFrame == this->frameStop || !this->firstActivation && AtFrame == 0)
			{
				NewSound += this->AddingNewSound();
				this->animationTimer.restart();
				this->wait = true;
			}
			else this->ChangingFrame(AtFrame);
		}
		else if (AtFrame > 0) // Reset trap when player is out of trap area.
		{
			this->spriteFrame.left = 0;
			this->body.setTextureRect(this->spriteFrame); // Trap goes to his starting frame.
			NewSound += this->AddingNewSound(); // Sound of deactivating trap.
			this->firstActivation = true;
			this->animationTimer.restart();
		}
		else this->animationTimer.restart(); // Reset animation timer.
	}
}