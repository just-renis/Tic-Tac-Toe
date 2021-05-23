#pragma once
#include "MouseClick.h"
#include "Chunk.h"

void MainP2Loop(RenderWindow& window, Event& e, int& mainState, int& depthState, Font* font)
{
	bool mainLoop{ true }, lostFocusOnWindow{ false };
	Vector2f mousePos;
	MouseClick mouse("water");
	vector<vector<Chunk*>> chunks;
	for (int i{ 0 }; i < 96; i++)
	{
		chunks.push_back(vector<Chunk*>{});
		for (int j{ 0 }; j < 54; j++)
		{
			chunks[i].push_back(new Chunk{ Vector2f{i * 20.f, j * 20.f}});
		}
	}
	while (window.isOpen() && mainLoop)
	{
		mousePos = Vector2f{ Mouse::getPosition(window) };
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
		if (Mouse::isButtonPressed(Mouse::Left)) mouse.click(mousePos, '2', chunks);
		else if (Mouse::isButtonPressed(Mouse::Right))
		{
			for (int i = 0; i < chunks.size(); i++)
			{
				for (int j = 0; j < chunks[i].size(); j++) chunks[i][j]->changeChunkColor();
			}
		}

		// ---------------------------------------------------------------- DRAW --------------------------------------------------------------------------------
		window.clear(Color::Black);
		for (int i{ 0 }; i < chunks.size(); i++)
		{
			for (int j{ 0 }; j < chunks[i].size(); j++)
			{
				chunks[i][j]->drawChunk(window);
			}
		}
		window.display();
	}
	for (int i{ 0 }; i < chunks.size(); i++)
	{
		for (int j{ 0 }; j < chunks[i].size(); j++)
		{
			delete chunks[i][j];
		}
	}
}