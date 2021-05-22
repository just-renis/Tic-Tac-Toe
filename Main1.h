#pragma once
#include "SpriteHolder.h"

void InitTiles(vector<G1_Tile>& Tiles);
void Clear(vector<G1_Tile>& Tiles, bool& turn, vector<SpriteHolder>& SpriteHold, Text& Winner, bool& GAMEOVER);

void Main1Loop(RenderWindow& window, Event& e, int &MainState, int &DepthState, TexturesMap& texs, Font *font)
{
	Vector2f MousePos;
	vector<G1_Tile> Tiles;
	bool MainLoop1 = true;
	bool turn = false;
	bool GAMEOVER = false;
	InitTiles(Tiles);
	Text Winner = TextSetter(font, FOw(60), 0.f, Color::Green, Color::Transparent, "", Vector2f());
	Text Restart = TextSetter(font, FOw(30), 0.f, Color::Red, Color::Transparent, "To restart a game press R", V2f(780.f, 180.f));
	int yes = 0;

	vector<SpriteHolder> SpriteHold;
	while (window.isOpen() && MainLoop1)
	{
		MousePos = (Vector2f(Mouse::getPosition(window)));
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Escape))
			{
				MainState = 0;
				DepthState = 1;
				MainLoop1 = false;
			}
			if (e.type == Event::MouseButtonPressed && GAMEOVER != true)
			{
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					for (int i = 0; i < 9; i++)
					{
						if (Tiles[i].Rectangle.getGlobalBounds().contains(MousePos))
						{
							if (turn == false && Tiles[i].Full != true)
							{
								SpriteHold.push_back(SpriteHolder(&texs["ZeroTex"], Vector2f(Tiles[i].Rectangle.getPosition().x + WINDOW_WIDTH / 213.333333f, Tiles[i].Rectangle.getPosition().y + WINDOW_HEIGHT / 120.f)));
								turn = !turn;
								Tiles[i].Contains = 1;
							}
							else if (Tiles[i].Full != true)
							{
								SpriteHold.push_back(SpriteHolder(&texs["CrossTex"], Vector2f(Tiles[i].Rectangle.getPosition().x + WINDOW_WIDTH / 213.333333f, Tiles[i].Rectangle.getPosition().y + WINDOW_HEIGHT / 120.f)));
								turn = !turn;
								Tiles[i].Contains = 2;
							}
							Tiles[i].Full = true;
						}
					}
					if (Tiles[0].Contains == 1 && Tiles[1].Contains == 1 && Tiles[2].Contains == 1) Winner.setString("Winner: Player 1");
					else if (Tiles[0].Contains == 2 && Tiles[1].Contains == 2 && Tiles[2].Contains == 2) Winner.setString("Winner: Player 2");
					else if (Tiles[0].Contains == 1 && Tiles[3].Contains == 1 && Tiles[6].Contains == 1) Winner.setString("Winner: Player 1");
					else if (Tiles[0].Contains == 2 && Tiles[3].Contains == 2 && Tiles[6].Contains == 2) Winner.setString("Winner: Player 2");
					else if (Tiles[0].Contains == 1 && Tiles[4].Contains == 1 && Tiles[8].Contains == 1) Winner.setString("Winner: Player 1");
					else if (Tiles[0].Contains == 2 && Tiles[4].Contains == 2 && Tiles[8].Contains == 2) Winner.setString("Winner: Player 2");
					else if (Tiles[1].Contains == 1 && Tiles[4].Contains == 1 && Tiles[7].Contains == 1) Winner.setString("Winner: Player 1");
					else if (Tiles[1].Contains == 2 && Tiles[4].Contains == 2 && Tiles[7].Contains == 2) Winner.setString("Winner: Player 2");
					else if (Tiles[2].Contains == 1 && Tiles[5].Contains == 1 && Tiles[8].Contains == 1) Winner.setString("Winner: Player 1");
					else if (Tiles[2].Contains == 2 && Tiles[5].Contains == 2 && Tiles[8].Contains == 2) Winner.setString("Winner: Player 2");
					else if (Tiles[2].Contains == 1 && Tiles[4].Contains == 1 && Tiles[6].Contains == 1) Winner.setString("Winner: Player 1");
					else if (Tiles[2].Contains == 2 && Tiles[4].Contains == 2 && Tiles[6].Contains == 2) Winner.setString("Winner: Player 2");
					else if (Tiles[3].Contains == 1 && Tiles[4].Contains == 1 && Tiles[5].Contains == 1) Winner.setString("Winner: Player 1");
					else if (Tiles[3].Contains == 2 && Tiles[4].Contains == 2 && Tiles[5].Contains == 2) Winner.setString("Winner: Player 2");
					else if (Tiles[6].Contains == 1 && Tiles[7].Contains == 1 && Tiles[8].Contains == 1) Winner.setString("Winner: Player 1");
					else if (Tiles[6].Contains == 2 && Tiles[7].Contains == 2 && Tiles[8].Contains == 2) Winner.setString("Winner: Player 2");
					for (int i = 0; i < 9; i++)
					{
						if (Tiles[i].Full == true) yes++;
					}
					if (yes != 9) yes = 0;
					else if (Winner.getString() == "") Winner.setString("DRAW");
				}
			}
		}
		if (GAMEOVER == true && Keyboard::isKeyPressed(Keyboard::R)) Clear(Tiles, turn, SpriteHold, Winner, GAMEOVER);

		// ---------------------------- DRAW ---------------------------------
		window.clear(Color::Black);

		for (int i = 0; i < 9; i++)
		{
			window.draw(Tiles[i].Rectangle);
		}

		for (int i = 0; i < SpriteHold.size(); i++)
		{
			window.draw(SpriteHold[i].sprite);
		}

		if (Winner.getString() != "")
		{
			if (Winner.getString() == "DRAW") Winner.setPosition(V2f(870.f, 120.f));
			else Winner.setPosition(V2f(745.f, 120.f));
			window.draw(Winner);
			window.draw(Restart);
			GAMEOVER = true;
		}

		window.display();
	}
}

void InitTiles(vector<G1_Tile>& Tiles)
{
	float adder = 0.f, y = WINDOW_HEIGHT / 4.5f;
	for (int i = 0; i < 9; i++)
	{
		if (adder > WINDOW_WIDTH / 3.2f - 0.001f) adder = 0.f;
		if (i % 3 == 0 && i != 0) y += WINDOW_HEIGHT / 5.4f;
		Tiles.push_back(G1_Tile(i, Vector2f(WINDOW_WIDTH / 2.909090f + adder, y)));
		adder += WINDOW_WIDTH / 9.6f;
	}
}
void Clear(vector<G1_Tile>& Tiles, bool& turn, vector<SpriteHolder>& SpriteHold, Text& Winner, bool& GAMEOVER)
{
	Tiles.clear();
	InitTiles(Tiles);
	turn = false;
	SpriteHold.clear();
	Winner.setString("");
	GAMEOVER = false;
}