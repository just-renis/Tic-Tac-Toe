#pragma once
#include "Level.h"

void InitUI(UI_ftt& UI, TexturesMap& Texs, Font* font);

void Main4Loop(RenderWindow& window, Event& e, int& MainState, int& DepthState, Font* font, TexturesMap& Texs, SoundsMap& Audio)
{
	int State = 0;
	bool MainLoop = true;
	UI_ftt UI;
	InitUI(UI, Texs, font);
	G1_Player player(Texs["Player4"], Audio["Walking"]);
	Texs["Wall1"].setRepeated(true);
	Texs["WallBackground"].setRepeated(true);
	window.setKeyRepeatEnabled(true);
	Level level;
	level.play(window, player, MainLoop, Texs, Audio, UI);
	MainState = 0;
	DepthState = 1;

	while (window.isOpen() && MainLoop)
	{
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Escape)
				{
					MainState = 0;
					DepthState = 1;
					MainLoop = false;
				}
			}
		}
		window.clear(Color::Black);
		window.display();
	}
}

void InitUI(UI_ftt& UI, TexturesMap& Texs, Font* font)
{
	// ------------------------------------------------------------- RECTANGLES ---------------------------------------------------------------------------
	UI.AddElement(RectangleSetter(V2f(960.f, 30.f), V2f(480.f, 1045.f), Color::White, Color::Black, FOw(2.f)), true, "HealthBarFrame"); // Health bar frame.
	UI.AddElement(RectangleSetter(V2f(960.f, 30.f), V2f(480.f, 1045.f), Color::Red, Color::Transparent, 0.f), true, "HealthBar"); // Health bar.

	// -------------------------------------------------------------- SPRITES -----------------------------------------------------------------------------
	UI.AddElement(&Texs["HeartUI"], V2f(1450.f, 1040.f), true, "HeartUI"); // Health heart.
	UI.AddElement(&Texs["PotionUI"], V2f(390.f, 1040.f), true, "HealthPotion"); // Health potion.

	// ------------------------------------------------------------- TEXTS ---------------------------------------------------------------------------
	UI.AddElement(TextSetter(font, FOw(35), FOw(4.f), Color::White, Color::Black, "X0", V2f(430.f, 1040.f)), true, "Potion"); // Potion text.
	UI.AddElement(TextSetter(font, FOw(35), FOw(4.f), Color::White, Color::Black, "REBORN X", V2f(50.f, 1040.f)), false, "Reborn"); // Reborn text.
}