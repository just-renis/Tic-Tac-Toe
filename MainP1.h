#pragma once
#include "Program.h"
#include "P1_Tile.h"
#include "UI.h"
#include <queue>

void initTiles(vector<vector<P1_Tile>>& map, int width, int height, Color pathColor);
void constructingSimulation(UI& UI, bool& grantedPermission, float& findingSpeed, vector<vector<P1_Tile>>& map, int& state, Color obstacleColor, Color startColor, Color finishColor, Color pathColor);

// ---------------------------------------------------- UI FUNCTIONS --------------------------------------------
UI initUI1(Font* font, TexturesMap& texs);
void takingInput(const Event& e, UI& UI);
void whichSquare(UI& UI, Vector2f mousePos, bool& grantedPermission);
void selectedTile(UI& UI, Vector2f mousePos);
Vector2i width_Height(const UI& UI);
string simulationSpeedSetter(float x, float linePos, float lineLength);
void draggingSimulationSetter(Vector2f mousePos, UI& UI);
void whichTriangleSelected(int& whichTriangle, bool& draggingRGB, Vector2f mousePos, const UI& UI);
void draggingRGB_setter(Vector2f mousePos, UI& UI, int whichTriangle);
int RGB_setter(float x, float linePos, float lineLength);
void increaseRGBButtons(Vector2f mousePos, UI& UI);
Color whatColorIsSettedUp(const UI& UI);
void applyTheColor(UI& UI);
void resetToDefaultColors(UI& UI);

void MainP1Loop(RenderWindow& window, Event& e, int& mainState, int& depthState, TexturesMap& texs, SoundsMap& buffers, Font* font)
{
	View view(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	FloatRect screenView(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT);
	bool mainLoop = true;
	int state = 0;
	int whichTriangle; // UI which triangle is dragged.
	Vector2f mousePos;
	UI UI = initUI1(font, texs); // Whole UI.
	Color backgroundColor = Color::Black, next;
	Clock backgroundTimer;
	bool newBackgroundColorNeeded = true;
	bool grantedPermission = false; // Granted permission to write numbers.
	bool draggingSimulationSpeed = false, draggingRGB = false; // For dragging UI some kinds of shapes.
	float findingSpeed = 0.f; // Simulation speed.
	Clock findingClock; // Simulation speed time checker.
	bool pause = false;
	bool lostFocusOnWindow = false;
	bool drawUI31 = false;
	// ---------------------------------------------- TILES ----------------------------------------------
	vector<vector<P1_Tile>> map;
	vector<Vector2i> toColor;
	bool foundFinish = false;
	// ----------------------------------------------------------------- WINDOW IS OPEN -----------------------------------------------------------------
	while (window.isOpen() && mainLoop)
	{
		mousePos = (Vector2f(Mouse::getPosition(window)));
		// ---------------------------------------------------------------- EVENTS ----------------------------------------------------------------------------
		while (window.pollEvent(e))
		{
			if (e.type == Event::LostFocus) lostFocusOnWindow = true;
			else if (e.type == Event::GainedFocus) lostFocusOnWindow = false;
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
			{
				if (state == 0)
				{
					depthState = 2;
					mainState = 0;
					mainLoop = false;
					pause = false;
				}
			}
			if (!lostFocusOnWindow)
			{
				if (e.type == Event::KeyPressed)
				{
					if (e.key.code == Keyboard::Escape)
					{
						if (state >= 1) state = 0, P1_Tile::clear(map, screenView, view, foundFinish, toColor, pause, drawUI31);
					}
					else if (e.key.code == Keyboard::Enter) // If program can start.
					{
						if (state == 0) constructingSimulation(UI, grantedPermission, findingSpeed, map, state, UI.Rects[12].getFillColor(), UI.Rects[13].getFillColor(), UI.Rects[14].getFillColor(), UI.Rects[15].getFillColor());
						else if (state == 1)
						{
							if (P1_Tile::programCanStart()) state = 2, toColor.push_back(P1_Tile::getStartTileLocation(map));
						}
					}
					else if (e.key.code == Keyboard::P) // Pause/unpause simulation.
					{
						if (state == 2) pause = !pause;
					}
				}
				else if (e.type == Event::MouseButtonPressed)
				{
					if (e.key.code == Mouse::Left)
					{
						if (state == 0)
						{
							increaseRGBButtons(mousePos, UI);
							whichSquare(UI, mousePos, grantedPermission);
							whichTriangleSelected(whichTriangle, draggingRGB, mousePos, UI);
							if (UI.Rects[5].getGlobalBounds().contains(mousePos)) constructingSimulation(UI, grantedPermission, findingSpeed, map, state, UI.Rects[12].getFillColor(), UI.Rects[13].getFillColor(), UI.Rects[14].getFillColor(), UI.Rects[15].getFillColor()); // Start the simulation
							else if (UI.Shapes[0].getGlobalBounds().contains(mousePos)) draggingSimulationSpeed = true;
							else if (UI.Rects[23].getGlobalBounds().contains(mousePos)) applyTheColor(UI);
							else if (UI.Rects[24].getGlobalBounds().contains(mousePos)) resetToDefaultColors(UI);
							else selectedTile(UI, mousePos);
						}
					}
				}
				else if (e.type == Event::MouseButtonReleased) // Stop the dragging simulation speed setter.
				{
					P1_Tile::resetPreviousPos(Vector2i(-1, -1));
					draggingSimulationSpeed = false;
					draggingRGB = false;
				}
				if (grantedPermission) takingInput(e, UI); // Writing height/width of map size.
			}
		}
		// --------------------------------------------------------- EVENTS END ------------------------------------------------------
		if (!lostFocusOnWindow)
		{
			if (state == 0)
			{
				if (draggingSimulationSpeed) draggingSimulationSetter(mousePos, UI); // Dragging simulation speed setter.
				else if (draggingRGB) // When dragging RGB setters.
				{
					draggingRGB_setter(mousePos, UI, whichTriangle); // Move triangle with the setter lines.
					UI.Texts[whichTriangle + 17].setString(to_string(RGB_setter(UI.Shapes[whichTriangle].getPosition().x, UI.Rects[whichTriangle + 15].getPosition().x, UI.Rects[whichTriangle + 15].getSize().x))); // Change RGB number values.
					for (int i = 18; i < 21; i++) UI.Texts[i].setPosition(FOw(1465.f - 11.f * (UI.Texts[i].getString().getSize() - 1)), UI.Texts[i].getPosition().y); // Align text to the center.
					UI.Rects[22].setFillColor(whatColorIsSettedUp(UI));
				}
			}
			else if (state == 1) // Drawing labyrinth.
			{
				if (Mouse::isButtonPressed(Mouse::Left)) P1_Tile::constructing(window, map, UI.Rects[12].getFillColor());
				else if (Mouse::isButtonPressed(Mouse::Right)) P1_Tile::constructing(window, map, UI.Rects[13].getFillColor());
				else if (Mouse::isButtonPressed(Mouse::Button::Middle) || Keyboard::isKeyPressed(Keyboard::E)) P1_Tile::constructing(window, map, UI.Rects[14].getFillColor());
				P1_Tile::screenBorders(map, view);
			}
			else
			{
				if (!pause)
				{
					P1_Tile::screenBorders(map, view);
					if (!foundFinish && findingClock.getElapsedTime().asSeconds() >= findingSpeed) // Simulation in process.
					{
						P1_Tile::findingAlgorithm(map, toColor, foundFinish, drawUI31);
						findingClock.restart();
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::R)) // To reset simulation.
				{
					P1_Tile::reset(map, foundFinish, toColor);
					state = 1;
					pause = false;
					drawUI31 = false;
				}
			}
		}
		// ------------------------------------------------------------------- DRAW ----------------------------------------------------------------------------
		backgroundColorChanger(backgroundColor, next, backgroundTimer, newBackgroundColorNeeded);
		window.clear(backgroundColor);
		switch (state)
		{
			case 0: // Menu/settings.
				for (int i = 0; i < UI.Rects.size(); i++) window.draw(UI.Rects[i]);
				for (int i = 0; i < UI.Texts.size() - 1; i++) window.draw(UI.Texts[i]);
				for (int i = 0; i < UI.Shapes.size(); i++) window.draw(UI.Shapes[i]);
				for (int i = 0; i < UI.Sprites.size(); i++) window.draw(UI.Sprites[i]);
				break;
			default: // Simulation.
				screenView.height = view.getSize().y;
				screenView.width = view.getSize().x;
				screenView.left = view.getCenter().x - WINDOW_WIDTH / 2;
				screenView.top = view.getCenter().y - WINDOW_HEIGHT / 2;
				P1_Tile::draw(window, map, screenView);
				if (drawUI31)
				{
					UI.Texts[31].setPosition(view.getCenter().x, view.getCenter().y);
					window.draw(UI.Texts[31]);
				}
		}
		window.setView(view);
		window.display();
	}
}

void initTiles(vector<vector<P1_Tile>>& map, int width, int height, Color pathColor)
{
	float x = WINDOW_WIDTH / width, y = WINDOW_HEIGHT / height;
	Color outlineColor = Color::Black;
	if (pathColor.r + pathColor.g + pathColor.b <= 150) outlineColor = Color::White;
	if (y < 22 || x < 22) y = 22, x = 22;
	else if (y < x) x = y;
	else
	{
		if (x * height <= WINDOW_HEIGHT) y = x;
		else x = y;
	}
	for (int i = 0; i < height; i++)
	{
		map.push_back(vector<P1_Tile>());
		for (int j = 0; j < width; j++)
		{
			map[i].push_back(P1_Tile(RectangleSetter(Vector2f(x - 2.f, y - 2.f), Vector2f(1.f + j * x, 1.f + i * y), pathColor, outlineColor, 1.f), false));
		}
	}
}

void constructingSimulation(UI& UI, bool& grantedPermission, float& findingSpeed, vector<vector<P1_Tile>>& map, int& state, Color obstacleColor, Color startColor, Color finishColor, Color pathColor)
{
	UI.Rects[3].setFillColor(Color::White);
	UI.Rects[4].setFillColor(Color::White);
	grantedPermission = false;
	Vector2i WaH = width_Height(UI);
	string simulationSpeed = UI.Texts[9].getString();
	findingSpeed = stof(simulationSpeed);
	P1_Tile::mapSizeSetter(WaH.x, WaH.y);
	P1_Tile::colorsSetter(obstacleColor, startColor, finishColor, pathColor);
	initTiles(map, WaH.x, WaH.y, pathColor);
	state = 1;
}

// ------------------------------------------------------------------------------- UI FUNCTIONS -----------------------------------------------------------------------------------

UI initUI1(Font* font, TexturesMap &texs)
{
	UI UI;
	// ---------------------------------------------------- TEXTS ------------------------------------------------------
	UI.Texts.push_back(TextSetter(font, FOw(25), 0.f, Color::Black, Color::White, "Width:", V2f(890.f, 845.f))); // 0 Width keyword.
	UI.Texts.push_back(TextSetter(font, FOw(25), 0.f, Color::Black, Color::White, "Height:", V2f(890.f, 895.f))); // 1 Height keyword.
	UI.Texts.push_back(TextSetter(font, FOw(20), 0.f, Color::Black, Color::White, "", V2f(970.f, 850.f))); // 2 Writing width.
	UI.Texts.push_back(TextSetter(font, FOw(20), 0.f, Color::Black, Color::White, "", V2f(970.f, 900.f))); // 3 Writing height.
	UI.Texts.push_back(TextSetter(font, FOw(180), FOw(15.f), Color::White, Color::Black, "PATHFINDER", V2f(420.f, -20.f))); // 4 Pathfinder keyword.
	UI.Texts.push_back(TextSetter(font, FOw(40), FOw(3.f), Color::Yellow, Color::Black, "Start the simulation", V2f(800.f, 955.f))); // 5 Start keyword.
	UI.Texts.push_back(TextSetter(font, FOw(45), FOw(2.f), Color::Red, Color::Black, "Map Size", V2f(865.f, 780.f))); // 6 Map size keyword.
	UI.Texts.push_back(TextSetter(font, FOw(60), FOw(4.f), Color::Red, Color::Black, "Simulation settings", V2f(310.f, 190.f))); // 7 Simulation settings keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), FOw(2.f), Color::Black, Color::White, "Simulation speed:", V2f(230.f, 270.f))); // 8 Simulation speed keyword.
	UI.Texts.push_back(TextSetter(font, FOw(25), 0.f, Color::Black, Color::Transparent, "0.000", V2f(482.f, 275.f))); // 9 Simulation speed.
	UI.Texts.push_back(TextSetter(font, FOw(60), FOw(4.f), Color::Red, Color::Black, "Color Settings", V2f(1160.f, 190.f))); // 10 Color settings keyword.
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "Obstacles", V2f(1040.f, 310.f))); // 11 obstacles keyword.
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "Start Point", V2f(1040.f, 410.f))); // 12 Start point keyword.
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "End Point", V2f(1040.f, 510.f))); // 13 End point keyword.
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "Path Tiles", V2f(1040.f, 610.f))); // 14 Path tiles keyword.
	UI.Texts.push_back(TextSetter(font, FOw(50), 0.f, Color::Red, Color::Transparent, "R", V2f(1460.f, 255.f))); // 15 R keyword.
	UI.Texts.push_back(TextSetter(font, FOw(50), 0.f, Color::Green, Color::Transparent, "G", V2f(1460.f, 405.f))); // 16 G keyword.
	UI.Texts.push_back(TextSetter(font, FOw(50), 0.f, Color::Blue, Color::Transparent, "B", V2f(1460.f, 555.f))); // 17 B keyword.
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "0", V2f(1465.f, 332.f))); // 18 R number.
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "0", V2f(1465.f, 482.f))); // 19 G number.
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "0", V2f(1465.f, 632.f))); // 20 B number.
	UI.Texts.push_back(TextSetter(font, FOw(40), 0.f, Color::Black, Color::Transparent, "Apply", V2f(1432.f, 682.f))); // 21 Apply keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), 0.f, Color::Black, Color::Transparent, "Reset to default", V2f(1026.f, 688.f))); // 22 Reset to default keyword.
	UI.Texts.push_back(TextSetter(font, FOw(32), 0.f, Color::White, Color::Transparent, "WARNING: When changing colors, do not make very similiar colors to each other, otherwise you won't experience the pathfinder fully.", V2f(50.f, 1020.f))); // 23 WARNING keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), FOw(2.f), Color::Black, Color::White, "Obstacle: Left mouse button", V2f(230.f, 780.f))); // 24 Tutorial:obstacles keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), FOw(2.f), Color::Black, Color::White, "Start: Right mouse button", V2f(230.f, 810.f))); // 25 Tutorial:Start keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), FOw(2.f), Color::Black, Color::White, "Finish: Mouse wheel button or Press E", V2f(230.f, 840.f))); // 26 Tutorial:Finish keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), FOw(2.f), Color::Black, Color::White, "Change color: Click on names", V2f(230.f, 870.f))); // 27 Tutorial:Color changer keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), FOw(2.f), Color::Black, Color::White, "Start simulation: Press ENTER", V2f(230.f, 900.f))); // 28 Tutorial:Start simulation keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), FOw(2.f), Color::Black, Color::White, "Restart simulation: Press R", V2f(230.f, 930.f))); // 29 Tutorial:Restart simulation keyword.
	UI.Texts.push_back(TextSetter(font, FOw(30), FOw(2.f), Color::Black, Color::White, "Pause simulation: Press P", V2f(230.f, 960.f))); // 30 Tutorial:Pause simulation keyword.
	UI.Texts.push_back(TextSetter(font, FOw(200), FOw(10.f), Color::Black, Color::White, "THERE IS NO PATH!!!", V2f(10.f, 400.f))); // 31 There is no path keyword.
	UI.Texts[31].setOrigin(UI.Texts[31].getLocalBounds().width / 2.f, UI.Texts[31].getLocalBounds().height / 2.f);

	// --------------------------------------------------- RECTS --------------------------------------------------------
	UI.Rects.push_back(RectangleSetter(V2f(700.f, 550.f), V2f(1000.f, 190.f), Color(218, 2, 5, 255), Color::Black, FOw(5.f))); // 0 Board for UI.
	UI.Rects.push_back(RectangleSetter(V2f(700.f, 550.f), V2f(220.f, 190.f), Color(218, 2, 5, 255), Color::Black, FOw(5.f))); // 1 Board for UI.
	UI.Rects.push_back(RectangleSetter(V2f(1480.f, 220.f), V2f(220.f, 780.f), Color(218, 2, 5, 255), Color::Black, FOw(5.f))); // 2 Board for UI.
	UI.Rects.push_back(RectangleSetter(V2f(50.f, 25.f), V2f(970.f, 850.f), Color::White, Color::Black, FOw(2.f))); // 3 Width of tiles.
	UI.Rects.push_back(RectangleSetter(V2f(50.f, 25.f), V2f(970.f, 900.f), Color::White, Color::Black, FOw(2.f))); // 4 Height of tiles.
	UI.Rects.push_back(RectangleSetter(V2f(370.f, 45.f), V2f(790.f, 955.f), Color::Transparent, Color::Black, FOw(4.f))); // 5 Start the program square.
	UI.Rects.push_back(RectangleSetter(V2f(67.f, 28.f), V2f(480.f, 275.f), Color::White, Color::Black, FOw(2.f))); // 6 Simulation speed square.
	UI.Rects.push_back(RectangleSetter(V2f(300.f, 2.f), V2f(570.f, 290.f), Color::Black, Color::Transparent, 0.f)); // 7 Simulation speed line.
	UI.Rects.push_back(RectangleSetter(V2f(200.f, 40.f), V2f(1030.f, 315.f), Color::White, Color::Black, FOw(2.f))); // 8 obstacles square.
	UI.Rects.push_back(RectangleSetter(V2f(200.f, 40.f), V2f(1030.f, 415.f), Color::White, Color::Black, FOw(2.f))); // 9 Start point square.
	UI.Rects.push_back(RectangleSetter(V2f(200.f, 40.f), V2f(1030.f, 515.f), Color::White, Color::Black, FOw(2.f))); // 10 End point square.
	UI.Rects.push_back(RectangleSetter(V2f(200.f, 40.f), V2f(1030.f, 615.f), Color::White, Color::Black, FOw(2.f))); // 11 Path tiles square.
	UI.Rects.push_back(RectangleSetter(V2f(20.f, 20.f), V2f(1250.f, 325.f), Color::Black, Color::Transparent, 0.f)); // 12 obstacles color showcase.
	UI.Rects.push_back(RectangleSetter(V2f(20.f, 20.f), V2f(1250.f, 425.f), Color::Red, Color::Transparent, 0.f)); // 13 Start point color showcase.
	UI.Rects.push_back(RectangleSetter(V2f(20.f, 20.f), V2f(1250.f, 525.f), Color::Green, Color::Transparent, 0.f)); // 14 End point color showcase.
	UI.Rects.push_back(RectangleSetter(V2f(20.f, 20.f), V2f(1250.f, 625.f), Color::White, Color::Transparent, 0.f)); // 15 Path tiles color showcase.
	UI.Rects.push_back(RectangleSetter(V2f(260.f, 45.f), V2f(1345.f, 335.f), Color::White, Color::Red, FOw(4.f))); // 16 R square.
	UI.Rects.push_back(RectangleSetter(V2f(260.f, 45.f), V2f(1345.f, 485.f), Color::White, Color::Green, FOw(4.f))); // 17 G square.
	UI.Rects.push_back(RectangleSetter(V2f(260.f, 45.f), V2f(1345.f, 635.f), Color::White, Color::Blue, FOw(4.f))); // 18 B square.
	UI.Rects.push_back(RectangleSetter(V2f(5.f, 45.f), V2f(1345.f, 335.f), Color::Red, Color::Transparent, 0.f)); // 19 R setter line.
	UI.Rects.push_back(RectangleSetter(V2f(5.f, 45.f), V2f(1345.f, 485.f), Color::Green, Color::Transparent, 0.f)); // 20 G setter line.
	UI.Rects.push_back(RectangleSetter(V2f(5.f, 45.f), V2f(1345.f, 635.f), Color::Blue, Color::Transparent, 0.f)); // 21 B setter line.
	UI.Rects.push_back(RectangleSetter(V2f(100.f, 100.f), V2f(1575.f, 210.f), Color::Black, Color::Transparent, 0.f)); // 22 What color you get with RGB combination.
	UI.Rects.push_back(RectangleSetter(V2f(150.f, 40.f), V2f(1408.f, 690.f), Color::White, Color::Black, FOw(2.f))); // 23 Apply square.
	UI.Rects.push_back(RectangleSetter(V2f(220.f, 40.f), V2f(1022.f, 690.f), Color::White, Color::Black, FOw(2.f))); // 24 Reset square.

	// ---------------------------------------------------- OTHER SHAPES --------------------------------------------------
	UI.Shapes.push_back(ShapeSetter(FOw(10.f), V2f(560.f, 281.f), Color::White, Color::Black, FOw(3.f), 0)); // Change speed circle.
	UI.Shapes.push_back(ShapeSetter(FOw(14.f), V2f(1348.f, 315.f), Color::Black, Color::Red, FOw(2.f), 3)); // Change R color.
	UI.Shapes.push_back(ShapeSetter(FOw(14.f), V2f(1348.f, 465.f), Color::Black, Color::Green, FOw(2.f), 3)); // Change G color.
	UI.Shapes.push_back(ShapeSetter(FOw(14.f), V2f(1348.f, 615.f), Color::Black, Color::Blue, FOw(2.f), 3)); // Change B color.
	for (int i = 1; i < UI.Shapes.size(); i++)
	{
		UI.Shapes[i].setOrigin(V2f(14.f, 14.f));
		UI.Shapes[i].rotate(180.f);
	}

	// --------------------------------------------------- UI SPRITES -------------------------------------------------------
	for (int i = 0; i < 6; i++)
	{
		UI.Sprites.push_back(Sprite());
		UI.Sprites[i].setScale(V2f(1.f, 1.f));
		if (i % 2 == 0) UI.Sprites[i].setTexture(texs["PlusSign"]);
		else UI.Sprites[i].setTexture(texs["MinusSign"]);
	}
	UI.Sprites[0].setPosition(V2f(1615.f, 332.5f));
	UI.Sprites[1].setPosition(V2f(1615.f, 357.5f));
	UI.Sprites[2].setPosition(V2f(1615.f, 482.5f));
	UI.Sprites[3].setPosition(V2f(1615.f, 507.5f));
	UI.Sprites[4].setPosition(V2f(1615.f, 632.5f));
	UI.Sprites[5].setPosition(V2f(1615.f, 657.5f));
	return UI;
}

void takingInput(const Event& e, UI& UI)
{
	string input;
	if (e.type == Event::TextEntered)
	{
		if (UI.Texts[UI.Which].getString().getSize() < 3)
		{
			if (e.text.unicode >= 48 && e.text.unicode <= 57)
			{
				input = e.text.unicode;
				UI.Texts[UI.Which].setString(UI.Texts[UI.Which].getString() + input);
			}
		}
	}
	if (e.type == Event::KeyPressed)
	{
		if (e.key.code == Keyboard::Backspace)
		{
			input = UI.Texts[UI.Which].getString();
			input.pop_back();
			UI.Texts[UI.Which].setString(input);
		}
		else if (e.key.code == Keyboard::Delete) UI.Texts[UI.Which].setString("");
	}
}

void whichSquare(UI& UI, Vector2f mousePos, bool& grantedPermission) // Width/Height UI squares color changer and permission to write in selected tile.
{
	for (int i = 3; i < 5; i++)
	{
		if (UI.Rects[i].getGlobalBounds().contains(mousePos))
		{
			if (UI.Rects[i].getFillColor() == Color::White) UI.Rects[i].setFillColor(Color::Yellow), grantedPermission = true;
			else UI.Rects[i].setFillColor(Color::White), grantedPermission = false;
			for (int j = 3; j < 5; j++)
			{
				if (UI.Rects[j].getFillColor() == Color::Yellow && i != j) UI.Rects[j].setFillColor(Color::White);
			}
			UI.Which = i - 1;
		}
	}
}

void selectedTile(UI& UI, Vector2f mousePos) // Tiles color changer UI.
{
	for (int i = 8; i < 12; i++)
	{
		if (UI.Rects[i].getGlobalBounds().contains(mousePos))
		{
			if (UI.Rects[i].getFillColor() == Color::White) UI.Rects[i].setFillColor(Color::Black), UI.Texts[i + 3].setFillColor(Color::White);
			else UI.Rects[i].setFillColor(Color::White), UI.Texts[i + 3].setFillColor(Color::Black);
			for (int j = 8; j < 12; j++)
			{
				if (UI.Rects[j].getFillColor() == Color::Black && i != j) UI.Rects[j].setFillColor(Color::White), UI.Texts[j + 3].setFillColor(Color::Black);
			}
		}
	}
}

Vector2i width_Height(const UI& UI)
{
	Vector2i WaH;
	string Number = UI.Texts[2].getString();
	if (Number.size() <= 0 || stoi(Number) <= 1) Number = "2";
	else if (stoi(Number) > 100) Number = "100";
	WaH.x = stoi(Number);
	Number = UI.Texts[3].getString();
	if (Number.size() <= 0 || stoi(Number) <= 1) Number = "2";
	else if (stoi(Number) > 100) Number = "100";
	WaH.y = stoi(Number);
	return WaH;
}

string simulationSpeedSetter(float x, float linePos, float lineLength)
{
	string s = to_string((x - linePos + FOw(10.f)) / lineLength);
	for (int i = 0; i < 3; i++) s.pop_back();
	if (s[0] == '-') s.erase(s.begin());
	return s;
}

void draggingSimulationSetter(Vector2f mousePos, UI& UI)
{
	if (mousePos.x >= FOw(560.f) && mousePos.x <= FOw(860.f)) UI.Shapes[0].setPosition(Vector2f(mousePos.x, UI.Shapes[0].getPosition().y));
	else if (mousePos.x < FOw(560.f)) UI.Shapes[0].setPosition(Vector2f(FOw(560.f), UI.Shapes[0].getPosition().y));
	else UI.Shapes[0].setPosition(Vector2f(FOw(860.f), UI.Shapes[0].getPosition().y));
	UI.Texts[9].setString(simulationSpeedSetter(UI.Shapes[0].getPosition().x, UI.Rects[7].getPosition().x, UI.Rects[7].getSize().x));
}

void whichTriangleSelected(int& whichTriangle, bool& draggingRGB, Vector2f mousePos, const UI& UI)
{
	for (int i = 1; i < 4; i++)
	{
		if (UI.Shapes[i].getGlobalBounds().contains(mousePos))
		{
			whichTriangle = i;
			draggingRGB = true;
			break;
		}
	}
}

void draggingRGB_setter(Vector2f mousePos, UI& UI, int whichTriangle)
{
	if (mousePos.x >= FOw(1348.f) && mousePos.x <= FOw(1603.f))
	{
		UI.Shapes[whichTriangle].setPosition(Vector2f(mousePos.x, UI.Shapes[whichTriangle].getPosition().y));
		UI.Rects[whichTriangle + 18].setPosition(Vector2f(mousePos.x - FOw(3.f), UI.Rects[whichTriangle + 18].getPosition().y));
	}
	else if (mousePos.x < FOw(1348.f))
	{
		UI.Shapes[whichTriangle].setPosition(Vector2f(FOw(1348.f), UI.Shapes[whichTriangle].getPosition().y));
		UI.Rects[whichTriangle + 18].setPosition(Vector2f(FOw(1345.f), UI.Rects[whichTriangle + 18].getPosition().y));
	}
	else
	{
		UI.Shapes[whichTriangle].setPosition(Vector2f(FOw(1603.f), UI.Shapes[whichTriangle].getPosition().y));
		UI.Rects[whichTriangle + 18].setPosition(Vector2f(FOw(1600.f), UI.Rects[whichTriangle + 18].getPosition().y));
	}
}

int RGB_setter(float x, float linePos, float lineLength)
{
	return int((((x - linePos - FOw(3.f)) * 100) / (lineLength - FOw(5.f))) * 2.55f);
}

void increaseRGBButtons(Vector2f mousePos, UI& UI)
{
	int kuris;
	for (int i = 0; i < 6; i++)
	{
		if (UI.Sprites[i].getGlobalBounds().contains(mousePos))
		{
			if (i < 2) kuris = 18;
			else if (i < 4) kuris = 19;
			else kuris = 20;
			string number = UI.Texts[kuris].getString();
			if (i % 2 == 0 && number != "255")
			{
				UI.Texts[kuris].setString(to_string(stoi(number) + 1));
				UI.Shapes[i / 2 + 1].move(FOw(1.f), 0.f);
				UI.Rects[kuris + 1].move(FOw(1.f), 0.f);
			}
			else if (i % 2 != 0 && number != "0")
			{
				UI.Texts[kuris].setString(to_string(stoi(number) - 1));
				UI.Shapes[i / 2 + 1].move(FOw(-1.f), 0.f);
				UI.Rects[kuris + 1].move(FOw(-1.f), 0.f);
			}
			UI.Texts[kuris].setPosition(FOw(1465.f - 11.f * (UI.Texts[kuris].getString().getSize() - 1)), UI.Texts[kuris].getPosition().y); // Align text to the center.
		}
	}
	UI.Rects[22].setFillColor(whatColorIsSettedUp(UI));
}

Color whatColorIsSettedUp(const UI& UI)
{
	string r = UI.Texts[18].getString(), g = UI.Texts[19].getString(), b = UI.Texts[20].getString();
	return Color(stoi(r), stoi(g), stoi(b));
}

void applyTheColor(UI& UI)
{
	for (int i = 8; i < 12; i++)
	{
		if (UI.Rects[i].getFillColor() == Color::Black) UI.Rects[i + 4].setFillColor(UI.Rects[22].getFillColor());
	}
}

void resetToDefaultColors(UI& UI)
{
	UI.Rects[12].setFillColor(Color::Black);
	UI.Rects[13].setFillColor(Color::Red);
	UI.Rects[14].setFillColor(Color::Green);
	UI.Rects[15].setFillColor(Color::White);
}