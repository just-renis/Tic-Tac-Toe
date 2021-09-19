#include "P3_Pipe.h"
#include "VoidFunctions.h"

P3_Pipe::P3_Pipe(const pair<RectangleShape, RectangleShape>& body)
	: body{body}
{

}

void P3_Pipe::pipeLogic(Clock& pipeGenerationTime, vector<P3_Pipe*>& pipes, vector<P3_Bird*>& birds)
{
	if (pipeGenerationTime.getElapsedTime().asSeconds() >= 2.5f) pipes.push_back(P3_Pipe::generatePipe()), pipeGenerationTime.restart(); // Generate pipe every 2.5 second.
	for (int i{ 0 }; i < pipes.size(); i++) pipes[i]->movement(); // Pipes movement.
	if (pipes.size() > 0) P3_Pipe::collision(pipes, birds);
}

void P3_Pipe::draw(RenderWindow& window, const vector<P3_Pipe*> pipes)
{
	for (int i{ 0 }; i < pipes.size(); i++) window.draw(pipes[i]->body.first), window.draw(pipes[i]->body.second);
}

// ------------------------------------------------------------ PRIVATE FUNCTIONS --------------------------------------------------------------

P3_Pipe* P3_Pipe::generatePipe()
{
	Vector2f size = V2f(100.f, rand() % static_cast<int>(WINDOW_HEIGHT - 250.f)); // Generate upper pipe size.
	RectangleShape firstHalf = RectangleSetter(size, Vector2f(WINDOW_WIDTH, 0.f), Color::Green, Color::White, 0.f); // Generate upper pipe part.
	float secondSize = FOh(WINDOW_HEIGHT - size.y - 200.f); // Generate bottom pipe size.
	RectangleShape secondHalf = RectangleSetter(Vector2f(size.x, secondSize), Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - secondSize), Color::Green, Color::White, 0.f); // Generate bottom pipe part.
	return new P3_Pipe(make_pair(firstHalf, secondHalf)); // Create the pipe and return it.
}

void P3_Pipe::movement()
{
	this->body.first.move(V2f(-1.f, 0.f));
	this->body.second.move(V2f(-1.f, 0.f));
}

void P3_Pipe::collision(vector<P3_Pipe*>& pipes, vector<P3_Bird*>& birds)
{
	for (int i = 0; i < birds.size(); i++)
	{
		if (birds[i]->getBounds().intersects(pipes[0]->body.first.getGlobalBounds()) || birds[i]->getBounds().intersects(pipes[0]->body.second.getGlobalBounds()))
		{
			delete birds[i];
			birds.erase(birds.begin() + i);
		}
	}
	if (pipes[0]->body.first.getPosition().x <= -pipes[0]->body.first.getGlobalBounds().width)
	{
		for (int i = 0; i < birds.size(); i++) birds[i]->addScore();
		delete pipes[0];
		pipes.erase(pipes.begin());
	}
}