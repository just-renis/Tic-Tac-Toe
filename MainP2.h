#pragma once
#include "MouseClick.h"
#include "Chunk.h"
#include "UI_P2.h"

void initChunks(RenderWindow& window, vector<vector<Chunk*>>& chunks, Font* font); // Initializing chunks function.
void initUI(UI_P2& UI); // Initializing all UI function.

void MainP2Loop(RenderWindow& window, Event& e, int& mainState, int& depthState, Font* font)
{
	bool mainLoop{ true }, lostFocusOnWindow{ false };
	Vector2i mousePos;
	MouseClick mouse("water");
	vector<vector<Chunk*>> chunks; // Chunks container.
	UI_P2 UI; // UI container.
	initUI(UI); // Initialize UI.
	initChunks(window, chunks, font); // Initialize chunks.
	while (window.isOpen() && mainLoop)
	{
		mousePos = Mouse::getPosition(window);
		// ---------------------------------------------------------------- EVENTS ----------------------------------------------------------------------------
		while (window.pollEvent(e))
		{
			if (e.type == Event::LostFocus) lostFocusOnWindow = true;
			else if (e.type == Event::GainedFocus) lostFocusOnWindow = false;
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) // Quitting program.
			{
				depthState = 2;
				mainState = 0;
				mainLoop = false;
			}
		}

		// ----------------------------------------------------------- MOUSE CLICK EVENTS -------------------------------------------------------------------
		if (Mouse::isButtonPressed(Mouse::Left)) mouse.click(mousePos, '2', chunks); // Creates fluids with mouse clicks.
		else if (Mouse::isButtonPressed(Mouse::Right)) // Randomizes color of every chunk fluids.
		{
			for (int i{ 0 }; i < chunks.size(); i++)
			{
				for (int j{ 0 }; j < chunks[i].size(); j++) chunks[i][j]->changeChunkColor();
			}
		}

		// ---------------------------------------------------------------- DRAW --------------------------------------------------------------------------------
		window.clear(Color::Black);
		for (int i{ 0 }; i < chunks.size(); i++) // Drawing all chunks.
		{
			for (int j{ 0 }; j < chunks[i].size(); j++)
			{
				chunks[i][j]->drawChunk(window);
			}
		}
		window.display();
	}
	for (int i{ 0 }; i < chunks.size(); i++) // Deleting all chunks.
	{
		for (int j{ 0 }; j < chunks[i].size(); j++)
		{
			delete chunks[i][j];
		}
	}
}

void initChunks(RenderWindow& window, vector<vector<Chunk*>>& chunks, Font* font)
{
	RectangleShape loadingFrame{ RectangleSetter(V2f(500.f, 50.f), V2f(710.f, 515.f), Color::White, Color::Blue, FOw(3.f))}; // Loading bar frame.
	RectangleShape loadingBar{ RectangleSetter(V2f(0.f, 50.f), V2f(710.f, 515.f), Color::Blue, Color::Transparent, 0.f) }; // Loading bar.
	Text percentageText{TextSetter(font, FOw(30), 0.f, Color::Black, Color::Transparent, "0 %", V2f(930.f, 520.f))}; // Percentage text.
	Text loadingText{ TextSetter( font, FOw(30), 0.f, Color::White, Color::Transparent, "Chunks loaded: 0", V2f(800.f, 575.f)) }; // Loading chunks text.
	int quantity{ 0 }; // Quantity of chunks.
	int percentage{ 0 }; // Percentage of loaded chunks.
	while (window.isOpen() && percentage < 100)
	{
		for (int i{ 0 }; i < 96; i++)
		{
			chunks.push_back(vector<Chunk*>{}); // Creating chunk row.
			for (int j{ 0 }; j < 54; j++)
			{
				chunks[i].push_back(new Chunk{ Vector2f{i * 20.f, j * 20.f} }); // Creating all chunks in a row.
				quantity++;
			}
			percentage = quantity / 51.84f; // Calculating percentage.
			percentageText.setString(to_string(percentage) + " %"); // Setting string of percentage.
			loadingText.setString("Chunks loaded: " + to_string(quantity)); // Setting string of how many chunks has been loaded.
			loadingBar.setSize(Vector2f(percentage * (WINDOW_WIDTH / 384.f), FOh(50.f))); // Setting size of loaded percentage bar.
			window.clear(Color::Black);
			window.draw(loadingFrame);
			window.draw(loadingBar);
			window.draw(loadingText);
			window.draw(percentageText);
			window.display();
		}
	}
}

void initUI(UI_P2& UI)
{

}