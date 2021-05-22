#pragma once
#include "Trap.h"

class Trap_nStop : public Trap
{
public:
	Trap_nStop(const string& type, Texture* tex, Vector2f pos, float frameSecs, float cooldownSecs, int frameStop, const IntRect& img, int damage, bool DmgAlways);
	virtual void Logic(Vector2f playerPos, string& NewSound);
protected:
	bool wait, firstActivation;
	float cooldownSecs;
	int frameStop;
	void waitFunction();
};