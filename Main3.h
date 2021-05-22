#pragma once
#include "Program.h"
#include "LeaderBoard.h"
#include "UI.h"

void InitSnake(vector<RectangleShape>& snake, int SnakeSize, char& Direction);
void SnakeMovement(vector<RectangleShape>& snake, char Direction);
Vector2f Borders(RectangleShape SnakeHead);
Vector2f NewLocation(const vector<RectangleShape>& snake);
void IncreaseSnakeLength(vector<RectangleShape>& snake, Vector2f SnakeTail);
bool GameOver(const vector<RectangleShape>& snake);
void ChangeDirection(char& direction, bool& CanChangeDirection);
void RestartTheGame(vector<RectangleShape>& snake, bool& GAMEOVER, int& FruitsEaten, Text& Score);
void GettingInput(const Event& e, bool& PermissionGranted, Text& text, bool OnlyNumbers);
void GameModeSwitcher(const vector<CircleShape>& Shapes, Vector2f MousePos, int& GameModeSwitch, Text& WhichGameMode);
string SpeedSetter(float x);
void CreatingUI(UI& UI, Font* font);

void Main3Loop(RenderWindow& window, Event& e, int& MainState, int& DepthState, Font* font, TexturesMap& Texs)
{
	// ---------------------------- GAME STUFF ----------------------------
	srand(static_cast<unsigned> (time(0)));
	Clock time; // Time when snake moves one cell.
	bool MainLoop = true, CanChangeDirection = false, GAMEOVER = false;
	int FruitsEaten = 0, State = 0, SnakeSize = 4, WhichSection = 0;
	float SnakeSpeed = 0.3f;
	Vector2f MousePos;

	// ---------------------------------- INPUT/UI STUFF -------------------------------
	bool PermissionGranted = false,
	DrawError = false,
	CanShowError = false,
	Dragging = false,
	OnlyNumbersAccepted = false;
	Clock ErrorTime;
	int GameModeSwitch = 0;

	// ----------------------------- LEADERBOARD ------------------------------
	Leaderboard lBoard(font, "Restart", Vector2f(920.f, 995.f), Vector2f(905.f, 990.f), Vector2f(130.f, 50.f), Color::Blue, Vector2f(585.f, 40.f), Vector2f(750.f, 1000.f), Color::Green, "SnakeLeaderboard.txt", 20, false, Vector2f(665.f, 140.f), Color::White, Color::Black, 25, 2.f, Vector2f(1180.f, 140.f), Color::Black, Color::White, 2.f, Color::Transparent, 1, false, Vector2f(), "");

	// -------------------------- IN-GAME UI ------------------------ 
	Text Score = TextSetter(font, 20, 1.f, Color::White, Color::Blue, "Score: 0", Vector2f(0.f, 0.f)); // Score text.
	Text CantStartGame = TextSetter(font, 50, 2.f, Color::Red, Color::White, "Can't start the game.", Vector2f(0.f, 0.f)); // Error text.

	// --------------------------------- OTHER UI ---------------------------
	UI UI; // UI data holder.
	CreatingUI(UI, font);
	
	// -------------------- SNAKE STUFF --------------------------------
	vector<RectangleShape> snake;
	Vector2f SnakeTail;
	char Direction;

	// ---------------------------- FRUIT ---------------------------
	RectangleShape Fruit(Vector2f(19.f, 19.f));
	Fruit.setFillColor(Color::Red);

	while (window.isOpen() && MainLoop)
	{
		if (DrawError) ErrorTime.restart(), DrawError = false, CanShowError = true;
		MousePos = (Vector2f(Mouse::getPosition(window)));
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::MouseButtonPressed)
			{
				if (State == 0) // <------------------------------------ IN THE SETTINGS ---------------------------------
				{
					if (UI.Rects[1].getGlobalBounds().contains(MousePos))
					{
						if (!PermissionGranted && UI.Texts[3].getString().getSize() > 0)
						{
							string Speed = UI.Texts[7].getString(), Size = UI.Texts[10].getString();
							if (UI.Texts[5].getString() == "Custom")
							{
								SnakeSpeed = stof(Speed); // <-------------- Snake speed changed in custom applied.
								SnakeSize = stoi(Size);
							}
							InitSnake(snake, SnakeSize, Direction);
							Fruit.setPosition(NewLocation(snake));
							State = 1;
						}
						else DrawError = true;
					}
					else if (e.type == Event::MouseButtonPressed && UI.Shapes[2].getGlobalBounds().contains(MousePos)) Dragging = true;
					else GameModeSwitcher(UI.Shapes, MousePos, GameModeSwitch, UI.Texts[5]);
					if (!PermissionGranted) // <------------------------------------- USER INPUT STUFF ------------------------------------
					{
						if (UI.Rects[2].getGlobalBounds().contains(MousePos)) PermissionGranted = true, OnlyNumbersAccepted = false, UI.Which = 3;
						else if (UI.Rects[6].getGlobalBounds().contains(MousePos)) PermissionGranted = true, OnlyNumbersAccepted = true, UI.Which = 10;
					}
				}
				else if (State == 2) // <------------------------------------ IN THE LEADERBOARD ---------------------------------
				{
					if (lBoard.ClickedButton(MousePos)) RestartTheGame(snake, GAMEOVER, FruitsEaten, Score), State = 0;
				}
			}
			else if (e.type == Event::MouseButtonReleased) Dragging = false;
			if (PermissionGranted) GettingInput(e, PermissionGranted, UI.Texts[UI.Which], OnlyNumbersAccepted);// <------------------------------------ INPUT ------------------------------------
			else if (e.type == Event::KeyPressed)
			{
				if (Keyboard::isKeyPressed(Keyboard::Escape)) // <----------------------- FOR QUITING THIS GAME.
				{
					MainState = 0;
					DepthState = 1;
					MainLoop = false;
				}
				if (State == 1) // <------------------------------------ IN THE GAME ---------------------------------
				{
					if (CanChangeDirection) ChangeDirection(Direction, CanChangeDirection);
				}
			}
		} // <------------------------------------------------------------------------------------------------------------- EVENTS ENDS.
		if (State == 0) // <------------------------------------ IN THE SETTINGS --------------------------------------
		{
			if (Dragging)
			{
				if (MousePos.x >= 850.f && MousePos.x <= 1150.f) UI.Shapes[2].setPosition(Vector2f(MousePos.x, UI.Shapes[2].getPosition().y));
				else if (MousePos.x < 850.f) UI.Shapes[2].setPosition(Vector2f(850.f, UI.Shapes[2].getPosition().y));
				else UI.Shapes[2].setPosition(Vector2f(1150.f, UI.Shapes[2].getPosition().y));
				UI.Texts[7].setString(SpeedSetter(UI.Shapes[2].getPosition().x));
			}
		}
		else if (State == 1 && !GAMEOVER) // <------------------------------------ IN THE GAME -------------------------------
		{
			if (time.getElapsedTime().asSeconds() >= SnakeSpeed)
			{
				SnakeTail = snake[snake.size() - 1].getPosition(); // <--------------- Snake tail last position before moving.
				SnakeMovement(snake, Direction); // <------------------------- Snake movement.
				if (snake[0].getPosition() == Fruit.getPosition()) // <---------- If snake eats fruit.
				{
					FruitsEaten++; // <----------- How many fruits were eaten.
					Score.setString("Score: " + to_string(FruitsEaten)); // <------------ Score increases.
					Fruit.setPosition(NewLocation(snake)); // <------------------ After fruit is eaten, new fruit is made at random location.
					IncreaseSnakeLength(snake, SnakeTail); // <-------------- Increase snake length.
				}
				time.restart(); // <-------------------------- Time restart.
				if (GameOver(snake)) GAMEOVER = true; // <--------------------- IF SNAKE DIED.
				CanChangeDirection = true;
			}
		}

		// --------------------------------------------------------------------------- DRAW ---------------------------------------------------------------------------
		if (State == 0) // <---------------------------------------------------- IN THE SETTINGS --------------------------------------------
		{
			window.clear(Color::Green);
			for (int i = 0; i < 4; i++) window.draw(UI.Rects[i]);
			for (int i = 0; i < 6; i++) window.draw(UI.Texts[i]);
			for (int i = 0; i < 2; i++) window.draw(UI.Shapes[i]);
			if (UI.Texts[5].getString() == "Custom")
			{
				for (int i = 4; i < UI.Rects.size(); i++) window.draw(UI.Rects[i]);
				for (int i = 6; i < 11; i++) window.draw(UI.Texts[i]);
				window.draw(UI.Shapes[2]);
			}
			if (PermissionGranted) window.draw(UI.Texts[UI.Texts.size() - 4]);
			if (ErrorTime.getElapsedTime().asSeconds() <= 1.f && CanShowError) window.draw(CantStartGame);
		}
		else if (State == 1) // <---------------------------------------------------- IN THE GAME --------------------------------------------
		{
			window.clear(Color::Black);
			for (int i = 1; i < snake.size(); i++) window.draw(snake[i]);
			window.draw(snake[0]);
			window.draw(Fruit);
			window.draw(Score);
			if (GAMEOVER)
			{
				lBoard.InsertPlayer(UI.Texts[3].getString(), to_string(FruitsEaten), font, true, 0);
				State = 2;
			}
		}
		else // <---------------------------------------------------- IN THE LEADERBOARD --------------------------------------------
		{
			lBoard.DrawLeaderboard(window, e, Color::Black, WhichSection);
			for (int i = (int)UI.Texts.size() - 3; i < UI.Texts.size(); i++) window.draw(UI.Texts[i]);
		}
		window.display();
	}
}

void InitSnake(vector<RectangleShape>& snake, int SnakeSize, char& Direction)
{
	Direction = 'r';
	snake.push_back(RectangleSetter(Vector2f(19.f, 19.f), Vector2f(100.f, 540.f), Color::Blue, Color::Transparent, 0.f));
	for (int i = 1; i < 3; i++)
	{
		snake.push_back(RectangleSetter(Vector2f(19.f, 19.f), Vector2f(100.f - 20.f * i, 540.f), Color::Green, Color::Transparent, 0.f));
	}
	for (int i = 3; i < SnakeSize; i++)
	{
		snake.push_back(RectangleSetter(Vector2f(19.f, 19.f), Vector2f(60.f, 540.f), Color::Green, Color::Transparent, 0.f));
	}
}

void SnakeMovement(vector<RectangleShape>& snake, char Direction)
{
	Vector2f PreviousPos1, PreviousPos2;
	PreviousPos1 = snake[0].getPosition();
	switch (Direction)
	{
		case 'r':
			snake[0].move(20.f, 0.f);
			break;
		case 'l':
			snake[0].move(-20.f, 0.f);
			break;
		case 'u':
			snake[0].move(0.f, -20.f);
			break;
		default:
			snake[0].move(0.f, 20.f);
	}
	snake[0].setPosition(Borders(snake[0]));
	for (int i = 1; i < snake.size(); i++)
	{
		PreviousPos2 = snake[i].getPosition();
		snake[i].setPosition(PreviousPos1);
		PreviousPos1 = PreviousPos2;
	}
}

Vector2f Borders(RectangleShape SnakeHead)
{
	float x = SnakeHead.getPosition().x, y = SnakeHead.getPosition().y;
	if (x == -20.f) SnakeHead.setPosition(Vector2f(1900.f, y));
	else if (x == 1920.f) SnakeHead.setPosition(Vector2f(0.f, y));
	else if (y == -20.f) SnakeHead.setPosition(Vector2f(x, 1060.f));
	else if (y == 1080.f) SnakeHead.setPosition(Vector2f(x, 0.f));
	return SnakeHead.getPosition();
}

Vector2f NewLocation(const vector<RectangleShape>& snake)
{
	bool ALLGOOD = false;
	int x, y;
	float xf, yf;
	while (true)
	{
		x = rand() % 1901, y = rand() % 1061;
		if (x % 20 >= 10) x += 20 - x % 20;
		else x -= x % 20;
		if (y % 20 >= 10) y += 20 - y % 20;
		else y -= y % 20;
		xf = float(x), yf = float(y);
		for (int i = 0; i < snake.size(); i++) // <--------------------------- Checking if fruit spawned on snake.
		{
			if (snake[i].getPosition().x == xf && snake[i].getPosition().y == yf) break;
			else if (i == snake.size() - 1) ALLGOOD = true;
		}
		if (ALLGOOD) break; // <---------------- If fruit didn't spawned on snake then change fruit location to new location and exit this function.
	}
	return Vector2f(xf, yf);
}

void IncreaseSnakeLength(vector<RectangleShape>& snake, Vector2f SnakeTail)
{
	snake.push_back(RectangleSetter(Vector2f(19.f, 19.f), SnakeTail, Color::Green, Color::Transparent, 0.f));
}

bool GameOver(const vector<RectangleShape>& snake)
{
	for (int i = 1; i < snake.size(); i++)
	{
		if (snake[0].getPosition() == snake[i].getPosition()) return true;
	}
	return false;
}

void ChangeDirection(char& direction, bool& CanChangeDirection)
{
	char previousDirection = direction;
	if (Keyboard::isKeyPressed(Keyboard::D) && direction != 'l' && direction != 'r') direction = 'r';
	else if (Keyboard::isKeyPressed(Keyboard::W) && direction != 'd' && direction != 'u') direction = 'u';
	else if (Keyboard::isKeyPressed(Keyboard::S) && direction != 'u' && direction != 'd') direction = 'd';
	else if (Keyboard::isKeyPressed(Keyboard::A) && direction != 'r' && direction != 'l') direction = 'l';
	if (previousDirection != direction) CanChangeDirection = false;
}

void RestartTheGame(vector<RectangleShape>& snake, bool& GAMEOVER, int& FruitsEaten, Text& Score)
{
	snake.clear();
	GAMEOVER = false;
	FruitsEaten = 0;
	Score.setString("Score: 0");
}

void GettingInput(const Event& e, bool& PermissionGranted, Text& text, bool OnlyNumbers)
{
	string Input;
	string string = text.getString();
	if (e.type == Event::KeyPressed)
	{
		if (e.key.code == Keyboard::Backspace && string.size() > 0) string.pop_back();
		else if (e.key.code == Keyboard::Delete) string.clear();
		else if (e.key.code == Keyboard::Enter && string.size() > 0)
		{
			if (OnlyNumbers)
			{
				if (stoi(string) > 500) string = "500";
				else if (stoi(string) < 3) string = "3";
			}
			PermissionGranted = false;
		}
	}
	if (e.type == Event::TextEntered)
	{
		if (OnlyNumbers)
		{
			if (e.text.unicode >= 48 && e.text.unicode <= 57) Input += e.text.unicode;
		}
		else if (e.text.unicode > 31 && e.text.unicode < 126) Input += e.text.unicode;
		string += Input;
		if (string.size() > 23) string.pop_back();
		else if (OnlyNumbers && string.size() > 3) string.pop_back();
	}
	text.setString(string);
}

void GameModeSwitcher(const vector<CircleShape>& Shapes, Vector2f MousePos, int& GameModeSwitch, Text& WhichGameMode)
{
	bool Changed = false;
	if (Shapes[0].getGlobalBounds().contains(MousePos)) GameModeSwitch++, Changed = true;
	else if (Shapes[1].getGlobalBounds().contains(MousePos)) GameModeSwitch--, Changed = true;
	if (Changed)
	{
		if (GameModeSwitch == -1) GameModeSwitch = 1;
		else if (GameModeSwitch == 2) GameModeSwitch = 0;
		if (GameModeSwitch == 0) WhichGameMode.setString("Classic");
		else WhichGameMode.setString("Custom");
	}
}

string SpeedSetter(float x)
{
	float speed = 0.05f;
	x -= 850.f;
	speed += (x / 3.f) / 100.f;
	string string = to_string(speed);
	for (int i = 0; i < 4; i++) string.pop_back();
	return string;
}

void CreatingUI(UI& UI, Font* font)
{
	// --------------------- GAME START UI ---------------------
	UI.Texts.push_back(TextSetter(font, 40, 2.f, Color::Black, Color::White, "Game Settings", Vector2f(830.f, 0.f))); // Game Settings text.
	UI.Texts.push_back(TextSetter(font, 35, 2.f, Color::Green, Color::Black, "Start the game", Vector2f(842.f, 1030.f))); // Start the game text.
	UI.Texts.push_back(TextSetter(font, 25, 2.f, Color::Black, Color::White, "Your Name: ", Vector2f(665.f, 60.f))); // Your name: text.
	UI.Texts.push_back(TextSetter(font, 18, 0.f, Color::Black, Color::Transparent, "", Vector2f(820.f, 65.f))); // Name text.
	UI.Texts.push_back(TextSetter(font, 20, 3.f, Color::Black, Color::White, "Game Mode: ", Vector2f(705.f, 120.f))); // Game mode text.
	UI.Texts.push_back(TextSetter(font, 20, 0.f, Color::Black, Color::Transparent, "Classic", Vector2f(980.f, 120.f))); // Game modes text.
	UI.Texts.push_back(TextSetter(font, 18, 1.f, Color::Black, Color::Blue, "Speed: ", Vector2f(705.f, 160.f))); // Speed text.
	UI.Texts.push_back(TextSetter(font, 18, 0.f, Color::Black, Color::Transparent, "0.05", Vector2f(770.f, 160.f))); // Snake speed.
	UI.Texts.push_back(TextSetter(font, 18, 1.f, Color::Black, Color::Yellow, "Snake Size: ", Vector2f(705.f, 190.f))); // Snake size text.
	UI.Texts.push_back(TextSetter(font, 13, 0.f, Color::Black, Color::Transparent, "(Min Size: 3, Max Size: 500)", Vector2f(860.f, 195.f))); // What size can snake be.
	UI.Texts.push_back(TextSetter(font, 18, 0.f, Color::Black, Color::Transparent, "3", Vector2f(810.f, 190.f))); // Snake size.
	UI.Texts.push_back(TextSetter(font, 30, 0.f, Color::Red, Color::Transparent, "Press 'Enter' to apply", Vector2f(823.f, 980.f))); // Warning to apply input text.

	UI.Rects.push_back(RectangleSetter(Vector2f(600.f, 1080.f), Vector2f(660.f, 0.f), Color::Cyan, Color::White, 10.f)); // Game start page.
	UI.Rects.push_back(RectangleSetter(Vector2f(280.f, 35.f), Vector2f(820.f, 1035.f), Color::Transparent, Color::White, 5.f)); // Start game text rectangle.
	UI.Rects.push_back(RectangleSetter(Vector2f(437.f, 25.f), Vector2f(820.f, 63.f), Color::White, Color::Black, 1.f)); // For name input.
	UI.Rects.push_back(RectangleSetter(Vector2f(350.f, 25.f), Vector2f(840.f, 120.f), Color::White, Color::Black, 1.f)); // Change game mode.
	UI.Rects.push_back(RectangleSetter(Vector2f(50.f, 25.f), Vector2f(765.f, 160.f), Color::White, Color::Black, 1.f)); // Speed container.
	UI.Rects.push_back(RectangleSetter(Vector2f(307.f, 2.f), Vector2f(850.f, 171.f), Color::Black, Color::Transparent, 0.f)); // Speed line.
	UI.Rects.push_back(RectangleSetter(Vector2f(50.f, 25.f), Vector2f(805.f, 190.f), Color::White, Color::Black, 1.f)); // Snake size input.

	UI.Shapes.push_back(ShapeSetter(15.f, Vector2f(1175.f, 132.f), Color::Black, Color::Transparent, 0.f, 3)); // Right arrow to change game mode.
	UI.Shapes[0].setOrigin(Vector2f(15.f, 15.f));
	UI.Shapes[0].rotate(90.f);
	UI.Shapes.push_back(ShapeSetter(15.f, Vector2f(855.f, 132.f), Color::Black, Color::Transparent, 0.f, 3)); // Left arrow to change game mode.
	UI.Shapes[1].setOrigin(Vector2f(15.f, 15.f));
	UI.Shapes[1].rotate(270.f);
	UI.Shapes.push_back(ShapeSetter(7.f, Vector2f(850.f, 166.f), Color::White, Color::Black, 2.f, 0)); // Change speed circle.

	// -------------------------- LEADERBOARD UI ----------------------------------------
	UI.Texts.push_back(TextSetter(font, 40, 4.f, Color::Red, Color::Black, "LEADERBOARD", Vector2f(825.f, 40.f))); // Leaderboard text.
	UI.Texts.push_back(TextSetter(font, 30, 3.f, Color::White, Color::Black, "PLAYER NAME", Vector2f(650.f, 90.f))); // Player name text.
	UI.Texts.push_back(TextSetter(font, 30, 3.f, Color::White, Color::Black, "PLAYER SCORE", Vector2f(1060.f, 90.f))); // Player score text.
}