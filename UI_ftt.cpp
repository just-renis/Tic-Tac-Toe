#include "UI_ftt.h"

void UI_ftt::AddElement(const Text& text, bool drawIt, const string& string)
{
	this->Texts[string] = make_pair(drawIt, text);
}

void UI_ftt::AddElement(const RectangleShape& rect, bool drawIt, const string& string)
{
	this->Rects[string] = make_pair(drawIt, rect);
}

void UI_ftt::AddElement(Texture* tex, Vector2f pos, bool drawIt, const string& string)
{
	Sprite spr{ *tex };
	spr.setPosition(pos);
	spr.setScale(V2f(1.f, 1.f));
	this->Sprites[string] = make_pair(drawIt, spr);
}

void UI_ftt::Draw(RenderWindow& window, UI_ftt& UI, const G1_Player& player)
{
	UI.gettingDamage(static_cast<float>(player.getHP()));
	UI.updateHealthUI(player.getPotions(), player.getReborns());
	UI.rebornShine();
	for (unordered_map<string, pair<bool, RectangleShape>>::iterator it = UI.Rects.begin(); it != UI.Rects.end(); it++)
	{
		if (it->second.first) window.draw(it->second.second);
	}
	for (unordered_map<string, pair<bool, Text>>::iterator it = UI.Texts.begin(); it != UI.Texts.end(); it++)
	{
		if (it->second.first) window.draw(it->second.second);
	}
	for (unordered_map<string, pair<bool, Sprite>>::iterator it = UI.Sprites.begin(); it != UI.Sprites.end(); it++)
	{
		if (it->second.first) window.draw(it->second.second);
	}
}

// -------------------------------------------------------------------------- PRIVATE FUNCTIONS ----------------------------------------------------------------------
void UI_ftt::gettingDamage(float hp)
{
	hp /= (1000.f / (WINDOW_WIDTH / 2.f));
	this->Rects["HealthBar"].second.setSize(Vector2f{ hp, this->Rects["HealthBar"].second.getSize().y });
}

void UI_ftt::updateHealthUI(int potions, int reborns)
{
	this->Texts["Potion"].second.setString("X" + to_string(potions));
	if (reborns > 0)
	{
		this->Texts["Reborn"].second.setString("REBORN X" + to_string(reborns));
		this->Texts["Reborn"].first = true;
	}
	else this->Texts["Reborn"].first = false;
}

void UI_ftt::rebornShine()
{
	if (this->Texts["Reborn"].first)
	{
		if (this->rebornShineTimer.getElapsedTime().asSeconds() >= 0.2f)
		{
			(this->Texts["Reborn"].second.getFillColor() == Color::Black) ? this->Texts["Reborn"].second.setFillColor(Color::White),
				this->Texts["Reborn"].second.setOutlineColor(Color::Black) : this->Texts["Reborn"].second.setFillColor(Color::Black),
				this->Texts["Reborn"].second.setOutlineColor(Color::White);
			this->rebornShineTimer.restart();
		}
	}
}