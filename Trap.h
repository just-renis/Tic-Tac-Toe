#pragma once
#include "VoidFunctions.h"
#include "Obstacle.h"

class Trap
{
public:
	Trap(const string& type, Texture* tex, Vector2f pos, float frameSecs, const IntRect& img, int damage, bool dmgAlways);
	virtual void Logic(Vector2f playerPos, string& NewSound);
	const string& getType() const { return this->type; }
	const IntRect& getSpriteFrame() const { return this->spriteFrame; }

	// ------------------------------------------------------------ TEMPLATE FUNCTIONS ---------------------------------------------------------------
	template<class T>
	static int intersects(const Sprite& player, const vector<T*>& traps)
	{
		int TotalDamage = 0;
		for (int i = 0; i < traps.size(); i++)
		{
			if (!traps[i]->damageOff)
			{
				if (traps[i]->body.getTextureRect().left > 0 || traps[i]->damageAlways)
				{
					if (Collision::PixelPerfectTest(player, traps[i]->body)) TotalDamage += traps[i]->damage;
				}
			}
		}
		return TotalDamage;
	}
	template<class T>
	static void Draw(RenderWindow& window, const vector<T*>& traps, string& newSound)
	{
		for (int i = 0; i < traps.size(); i++)
		{
			window.draw(traps[i]->body);
		}
	}
protected:
	string type;
	int damage;
	Sprite body;
	int frameEnd, frameWidth, frameHeight, frameTop; // FRAME INFO.
	float frameSecs; // Seconds for frames.
	Clock animationTimer;
	bool damageAlways, damageOff;
	IntRect spriteFrame;

	// ---------------------------------------------------- FUNCTIONS -----------------------------------------------------------
	bool TimerToChangeFrame();
	bool CanAttack(Vector2f playerPos);
	void ChangingFrame(int AtFrame);
	string AddingNewSound();
	const FloatRect getBounds() const { return this->body.getGlobalBounds(); }
private:
	float leftX, upY, rightX, downY; // ACTIVATION DISTANCES.
};