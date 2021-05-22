#pragma once
#include "VoidFunctions.h"
#include "UI.h"

void StartingLoop(RenderWindow &window, Event &e, int &mainState, int &depthState, TexturesMap& texs, Font* font)
{
	View view(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	window.setView(view);
	srand(static_cast<unsigned> (time(0)));
	bool startingMenuLoop = true, newBackgroundColorNeeded = true;
	Vector2f mousePos;
	Color backgroundColor = Color::Black, next;
	Clock backgroundTimer;

	UI UI;

	// ---------------------------------------------------------------- SETTING UP SPRITES --------------------------------------------------------------
	vector<Sprite> gameSprites, programSprites;
	for (int i = 0; i < 2; i++) programSprites.push_back(Sprite());
	for (int i = 0; i < programSprites.size(); i++) programSprites[i].setTexture(texs["program1"]);
	programSprites[0].setPosition(V2f(0.f, 40.f));
	programSprites[1].setPosition(V2f(155.f, 40.f));
	for (int i = 0; i < programSprites.size(); i++) programSprites[i].setScale(V2f(1.f, 1.f));
	for (int i = 0; i < 4; i++) gameSprites.push_back(Sprite());
	gameSprites[0].setTexture(texs["game1"]);
	gameSprites[1].setTexture(texs["game2"]);
	gameSprites[2].setTexture(texs["game3"]);
	gameSprites[3].setTexture(texs["game4"]);
	for (int i = 0; i < gameSprites.size(); i++) gameSprites[i].setScale(V2f(1.f, 1.f));
	gameSprites[0].setPosition(V2f(0.f, 40.f));
	gameSprites[1].setPosition(V2f(0.f, 230.f));
	gameSprites[2].setPosition(V2f(155.f, 230.f));
	gameSprites[3].setPosition(V2f(310.f, 230.f));

	// -------------------------------------------- STATE 0 STUFF ---------------------------------------
	UI.Texts.push_back(TextSetter(font, FOw(100), FOw(4.f), Color::Yellow, Color::White, "GAMES", V2f(790.f, 340.f)));
	UI.Texts.push_back(TextSetter(font, FOw(100), FOw(4.f), Color::Green, Color::White, "PROGRAMS", V2f(700.f, 490.f)));
	UI.Texts.push_back(TextSetter(font, FOw(40), FOw(2.f), Color::Black, Color::White, "TWO PLAYER GAMES", V2f(755.f, -5.f)));
	UI.Texts.push_back(TextSetter(font, FOw(40), FOw(2.f), Color::Black, Color::White, "SINGLE PLAYER GAMES", V2f(730.f, 185.f)));
	UI.Texts.push_back(TextSetter(font, FOw(40), FOw(2.f), Color::Black, Color::White, "PROGRAMS", V2f(860.f, -5.f)));

	// -------------------------------------------- STATE 1 STUFF ----------------------------------------
	
	while (window.isOpen() && startingMenuLoop)
	{
		mousePos = (Vector2f(Mouse::getPosition(window)));
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Escape))
			{
				if (depthState == 1 || depthState == 2) depthState = 0;
				else window.close();
			}

			switch (depthState)
			{
				case 0: // <------------------------------------------------- STATE 0 ----------------------------------------------------
				if (e.type == Event::MouseButtonPressed)
				{
					if (Mouse::isButtonPressed(Mouse::Left))
					{
						for (int i = 0; i < 2; i++)
						{
							if (UI.Texts[i].getGlobalBounds().contains(mousePos)) depthState = i + 1;
						}
					}
				}
				break;
				case 1:  // <------------------------------------------------- STATE 1 --------------------------------------------------
				if (e.type == Event::MouseButtonPressed)
				{
					if (Mouse::isButtonPressed(Mouse::Left))
					{
						for (int i = 0; i < gameSprites.size(); i++)
						{
							if (gameSprites[i].getGlobalBounds().contains(mousePos))
							{
								mainState = i + 1;
								startingMenuLoop = false;
							}
						}
					}
				}
				break;
				case 2: // <------------------------------------------------- STATE 2 --------------------------------------------------
				if (e.type == Event::MouseButtonPressed)
				{
					if (Mouse::isButtonPressed(Mouse::Left))
					{
						for (int i = 0; i < programSprites.size(); i++)
						{
							if (programSprites[i].getGlobalBounds().contains(mousePos))
							{
								mainState = static_cast<int>(gameSprites.size()) + 1 + i;
								startingMenuLoop = false;
							}
						}
					}
				}
			}
		}

		// -------------------------------------------- BACKGROUND COLOR CHANGER ---------------------------------------
		backgroundColorChanger(backgroundColor, next, backgroundTimer, newBackgroundColorNeeded);

		// --------------------------------------------------------- DRAW ----------------------------------------------------------
		window.clear(backgroundColor);

		switch (depthState)
		{
			case 0: // <------------------------------------------------- STATE 0 ------------------------------------------------
				for (int i = 0; i < 2; i++) window.draw(UI.Texts[i]);
				break;
			case 1: // <------------------------------------------------- STATE 1 --------------------------------------------------
				for (int i = 0; i < gameSprites.size(); i++) window.draw(gameSprites[i]);
				for (int i = 2; i < UI.Texts.size() - 1; i++) window.draw(UI.Texts[i]);
				break;
			case 2: // <------------------------------------------------- STATE 2 --------------------------------------------------
				for (int i = 0; i < programSprites.size(); i++) window.draw(programSprites[i]);
				window.draw(UI.Texts[4]);
		}
		window.display();
	}
}