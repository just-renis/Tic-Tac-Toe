#pragma once
#include "MouseClick.h"

void MainP2Loop(RenderWindow& window, Event& e, int& mainState, int& depthState, Font* font)
{
	bool mainLoop{ true }, lostFocusOnWindow{ false };
	Vector2f mousePos;
	MouseClick mouse("water");
	vector<Fluid> fluids;
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
		if (Mouse::isButtonPressed(Mouse::Left)) mouse.click(mousePos, '2', fluids);

		// ---------------------------------------------------------------- DRAW --------------------------------------------------------------------------------
		window.clear(Color::Black);
		Fluid::drawFluids(window, fluids);
		window.display();
	}
}