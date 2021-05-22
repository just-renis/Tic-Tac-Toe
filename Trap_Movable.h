#pragma once
#include "VoidFunctions.h"
#include "Trap.h"

class Trap_Movable : public Trap
{
public:
	Trap_Movable(const string& type, Texture* tex, Vector2f pos, float frameSecs, const IntRect& img,
		int damage, bool dmgAlways, char dir, float speed, bool isActivated);
	virtual void Logic(Vector2f playerPos, string& NewSound, Time dt, bool defaultMove);
protected:
	void move(Time dt);
	bool isActivated;
	char dir;
private:
	float speed;
	bool addFirstSound;
	Clock trapSoundTime;
	float trapSoundSecs{};
};