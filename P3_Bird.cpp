#include "P3_Bird.h"
#include "VoidFunctions.h"

P3_Bird::P3_Bird(Font* font)
	: body{ RectangleSetter(V2f(50.f, 50.f), V2f(0.f, WINDOW_HEIGHT / 2.f), Color::Red, Color::Blue, FOw(1.f)) }, jumped{false}, score{0},
	scoreText{ TextSetter(font, FOw(20.f), 0.f, Color::Black, Color::White, "0", Vector2f(FOw(20.f), this->body.getPosition().y + FOh(20.f))) }
{
	
}

void P3_Bird::draw(RenderWindow& window, const vector<P3_Bird*> birds)
{
	for (int i{ 0 }; i < birds.size(); i++)
	{
		window.draw(birds[i]->body);
		window.draw(birds[i]->scoreText);
	}
}

void P3_Bird::update(vector<P3_Bird*>& birds)
{
	for (int i{ 0 }; i < birds.size(); i++)
	{
		if (birds[i]->jumped) birds[i]->jump(); // If jump event is happening then bird is jumping.
		else birds[i]->fall(); // Otherwise bird will fall.
		birds[i]->textUpdate();
		if (birds[i]->mapBounds()) // Destroy bird, if he exited of bounds.
		{
			delete birds[i]; // Deletes a pointer of bird in bird array.
			birds.erase(birds.begin() + i);
		}
	}
}

void P3_Bird::eventJump()
{
	this->jumpTime.restart();
	this->jumped = true; // Trigger the jump.
}

// ---------------------------------------------------------- PRIVATE FUNCTIONS --------------------------------------------------------
void P3_Bird::fall()
{
	this->body.move(V2f(0.f, 1.75f));
}

void P3_Bird::jump()
{
	if (this->jumpTime.getElapsedTime().asSeconds() <= 0.25f) this->body.move(V2f(0.f, -2.f));
	else this->jumped = false;
}

bool P3_Bird::mapBounds()
{
	if (this->body.getPosition().y <= 0.f || this->body.getPosition().y >= FOh(1080.f - this->body.getGlobalBounds().height)) return true;
	return false;
}

void P3_Bird::textUpdate()
{
	this->scoreText.setPosition(FOw(18.f), this->body.getPosition().y + FOh(12.f));
	this->scoreText.setString(to_string(this->score));
}