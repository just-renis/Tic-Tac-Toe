#include "Player.h"

Player::Player(Font* font, string id, Color BodyColor)
{
	this->ID = id;
	this->body.setSize(V2f(40.f, 40.f));
	this->body.setOutlineColor(Color::Black);
	this->body.setOutlineThickness(FOw(2.f));
	this->Score = 0;
	this->ScoreText = TextSetter(font, FOw(30), 0.f, Color::White, Color::Transparent, id + ": 0", Vector2f(0.f, FOh(1050.f)));
	this->BodyColor = BodyColor;
	this->YouText = TextSetter(font, FOw(15), 0.f, Color::Black, Color::Transparent, "YOU", V2f(5.f, 10.f));
	this->Speed = 1.f;
	this->PlayerMoved = false;
	for (int i = 0; i < 6; i++) PowTimes.push_back(Clock());
}

bool Player::StillOn(float Secs, int Which)
{
	if (this->PowTimes[Which].getElapsedTime().asSeconds() < Secs) return true;
	return false;
}

void Player::ResetTime(int Which)
{
	this->PowTimes[Which].restart();
}

void Player::SetBodyColor()
{
	this->body.setFillColor(this->BodyColor);
	this->body.setOutlineColor(Color::Black);
}

void Player::UpMovement()
{
	if (body.getPosition().y - FOh(20.f * Speed) < 0.f) body.setPosition(body.getPosition().x, 0.f), YouText.setPosition(YouText.getPosition().x, FOh(10.f));
	else body.move(0.f, -FOh(20.f * Speed)), YouText.move(0.f, -FOh(20.f * Speed));
	PlayerMoved = true;
}

void Player::DownMovement()
{
	if (body.getPosition().y + FOh(20.f * Speed) > WINDOW_HEIGHT - body.getGlobalBounds().height) body.setPosition(body.getPosition().x, WINDOW_HEIGHT - body.getGlobalBounds().height), YouText.setPosition(YouText.getPosition().x, WINDOW_HEIGHT - body.getGlobalBounds().height + FOh(10.f));
	else body.move(0.f, FOh(20.f * Speed)), YouText.move(0.f, FOh(20.f * Speed));
	PlayerMoved = true;
}

void Player::LeftMovement()
{
	if (body.getPosition().x - FOw(20.f * Speed) < 0.f) body.setPosition(0.f, body.getPosition().y), YouText.setPosition(FOw(5.f), YouText.getPosition().y);
	else body.move(-FOw(20.f * Speed), 0.f), YouText.move(-FOw(20.f * Speed), 0.f);
	PlayerMoved = true;
}

void Player::RightMovement()
{
	if (body.getPosition().x + FOw(20.f * Speed) > WINDOW_WIDTH - body.getGlobalBounds().width) body.setPosition(WINDOW_WIDTH - body.getGlobalBounds().width, body.getPosition().y), YouText.setPosition(WINDOW_WIDTH - body.getGlobalBounds().width + FOw(5.f), YouText.getPosition().y);
	else body.move(FOw(20.f * Speed), 0.f), YouText.move(FOw(20.f * Speed), 0.f);
	PlayerMoved = true;
}