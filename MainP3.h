#pragma once
#include "Program.h"
#include "P3_Pipe.h"

void pointerCleaner(vector<P3_Bird*>& birds, vector<P3_Pipe*>& pipes); // This function deletes all pointers.
void restart(vector<P3_Bird*>& birds, vector<P3_Pipe*>& pipes, Font* font);

void MainP3Loop(RenderWindow& window, Event& e, int& mainState, int& depthState, Font* font)
{
	bool mainLoop{ true };
	Clock pipeGenerationTime;
	vector<P3_Bird*> birds;
	vector<P3_Pipe*> pipes;
	birds.push_back(new P3_Bird(font));

	while (window.isOpen() && mainLoop)
	{
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::KeyPressed) // Quitting program.
			{
				if (e.key.code == Keyboard::Escape)
				{
					depthState = 2;
					mainState = 0;
					mainLoop = false;
				}
				else if (e.key.code == Keyboard::Space) birds[0]->eventJump();
			}
		}

		// ------------------------------------ DRAW ------------------------------------------------
		window.clear(Color::White);
		P3_Pipe::pipeLogic(pipeGenerationTime, pipes, birds);
		P3_Pipe::draw(window, pipes);
		P3_Bird::update(birds);
		P3_Bird::draw(window, birds);
		window.display();
		if (birds.size() == 0) restart(birds, pipes, font);
	}
	pointerCleaner(birds, pipes);
}

void pointerCleaner(vector<P3_Bird*>& birds, vector<P3_Pipe*>& pipes)
{
	for (int i{ 0 }; i < birds.size(); i++) delete birds[i];
	for (int i{ 0 }; i < pipes.size(); i++) delete pipes[i];
}

void restart(vector<P3_Bird*>& birds, vector<P3_Pipe*>& pipes, Font* font)
{
	pointerCleaner(birds, pipes);
	pipes.clear();
	birds.push_back(new P3_Bird(font));
}