#include "Collectible.h"

Collectible::Collectible(const string& type, Texture* tex, Vector2f pos)
	: type{type}
{
	this->body.setTexture(*tex);
	this->body.setTextureRect(IntRect(0, 0, 20, 20));
	this->body.setScale(V2f(1.f, 1.f));
	this->FrameTime = 0.1f;
	this->body.setPosition(pos);
	if (this->type == "coin") this->frameEnd = 140;
	else this->frameEnd = 100;
}

void Collectible::Draw(RenderWindow& window, vector<Collectible>& collectibles)
{
	for (int i = 0; i < collectibles.size(); i++)
	{
		window.draw(collectibles[i].body);
		collectibles[i].Animation();
	}
}

void Collectible::intersects(const Sprite& player, vector<Collectible>& collectibles, string& collectedItem, string& NewSound)
{
	for (int i = 0; i < collectibles.size(); i++)
	{
		if (Collision::PixelPerfectTest(player, collectibles[i].body))
		{
			if (collectibles[i].type == "coin") NewSound += "CollectCoin ";
			else NewSound += "CollectHeart ", collectedItem = "Heart";
			collectibles.erase(collectibles.begin() + i);
			break;
		}
	}
}

// ---------------------------------------------------------------------- PRIVATE FUNCTIONS ----------------------------------------------------------------------------------------
void Collectible::Animation()
{
	if (this->animationTimer.getElapsedTime().asSeconds() >= this->FrameTime)
	{
		if (this->body.getTextureRect().left >= this->frameEnd) this->body.setTextureRect(IntRect(0, 0, 20, 20));
		else this->body.setTextureRect(IntRect(this->body.getTextureRect().left + 20, 0, 20, 20));
		this->animationTimer.restart();
	}
}