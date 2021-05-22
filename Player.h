#pragma once
#include "VoidFunctions.h"

class Player
{
public:
	string ID;
	RectangleShape body;
	int Score;
	float Speed;
	Text ScoreText, YouText;
	Color BodyColor;
	bool PlayerMoved;
	Player(Font* font, string id, Color BodyColor);
	bool StillOn(float Secs, int Which);
	void ResetTime(int Which);
	void SetBodyColor();
	void UpMovement();
	void DownMovement();
	void RightMovement();
	void LeftMovement();
private:
	vector<Clock> PowTimes;
};