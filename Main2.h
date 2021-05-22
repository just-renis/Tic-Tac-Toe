#pragma once
#include <set>
#include "G2_Tile.h"
#include "VoidFunctions.h"
#include <string>
#include "LeaderBoard.h"
#include "UI.h"
#include "Chronometer.h"

using namespace sftools;

void InitTiles(vector<vector<G2_Tile>>& Map, Texture& tex, int x, int y, RenderWindow& window, Sprite& emoji, bool& FirstClick, Chronometer& time, Text& text, bool& Pause);
void CreatingMines(vector<vector<G2_Tile>>& Map, int mines, int x, int y, Vector2i Coordinates);
void PutNumbers(vector<vector<G2_Tile>>& Map);
void EmptyTiles(vector<vector<G2_Tile>>& Map, int i, int j);
void ClearGame(vector<vector<G2_Tile>>& Map, bool& GameOver, Sprite& emoji, bool& PuttingEverything);
bool CanDoAround(const vector<vector<G2_Tile>>& Map, int i, int j, int Left);
void AroundReveal(vector<vector<G2_Tile>>& Map, int i, int j);
void AroundPutFlags(vector<vector<G2_Tile>>& Map, int i, int j, int& flags);
bool WinnerChecking(const vector<vector<G2_Tile>>& Map, int flags, int mines);
int CalculatingMines(const vector<vector<G2_Tile>>& Map, int mines);
void RandomGameInit(vector<vector<G2_Tile>>& Map, Texture& tex, int& x, int& y, int& mines);
void RevealAllMines(vector<vector<G2_Tile>>& Map);
bool RevealedMine(const vector<vector<G2_Tile>>& Map, int i, int j);

// ---------------------------- UI && LEADERBOARD FUNCTIONS -------------------------------
void InitUI(UI& UI, Font* font);
string AskingName(const Event& e, const Text& text);
string CalculateTime(const Chronometer& time);

void Main2Loop(RenderWindow& window, Event& e, int& MainState, int& DepthState, TexturesMap& texs, Font* font)
{
	int State = 0, flags = 0, mines, x, y;
	srand(static_cast<unsigned> (time(0)));
	bool MainLoop = true;
	bool StartingGame = false;
	bool PuttingEverything = false;
	bool Win = false;
	bool GameOver = false;
	bool Random = false;
	bool FirstClick = true;
	bool Pause = false;
	int Which;
	int LeaderboardType = 0;
	Vector2f MousePos;
	vector<vector<G2_Tile>> Map;
	Sprite spr, spr1, emoji;
	Chronometer time;

	// -------------------------- BACKGROUND FOR LEADERBOARD -------------------------
	bool newBackgroundColorNeeded = true;
	Color backgroundColor = Color::Black, next;
	Clock backgroundTimer;

	// ----------------------- MUSIC --------------------------
	Music music;
	music.openFromFile("Audio/MineSweeperMusic.ogg");
	music.setLoop(true);
	music.play();

	// --------------------------------- SPRITES FOR UI --------------------------
	emoji.setTexture(texs["MineSweeperEmoji"]);
	emoji.setScale(V2f(1.f, 1.f));
	emoji.setTextureRect(IntRect(0, 0, 40, 40));
	spr1.setTexture(texs["Check"]);
	spr1.setScale(V2f(1.f, 1.f));
	spr.setTexture(texs["MineSweeperTex"]);
	spr.setTextureRect(IntRect(40, 0, 20, 20));
	spr.setPosition(Vector2f(0.f, 0.f));
	spr.setScale(V2f(2.f, 2.f));

	// ------------------------- UI ---------------------
	UI UI;
	InitUI(UI, font);
	bool Granted = false;

	// ------------------------ LEADERBOARD ----------------------
	Leaderboard lBoard(font, "Restart", V2f(940.f, 725.f), V2f(925.f, 720.f), V2f(130.f, 50.f), Color::Red, V2f(710.f, 300.f), V2f(550.f, 500.f), Color(170, 170, 170, 255), "MineSweeperLeaderboard.txt", 10, true, V2f(750.f, 400.f), Color::Black, Color::White, FOw(20), FOw(2.f), V2f(1020.f, 400.f), Color::White, Color::Black, FOw(2.f), Color::Red, 4, true, V2f(888.f, 352.f), "Beginner~Medium~Advanced~Custom");
	spr1.setPosition(UI.Rects[6].getPosition());

	while (window.isOpen() && MainLoop)
	{
		MousePos = (Vector2f(Mouse::getPosition(window)));
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) window.close();

			// ------------------------------------------ SETTINGS(State 0) --------------------------------------------------
			if (State == 0)
			{
				if (e.type == Event::MouseButtonPressed)
				{
					if (Mouse::isButtonPressed(Mouse::Left))
					{
						// ----------------------------------- QUIT BUTTON ----------------------------------------
						if (UI.Rects[1].getGlobalBounds().contains(MousePos))
						{
							DepthState = 1;
							MainState = 0;
							MainLoop = false;
						}
						// ------------------------------- START GAME BUTTON --------------------------------------
						else if (UI.Rects[5].getGlobalBounds().contains(MousePos))
						{
							if (!Random)
							{
								int Works = 0;
								for (int i = 6; i < 9; i++)
								{
									if (UI.Texts[i].getString().getSize() > 0) Works++;
									if (UI.Rects[i + 1].getFillColor() == Color::Red) Works = -4;
								}
								if (Works == 3)
								{
									string string = UI.Texts[6].getString();
									if (stoi(string) > 96) string = "96";
									else if (stoi(string) < 2) string = "2";
									x = stoi(string);
									string = UI.Texts[7].getString();
									if (stoi(string) > 48) string = "48";
									else if (stoi(string) < 2) string = "2";
									y = stoi(string);
									string = UI.Texts[8].getString();
									if (stoi(string) > (x * y) - 1) string = to_string((x * y) - 1);
									else if (stoi(string) < 1) string = "1";
									mines = stoi(string);
									State = 1;
									StartingGame = true;
								}
								else if (Works < 0)
								{
									for (int i = 7; i < 10; i++)
									{
										if (UI.Rects[i].getFillColor() == Color::Red)
										{
											Works = i;
											break;
										}
									}
									if (Works == 7) x = 9, y = 9, mines = 10;
									else if (Works == 8) x = 16, y = 16, mines = 40;
									else x = 16, y = 30, mines = 99;
									State = 1;
									StartingGame = true;
								}
							}
							else if (Random) RandomGameInit(Map, texs["MineSweeperTex"], x, y, mines), State = 1, StartingGame = true;
						}
						else if (UI.Rects[6].getGlobalBounds().contains(MousePos)) // RANDOM BUTTON CLICKED.
						{
							Random = !Random;
							Granted = false;
						}
						else if (!Random)
						{
							for (int i = 2; i < 5; i++)
							{
								if (UI.Rects[i].getGlobalBounds().contains(MousePos))
								{
									if (UI.Rects[i].getFillColor() == Color::White) UI.Rects[i].setFillColor(Color::Yellow);
									else UI.Rects[i].setFillColor(Color::White);
									for (int j = 2; j < 5; j++)
									{
										if (UI.Rects[j].getFillColor() == Color::Yellow && i != j) UI.Rects[j].setFillColor(Color::White);
									}
									Granted = true;
									Which = i + 4;
									break;
								}
							}
							for (int i = 7; i < 10; i++)
							{
								if (UI.Rects[i].getGlobalBounds().contains(MousePos))
								{
									if (UI.Rects[i].getFillColor() == Color::White) UI.Rects[i].setFillColor(Color::Red);
									else UI.Rects[i].setFillColor(Color::White);
									for (int j = 7; j < 10; j++)
									{
										if (UI.Rects[j].getFillColor() == Color::Red && i != j) UI.Rects[j].setFillColor(Color::White);
									}
									break;
								}
							}
						}
						else Granted = false;
					}
				}
				if (Granted)
				{
					string Input;
					if (e.type == Event::TextEntered)
					{
						if (UI.Texts[Which].getString().getSize() <= 1)
						{
							if (e.text.unicode >= 48 && e.text.unicode <= 57)
							{
								Input = e.text.unicode;
								UI.Texts[Which].setString(UI.Texts[Which].getString() + Input);
							}
						}
						else if (Which == 8 && UI.Texts[8].getString().getSize() <= 3)
						{
							if (e.text.unicode >= 48 && e.text.unicode <= 57)
							{
								Input = e.text.unicode;
								UI.Texts[8].setString(UI.Texts[8].getString() + Input);
							}
						}
					}
					if (e.type == Event::KeyPressed)
					{
						if (e.key.code == Keyboard::Backspace)
						{
							Input = UI.Texts[Which].getString();
							Input.pop_back();
							UI.Texts[Which].setString(Input);
						}
					}
				}
			}
			// ------------------------------------------------------- GAMEPLAY(State 1) ----------------------------------
			else if (State == 1 && !GameOver)
			{
				if (StartingGame)
				{
					InitTiles(Map, texs["MineSweeperTex"], x, y, window, emoji, FirstClick, time, UI.Texts[22], Pause);
					StartingGame = false;
				}
				if (!Pause && e.type == Event::MouseButtonPressed)
				{
					if (FirstClick) time.reset(true), FirstClick = false;
					if (Mouse::isButtonPressed(Mouse::Left))
					{
						for (int i = 0; i < Map.size(); i++)
						{
							for (int j = 0; j < Map[i].size(); j++)
							{
								if (Map[i][j].Now.getGlobalBounds().contains(MousePos))
								{
									if (!PuttingEverything)
									{
										CreatingMines(Map, mines, x, y, Vector2i(i, j));
										PutNumbers(Map);
										PuttingEverything = true;
									}
									if (Map[i][j].Now.getTextureRect().left != 20 && Map[i][j].type == 40)
									{
										RevealAllMines(Map);
										GameOver = true, flags = 0;
										emoji.setTextureRect(IntRect(80, 0, 40, 40));
									}
									if (Map[i][j].Now.getTextureRect().left == 0)
									{
										if (Map[i][j].type == 60) EmptyTiles(Map, i, j);
										else Map[i][j].Now.setTextureRect(IntRect(Map[i][j].type, 0, 20, 20));
									}
								}
							}
						}
					}
					else if (Mouse::isButtonPressed(Mouse::Right))
					{
						for (int i = 0; i < Map.size(); i++)
						{
							for (int j = 0; j < Map[i].size(); j++)
							{
								if (Map[i][j].Now.getGlobalBounds().contains(MousePos))
								{
									if (Map[i][j].Now.getTextureRect().left == 20) Map[i][j].Now.setTextureRect(IntRect(0, 0, 20, 20)), flags--;
									else if (Map[i][j].Now.getTextureRect().left == 0) Map[i][j].Now.setTextureRect(IntRect(20, 0, 20, 20)), flags++;
									else if (Map[i][j].Now.getTextureRect().left >= 80)
									{
										if (CanDoAround(Map, i, j, 20))
										{
											AroundReveal(Map, i, j);
											if (RevealedMine(Map, i, j))
											{
												RevealAllMines(Map);
												GameOver = true;
												flags = 0;
												emoji.setTextureRect(IntRect(80, 0, 40, 40));
											}
										}
										else if (CanDoAround(Map, i, j, 0)) AroundPutFlags(Map, i, j, flags);
									}
								}
							}
						}
					}
				}
				else if (e.type == Event::KeyPressed)
				{
					if (!GameOver && e.key.code == Keyboard::Escape) // ESCAPE
					{
						ClearGame(Map, GameOver, emoji, PuttingEverything);
						State = 0;
					}
					else if (e.key.code == Keyboard::R)
					{
						ClearGame(Map, GameOver, emoji, PuttingEverything);
						InitTiles(Map, texs["MineSweeperTex"], x, y, window, emoji, FirstClick, time, UI.Texts[22], Pause);
					}
					else if (e.key.code == Keyboard::P)
					{
						Pause = !Pause;
						if (Pause) time.pause();
						else time.resume();
					}
				}
				else if (!GameOver && WinnerChecking(Map, flags, mines))
				{
					for (int i = 0; i < Map.size(); i++)
					{
						for (int j = 0; j < Map[i].size(); j++)
						{
							if (Map[i][j].Now.getTextureRect().left == 0) Map[i][j].Now.setTextureRect(IntRect(20, 0, 20, 20));
						}
					}
					GameOver = true, Win = true, flags = 0;
					emoji.setTextureRect(IntRect(40, 0, 40, 40));
				}
			}
			else if (State == 2) // <--------------------------------------------------------------- LEADERBOARD STATE.
			{
				if (e.type == Event::MouseButtonPressed)
				{
					if (lBoard.ClickedButton(MousePos))
					{
						InitTiles(Map, texs["MineSweeperTex"], x, y, window, emoji, FirstClick, time, UI.Texts[22], Pause);
						State = 1;
					}
				}
				else if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) ClearGame(Map, GameOver, emoji, PuttingEverything), State = 0;
			}
			if (GameOver) // GAME OVER LOST/WON
			{
				if (e.type == Event::KeyPressed && !Win)
				{
					if (e.key.code == Keyboard::R)
					{
						ClearGame(Map, GameOver, emoji, PuttingEverything);
						InitTiles(Map, texs["MineSweeperTex"], x, y, window, emoji, FirstClick, time, UI.Texts[22], Pause);
					}
					else if (e.key.code == Keyboard::Escape) ClearGame(Map, GameOver, emoji, PuttingEverything), State = 0;
				}
				else if (Win)
				{
					if (Mouse::isButtonPressed(Mouse::Left) && UI.Rects[12].getGlobalBounds().contains(MousePos) && UI.Texts[20].getString().getSize() > 0)
					{
						for (int i = 7; i < 10; i++)
						{
							if (UI.Rects[i].getFillColor() == Color::Red)
							{
								LeaderboardType = i - 7;
								lBoard.InsertPlayer(UI.Texts[20].getString(), UI.Texts[22].getString(), font, false, LeaderboardType);
								break;
							}
							else if (i == 9) LeaderboardType = 3, lBoard.InsertPlayer(UI.Texts[20].getString(), UI.Texts[22].getString(), font, false, LeaderboardType);
						}
						ClearGame(Map, GameOver, emoji, PuttingEverything);
						State = 2;
						Win = false;
					}
					else UI.Texts[20].setString(AskingName(e, UI.Texts[20]));
				}
			}
		}
		// ---------------------------------------- DRAW ----------------------------------
		window.clear(Color(170, 170, 170, 255));

		if (State == 0)
		{
			for (int i = 0; i < UI.Rects.size() - 3; i++)
			{
				window.draw(UI.Rects[i]);
			}
			for (int i = 0; i < UI.Texts.size() - 8; i++)
			{
				window.draw(UI.Texts[i]);
			}
			if (Random) window.draw(spr1);
		}
		else if (State == 1)
		{
			UI.Texts[23].setString(to_string(CalculatingMines(Map, mines)));
			if (!FirstClick && !GameOver) UI.Texts[22].setString(CalculateTime(time));
			for (int i = 22; i < 24; i++) window.draw(UI.Texts[i]);
			window.draw(spr);
			for (int i = 0; i < Map.size(); i++)
			{
				for (int j = 0; j < Map[i].size(); j++) window.draw(Map[i][j].Now);
			}
			window.draw(emoji);
			if (Win)
			{
				for (int i = 10; i < UI.Rects.size(); i++) window.draw(UI.Rects[i]);
				for (int i = 19; i < 22; i++) window.draw(UI.Texts[i]);
			}
			if (Pause) window.draw(UI.Texts[18]);
		}
		else
		{
			backgroundColorChanger(backgroundColor, next, backgroundTimer, newBackgroundColorNeeded);
			lBoard.DrawLeaderboard(window, e, backgroundColor, LeaderboardType);
			for (int i = 24; i < UI.Texts.size(); i++) window.draw(UI.Texts[i]);
		}
		window.display();
	}
}

// --------------------------------------------------------------------- FUNCTIONS --------------------------------------------------------------------------

void InitTiles(vector<vector<G2_Tile>>& Map, Texture& tex, int x, int y, RenderWindow& window, Sprite& Emoji, bool& FirstClick, Chronometer& time, Text& text, bool& Pause)
{
	time.reset(true);
	Pause = false;
	text.setString("0 hours : 0 minutes : 0 seconds");
	Emoji.setPosition(Vector2f(WINDOW_WIDTH / 2.086956f, 0.f));
	for (int i = 0; i < y; i++)
	{
		Map.push_back(vector<G2_Tile>());
		for (int j = 0; j < x; j++)
		{
			Map[i].push_back(G2_Tile(&tex, Vector2f(j * (WINDOW_WIDTH / 96.f), i * (WINDOW_HEIGHT / 54.f) + WINDOW_HEIGHT / 27.f)));
			Map[i][j].Now.setScale(V2f(1.f, 1.f));
		}
	}
	FirstClick = true;
}

void CreatingMines(vector<vector<G2_Tile>>& Map, int mines, int x, int y, Vector2i Coordinates)
{
	int X, Y;
	set<pair<int, int>> MinePos;
	size_t PreviousMineSize = 0;
	while (mines != 0)
	{
		X = rand() % x;
		Y = rand() % y;
		if (!(Coordinates.x == Y && Coordinates.y == X)) MinePos.insert(make_pair(X, Y));
		if (PreviousMineSize != MinePos.size())
		{
			Map[Y][X].type = 40;
			PreviousMineSize = MinePos.size();
			mines--;
		}
	}
}

void PutNumbers(vector<vector<G2_Tile>>& Map)
{
	int mines = 0;
	for (int i = 0; i < Map.size(); i++)
	{
		for (int j = 0; j < Map[i].size(); j++)
		{
			if (Map[i][j].type != 40)
			{
				for (int k = i - 1; k <= i + 1; k++)
				{
					if (k > -1 && k < Map.size())
					{
						for (int l = j - 1; l <= j + 1; l++)
						{
							if (l > -1 && l < Map[i].size())
							{
								if (Map[k][l].type == 40) mines++;
							}
						}
					}
				}
				Map[i][j].type = 60 + mines * 20;
				mines = 0;
			}
		}
	}
}

void EmptyTiles(vector<vector<G2_Tile>>& Map, int i, int j)
{
	vector<Vector2i> ToCheck, Uncheck;
	ToCheck.push_back(Vector2i(j, i));
	while (ToCheck.size() > 0)
	{
		for (int k = i - 1; k <= i + 1; k++)
		{
			if (k > -1 && k < Map.size())
			{
				for (int l = j - 1; l <= j + 1; l++)
				{
					if (l > -1 && l < Map[i].size() && !Map[k][l].Checked)
					{
						if (Map[k][l].Now.getTextureRect().left == 20)
						{
							Uncheck.push_back(Vector2i(l, k));
							Map[k][l].Checked = true;
						}
						else if (Map[k][l].type == 60)
						{
							Map[k][l].Now.setTextureRect(IntRect(60, 0, 20, 20));
							ToCheck.push_back(Vector2i(l, k));
							Map[k][l].Checked = true;
						}
						else
						{
							Map[k][l].Now.setTextureRect(IntRect(Map[k][l].type, 0, 20, 20));
							Map[k][l].Checked = true;
						}
					}
				}
			}
		}
		Map[i][j].Checked = true;
		ToCheck.erase(ToCheck.begin());
		if (ToCheck.size() == 0) break;
		else i = ToCheck[0].y, j = ToCheck[0].x;
	}
	for (int k = 0; k < Uncheck.size(); k++)
	{
		Map[Uncheck[k].y][Uncheck[k].x].Checked = false;
	}
}

void ClearGame(vector<vector<G2_Tile>>& Map, bool& GameOver, Sprite& emoji, bool& PuttingEverything)
{
	Map.clear();
	GameOver = false;
	PuttingEverything = false;
	emoji.setTextureRect(IntRect(0, 0, 40, 40));
}

bool CanDoAround(const vector<vector<G2_Tile>>& Map, int i, int j, int Left)
{
	int Tiles = 0;
	for (int k = i - 1; k <= i + 1; k++)
	{
		if (k > -1 && k < Map.size())
		{
			for (int l = j - 1; l <= j + 1; l++)
			{
				if (l > -1 && l < Map[i].size())
				{
					if (Map[k][l].Now.getTextureRect().left == Left) Tiles++;
					else if (Left == 0 && Map[k][l].Now.getTextureRect().left == 20) Tiles++;
				}
			}
		}
	}
	if (Map[i][j].type == 60 + Tiles * 20) return true;
	return false;
}

void AroundReveal(vector<vector<G2_Tile>>& Map, int i, int j)
{
	for (int k = i - 1; k <= i + 1; k++)
	{
		if (k > -1 && k < Map.size())
		{
			for (int l = j - 1; l <= j + 1; l++)
			{
				if (l > -1 && l < Map[i].size())
				{
					if (Map[k][l].type == 60) EmptyTiles(Map, k, l);
					else if (Map[k][l].Now.getTextureRect().left != 20) Map[k][l].Now.setTextureRect(IntRect(Map[k][l].type, 0, 20, 20));
				}
			}
		}
	}
}

void AroundPutFlags(vector<vector<G2_Tile>>& Map, int i, int j, int& flags)
{
	for (int k = i - 1; k <= i + 1; k++)
	{
		if (k > -1 && k < Map.size())
		{
			for (int l = j - 1; l <= j + 1; l++)
			{
				if (l > -1 && l < Map[i].size())
				{
					if (Map[k][l].Now.getTextureRect().left == 0) Map[k][l].Now.setTextureRect(IntRect(20, 0, 20, 20)), flags++;
				}
			}
		}
	}
}

bool RevealedMine(const vector<vector<G2_Tile>>& Map, int i, int j)
{
	for (int i = 0; i < Map.size(); i++)
	{
		for (int j = 0; j < Map[i].size(); j++)
		{
			if (Map[i][j].Now.getTextureRect().left == 40) return true;
		}
	}
	return false;
}

bool WinnerChecking(const vector<vector<G2_Tile>>& Map, int flags, int mines)
{
	int kiek = 0;
	for (int i = 0; i < Map.size(); i++)
	{
		for (int j = 0; j < Map[i].size(); j++)
		{
			if (Map[i][j].Now.getTextureRect().left != 0) kiek++;
		}
	}
	if (kiek == Map.size() * Map[0].size() && flags == mines) return true;
	kiek = 0;
	for (int i = 0; i < Map.size(); i++)
	{
		for (int j = 0; j < Map[i].size(); j++)
		{
			if (Map[i][j].Now.getTextureRect().left == 0) kiek++;
		}
	}
	if (kiek == mines - flags) return true;
	return false;
}

int CalculatingMines(const vector<vector<G2_Tile>>& Map, int mines)
{
	int flags = 0;
	for (int i = 0; i < Map.size(); i++)
	{
		for (int j = 0; j < Map[i].size(); j++)
		{
			if (Map[i][j].Now.getTextureRect().left == 20) flags++;
		}
	}
	return mines - flags;
}

void RandomGameInit(vector<vector<G2_Tile>>& Map, Texture& tex, int& x, int& y, int& mines)
{
	x = rand() % 94 + 2;
	y = rand() % 47 + 2;
	mines = rand() % (x * y - 1) + 1;
}

void RevealAllMines(vector<vector<G2_Tile>>& Map)
{
	for (int i = 0; i < Map.size(); i++)
	{
		for (int j = 0; j < Map[i].size(); j++)
		{
			if (Map[i][j].type == 40) Map[i][j].Now.setTextureRect(IntRect(40, 0, 20, 20));
		}
	}
}

void InitUI(UI& UI, Font* font)
{
	// -------------------------------------------- INIT TEXTS ----------------------------------------------
	UI.Texts.push_back(TextSetter(font, FOw(45), 0.f, Color::Black, Color::White, "Mine Sweeper Settings", V2f(730.f, 315.f))); // 0
	UI.Texts.push_back(TextSetter(font, FOw(25), 0.f, Color::Black, Color::White, "Quit", V2f(940.f, 715.f))); // 1
	UI.Texts.push_back(TextSetter(font, FOw(35), 0.f, Color::Black, Color::White, "Custom Map", V2f(740.f, 380.f))); // 2
	UI.Texts.push_back(TextSetter(font, FOw(35), 0.f, Color::Black, Color::White, "Width:", V2f(715.f, 430.f))); // 3
	UI.Texts.push_back(TextSetter(font, FOw(35), 0.f, Color::Black, Color::White, "Height:", V2f(715.f, 470.f))); // 4
	UI.Texts.push_back(TextSetter(font, FOw(35), 0.f, Color::Black, Color::White, "Mines:", V2f(715.f, 510.f))); // 5
	UI.Texts.push_back(TextSetter(font, FOw(15), 0.f, Color::Black, Color::White, "", V2f(840.f, 445.f))); // 6
	UI.Texts.push_back(TextSetter(font, FOw(15), 0.f, Color::Black, Color::White, "", V2f(840.f, 485.f))); // 7
	UI.Texts.push_back(TextSetter(font, FOw(15), 0.f, Color::Black, Color::White, "", V2f(840.f, 525.f))); // 8
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Yellow, Color::White, "Start the Game", V2f(830.f, 625.f))); // 9
	UI.Texts.push_back(TextSetter(font, FOw(35), 0.f, Color::Black, Color::White, "Random:", V2f(715.f, 550.f))); // 10
	UI.Texts.push_back(TextSetter(font, FOw(17), FOw(1.f), Color::Red, Color::White, "(Max: 96 width)", V2f(870.f, 445.f))); // 11
	UI.Texts.push_back(TextSetter(font, FOw(17), FOw(1.f), Color::Red, Color::White, "(Max: 48 height)", V2f(870.f, 485.f))); // 12
	UI.Texts.push_back(TextSetter(font, FOw(20), 0.f, Color::Black, Color::Transparent, "Click 'r' button to restart level in game", V2f(795.f, 790.f))); // 13
	UI.Texts.push_back(TextSetter(font, FOw(20), 0.f, Color::Black, Color::Transparent, "Click 'p' button to pause level in game", V2f(795.f, 820.f))); // 14
	UI.Texts.push_back(TextSetter(font, FOw(20), 0.f, Color::Black, Color::Transparent, "Beginner", V2f(1079.f, 445.f))); // 15
	UI.Texts.push_back(TextSetter(font, FOw(20), 0.f, Color::Black, Color::Transparent, "Medium", V2f(1083.f, 480.f))); // 16
	UI.Texts.push_back(TextSetter(font, FOw(20), 0.f, Color::Black, Color::Transparent, "Advanced", V2f(1075.f, 515.f))); // 17

	UI.Texts.push_back(TextSetter(font, FOw(200), 0.f, Color::Black, Color::Transparent, "GAME PAUSED", V2f(280.f, 320.f))); // 18
	UI.Texts.push_back(TextSetter(font, FOw(25), FOw(2.f), Color::Black, Color::White, "Write your name", V2f(870.f, 480.f))); // 19
	UI.Texts.push_back(TextSetter(font, FOw(20), 0.f, Color::Black, Color::Transparent, "", V2f(735.f, 515.f))); // 20
	UI.Texts.push_back(TextSetter(font, FOw(25), 0.f, Color::Black, Color::Transparent, "OK", V2f(943.f, 582.f))); // 21
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "0 hours : 0 minutes : 0 seconds", V2f(960.f, -10.f))); // 22
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::White, "", V2f(40.f, -10.f))); // 23
	UI.Texts.push_back(TextSetter(font, FOw(40), FOw(2.f), Color::Black, Color::White, "LEADERBOARD", V2f(855.f, 300.f))); // 24
	UI.Texts.push_back(TextSetter(font, FOw(25), 0.f, Color::Black, Color::Transparent, "", V2f(935.f, 350.f))); // 25

	// -------------------------------------------- INIT RECTS ----------------------------------------------
	UI.Rects.push_back(RectangleSetter(V2f(550.f, 450.f), V2f(685.f, 315.f), Color(140, 140, 140, 255), Color::Black, FOw(5.f))); // 0
	UI.Rects.push_back(RectangleSetter(V2f(100.f, 40.f), V2f(915.f, 713.f), Color::Transparent, Color::Black, FOw(3.f))); // 1
	UI.Rects.push_back(RectangleSetter(V2f(25.f, 20.f), V2f(840.f, 445.f), Color::White, Color::Black, FOw(2.f))); // 2
	UI.Rects.push_back(RectangleSetter(V2f(25.f, 20.f), V2f(840.f, 485.f), Color::White, Color::Black, FOw(2.f))); // 3
	UI.Rects.push_back(RectangleSetter(V2f(40.f, 20.f), V2f(840.f, 525.f), Color::White, Color::Black, FOw(2.f))); // 4
	UI.Rects.push_back(RectangleSetter(V2f(310.f, 50.f), V2f(810.f, 625.f), Color::Transparent, Color::Black, FOw(4.f))); // 5
	UI.Rects.push_back(RectangleSetter(V2f(20.f, 20.f), V2f(870.f, 565.f), Color::White, Color::Black, FOw(2.f))); // 6
	UI.Rects.push_back(RectangleSetter(V2f(200.f, 25.f), V2f(1020.f, 445.f), Color::White, Color::Black, FOw(2.f))); // 7
	UI.Rects.push_back(RectangleSetter(V2f(200.f, 25.f), V2f(1020.f, 480.f), Color::White, Color::Black, FOw(2.f))); // 8
	UI.Rects.push_back(RectangleSetter(V2f(200.f, 25.f), V2f(1020.f, 515.f), Color::White, Color::Black, FOw(2.f))); // 9
	UI.Rects.push_back(RectangleSetter(V2f(500.f, 150.f), V2f(710.f, 465.f), Color(120, 120, 120, 255), Color::Red, FOw(10.f))); // 10
	UI.Rects.push_back(RectangleSetter(V2f(450.f, 25.f), V2f(735.f, 515.f), Color::White, Color::Black, FOw(2.f))); // 11
	UI.Rects.push_back(RectangleSetter(V2f(50.f, 25.f), V2f(935.f, 585.f), Color::White, Color::Black, FOw(2.f))); // 12
}

string AskingName(const Event& e, const Text& text)
{
	string Input, name = text.getString();
	if (e.type == Event::KeyPressed)
	{
		if (e.key.code == Keyboard::BackSpace && name.size() > 0) name.pop_back();
		else if (e.key.code == Keyboard::Delete) name.clear();
	}
	if (e.type == Event::TextEntered)
	{
		if (e.text.unicode > 31 && e.text.unicode < 126) Input += e.text.unicode;
		name += Input;
		if (name.size() > 12) name.pop_back();
	}
	return name;
}

string CalculateTime(const Chronometer& time)
{
	string Time;
	int TimeNumber = (int)time.getElapsedTime().asSeconds();
	Time = to_string(TimeNumber / 3600) + " hours : " + to_string(TimeNumber / 60) + " minutes : " + to_string(TimeNumber % 60) + " seconds";
	return Time;
}