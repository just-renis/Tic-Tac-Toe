#include "G1_Player.h"

G1_Player::G1_Player(Texture& player, const SoundBuffer& walkingSound)
	: maxHp{1000}, hp{1000}, potions{0}, reborns{0}, previousDirection{'s'}, isMoving{false}, walkingB{walkingSound}, isAlive{true}, hits{0}
{
	this->body.setTexture(player);
	this->body.setScale(V2f(1.f, 1.f));
	this->body.setTextureRect(IntRect{ 0, 0, 16, 29 });
	this->walkingS.setBuffer(this->walkingB);
	this->walkingS.setLoop(true);
}

void G1_Player::drawPlayer(RenderWindow& window)
{
	window.draw(this->body);
}

void G1_Player::movement(char dir, const vector<Obstacle*>& obstacles)
{
	this->isMoving = true;
	if (dir == 'w') movementDerivative(0.f, -5.f, 'w', 80, 48, 16, obstacles);
	else if (dir == 'a') movementDerivative(-5.f, 0.f, 'a', 166, 138, 14, obstacles);
	else if (dir == 's') movementDerivative(0.f, 5.f, 's', 32, 0, 16, obstacles);
	else movementDerivative(5.f, 0.f, 'd', 124, 96, 14, obstacles);
	if (this->isMoving)
	{
		this->previousDirection = dir;
		walking();
		this->isMoving = false;
	}
	else idle();
}

void G1_Player::idle()
{
	if (!this->isMoving)
	{
		this->walkingS.pause();
		if (this->previousDirection == 'd') this->body.setTextureRect(IntRect{ 96, 0, 14, 29 });
		else if (this->previousDirection == 'a') this->body.setTextureRect(IntRect{ 138, 0, 14, 29 });
		else if (this->previousDirection == 's') this->body.setTextureRect(IntRect{0, 0, 16, 29});
		else this->body.setTextureRect(IntRect{ 48, 0, 16, 29 });
	}
}

void G1_Player::walking()
{
	if (!(this->walkingS.getStatus() == Sound::Playing)) this->walkingS.play();
	this->movementClock.restart();
}

bool G1_Player::checkMovement()
{
	if (this->movementClock.getElapsedTime().asSeconds() >= 0.04f) return true;
	return false;
}

void G1_Player::collectedItem(string& collectible)
{
	if (collectible == "Heart")
	{
		(this->maxHp - this->hp >= 250) ? this->hp += 250 : this->potions++;
		if (this->potions == 3)
		{
			this->reborns++;
			this->potions = 0;
		}
		collectible.clear();
	}
}

void G1_Player::usePotion(string& newSound)
{
	if (this->potions > 0)
	{
		newSound += "DrinkPotion ";
		this->potions--;
		this->hp += 250;
		if (this->hp > this->maxHp) this->hp = this->maxHp;
	}
}

void G1_Player::getDamage(int damage, string& newSound)
{
	if (this->isAlive)
	{
		this->hp -= damage;
		if (this->hp < 0)
		{
			this->hp = 0;
			reborn(newSound);
		}
		else if (damage > 0)
		{
			hits++;
			if (hurtClock.getElapsedTime().asSeconds() >= 0.3f)
			{
				newSound += "PlayerHurt ";
				hurtClock.restart();
			}
		}
	}
}

// -------------------------------------------------------------------------- PRIVATE FUNCTIONS --------------------------------------------------------------------------

void G1_Player::movementDerivative(float x, float y, char dir, int leftEnd, int leftStart, int sprWidth, const vector<Obstacle*>& obstacles)
{
	this->body.move(V2f(x, y));
	this->isMoving = Obstacle::intersects(this->body, obstacles);
	if (this->isMoving)
	{
		if (this->previousDirection == dir)
		{
			if (this->body.getTextureRect().left >= leftEnd) this->body.setTextureRect(IntRect{leftStart, 0, sprWidth, 29});
			else this->body.setTextureRect(IntRect{ this->body.getTextureRect().left + sprWidth, 0, sprWidth, 29 });
		}
		else
		{
			this->body.setTextureRect(IntRect{ leftStart, 0, sprWidth, 29 });
			if (dir == 'w' || dir == 's')
			{
				if (this->previousDirection == 'd') this->body.move(-FOw(2.f), 0.f );
			}
		}
	}
	else this->body.move(-V2f(x, y));
}

void G1_Player::reborn(string& newSound)
{
	if (this->reborns > 0)
	{
		this->reborns--;
		this->hp = this->maxHp;
		newSound += "Reborn ";
	}
	else newSound += "Death ", this->isAlive = false;
}