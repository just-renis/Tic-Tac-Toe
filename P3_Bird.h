#pragma once
#include "Program.h"

class P3_Bird
{
public:
	P3_Bird(Font* font);
	void eventJump();
	static void draw(RenderWindow& window, const vector<P3_Bird*> birds);
	static void update(vector<P3_Bird*>& birds);

	const FloatRect& getBounds() const { return this->body.getGlobalBounds(); }
	const void addScore() { this->score++; }
private:
	bool jumped;
	RectangleShape body;
	Clock jumpTime;
	int score;
	Text scoreText;
	void fall();
	void jump();
	bool mapBounds();
	void textUpdate();
};