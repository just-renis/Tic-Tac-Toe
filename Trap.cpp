#include "Trap.h"

Trap::Trap(const string& type, Texture* tex, Vector2f pos, float frameSecs, const IntRect& img, int damage, bool dmgAlways)
	: type{ type }, frameSecs{ frameSecs }, leftX{FOw(56.f)}, upY{FOh(64.f)}, rightX{FOw(80.f)}, downY{FOh(75.f)},
	frameEnd{img.left}, frameTop{ img.top }, frameWidth{ img.width }, frameHeight{img.height}, damage{damage}, damageAlways{dmgAlways}, damageOff{false}
{
	this->body.setTexture(*tex);
	this->body.setPosition(pos);
	this->spriteFrame = IntRect{0, this->frameTop, this->frameWidth, this->frameHeight};
	this->body.setTextureRect(this->spriteFrame);
	this->body.setScale(V2f(1.f, 1.f));
}

void Trap::Logic(Vector2f playerPos, string& NewSound)
{
	if (TimerToChangeFrame())
	{
		int AtFrame{ this->spriteFrame.left }; // At what frame the trap is.
		if (this->CanAttack(playerPos)) // If trap is in range of player.
		{
			if (AtFrame == this->frameEnd || AtFrame == 0) NewSound += this->AddingNewSound();
			this->ChangingFrame(AtFrame);
			this->animationTimer.restart();
		}
		else if (AtFrame > 0) // Reset trap when player is out of trap area.
		{
			this->spriteFrame.left = 0;
			this->body.setTextureRect(this->spriteFrame); // Trap goes to his starting frame.
			NewSound += this->AddingNewSound(); // Sound of deactivating trap.
			this->animationTimer.restart();
		}
		else this->animationTimer.restart(); // Reset animation timer.
	}
}

// ---------------------------------------------------------------------- PRIVATE FUNCTIONS ------------------------------------------------------------------------------
bool Trap::CanAttack(Vector2f playerPos)
{
	Vector2f TrapPos = this->body.getPosition();
	if (TrapPos.x <= playerPos.x && TrapPos.x >= playerPos.x - this->rightX || TrapPos.x >= playerPos.x && TrapPos.x <= playerPos.x + this->leftX) // DISTANCE TILL ACTIVATION.
	{
		if (TrapPos.y <= playerPos.y && TrapPos.y >= playerPos.y - this->downY || TrapPos.y >= playerPos.y && TrapPos.y <= playerPos.y + this->upY) return true; // DISTANCE TILL ACTIVATION.
	}
	return false;
}

void Trap::ChangingFrame(int AtFrame)
{
	if (AtFrame >= this->frameEnd)
	{
		this->spriteFrame.left = 0;
		this->body.setTextureRect(this->spriteFrame); // Last frame reached, reset it.
	}
	else
	{
		this->spriteFrame.left += this->frameWidth;
		this->body.setTextureRect(this->spriteFrame);
	}
	this->animationTimer.restart();
}

bool Trap::TimerToChangeFrame()
{
	if (this->animationTimer.getElapsedTime().asSeconds() >= this->frameSecs) return true;
	return false;
}

string Trap::AddingNewSound()
{
	return this->type + " ";
}