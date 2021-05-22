#include "Trap_Movable.h"

Trap_Movable::Trap_Movable(const string& type, Texture* tex, Vector2f pos, float frameSecs, const IntRect& img, int damage,
	bool dmgAlways, char dir, float speed, bool isActivated)
	: Trap{ type, tex, pos, frameSecs, img, damage, dmgAlways}, dir{ dir }, speed{ speed }, isActivated{isActivated}
{
	if (this->type == "BoulderTrap")
	{
		this->trapSoundSecs = 2.83f;
		this->addFirstSound = true;
		if (this->dir == 'u' || this->dir == 'd')
		{
			this->spriteFrame.left = 0;
			this->spriteFrame.height = 0;
			this->spriteFrame.width = 40;
			if (this->dir == 'd') this->spriteFrame.top = 80;
			else this->spriteFrame.top = 40;
		}
		else this->spriteFrame.left = 40, this->spriteFrame.top = 0;
	}
	this->body.setTextureRect(this->spriteFrame);
}

void Trap_Movable::Logic(Vector2f playerPos, string& NewSound, Time dt, bool defaultMove)
{
	if (!this->isActivated)
	{
		if (this->CanAttack(playerPos)) this->isActivated = true;
	}
	else if (TimerToChangeFrame())
	{
		int AtFrame = this->spriteFrame.left; // At what frame the trap is.
		if (this->addFirstSound) NewSound += this->AddingNewSound(), this->addFirstSound = false; // For adding first sound before timer.
		else if (this->trapSoundTime.getElapsedTime().asSeconds() > this->trapSoundSecs) NewSound += this->AddingNewSound(), this->trapSoundTime.restart();
		this->ChangingFrame(AtFrame);
		this->animationTimer.restart(); // Reset animation timer.
	}
	if (!defaultMove && this->isActivated) this->move(dt);
	else
	{
		if (this->dir == 'l') this->body.move(V2f(-5.f, 0.f));
		else if (this->dir == 'u') this->body.move(V2f(0.f, -5.f));
	}
}

void Trap_Movable::move(Time dt)
{
	switch (this->dir)
	{
		case 'l': this->body.move(Vector2f(FOw(-speed) * dt.asSeconds() * 90.f, 0.f)); break;
		case 'r': this->body.move(Vector2f(FOw(speed) * dt.asSeconds() * 90.f, 0.f)); break;
		case 'u': this->body.move(Vector2f(0.f, FOh(-speed) * dt.asSeconds() * 90.f)); break;
		case 'd': this->body.move(Vector2f(0.f, FOh(speed) * dt.asSeconds() * 90.f)); break;
	}
}