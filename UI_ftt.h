#pragma once
#include "VoidFunctions.h"
#include "G1_Player.h"

class UI_ftt
{
public:
	void AddElement(const Text& text, bool drawIt, const string& string);
	void AddElement(const RectangleShape& rect, bool drawIt, const string& string);
	void AddElement(Texture* tex, Vector2f pos, bool drawIt, const string& string);
	static void Draw(RenderWindow& window, UI_ftt& UI, const G1_Player& player);
private:
	unordered_map<string, pair<bool, Text>> Texts;
	unordered_map<string, pair<bool, RectangleShape>> Rects;
	unordered_map<string, pair<bool, Sprite>> Sprites;
	Clock rebornShineTimer;
	void gettingDamage(float hp);
	void updateHealthUI(int potions, int reborns);
	void rebornShine();
};