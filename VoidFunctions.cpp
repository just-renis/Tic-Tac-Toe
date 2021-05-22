#pragma once
#include "VoidFunctions.h"

Text TextSetter(Font* font, int charSize, float outlineThickness, Color fillColor, Color outlineColor, const string& string, Vector2f pos)
{
	Text text;
	text.setFont(*font);
	text.setCharacterSize(charSize);
	text.setFillColor(fillColor);
	text.setOutlineColor(outlineColor);
	text.setOutlineThickness(outlineThickness);
	text.setPosition(pos);
	text.setString(string);
	return text;
}

RectangleShape RectangleSetter(Vector2f size, Vector2f pos, Color fillColor, Color outlineColor, float outlineThickness)
{
	RectangleShape rect(size);
	rect.setPosition(pos);
	rect.setFillColor(fillColor);
	rect.setOutlineColor(outlineColor);
	rect.setOutlineThickness(outlineThickness);
	return rect;
}

CircleShape ShapeSetter(float radius, Vector2f pos, Color fillColor, Color outlineColor, float outlineThickness, int points)
{
	CircleShape shape(radius);
	if (points != 0) shape.setPointCount(points);
	shape.setPosition(pos);
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
	return shape;
}

void backgroundColorChanger(Color& backgroundColor, Color& next, Clock& backgroundTimer, bool& newBackgroundColorNeeded)
{
	if (newBackgroundColorNeeded)
	{
		next = Color(rand() % 255, rand() % 255, rand() % 255);
		newBackgroundColorNeeded = false;
	}
	else if (backgroundTimer.getElapsedTime().asSeconds() > 0.01f)
	{
		if (next == backgroundColor) newBackgroundColorNeeded = true;
		else
		{
			if (next.r > backgroundColor.r) backgroundColor.r++;
			else if (next.r < backgroundColor.r) backgroundColor.r--;
			if (next.g > backgroundColor.g) backgroundColor.g++;
			else if (next.g < backgroundColor.g) backgroundColor.g--;
			if (next.b > backgroundColor.b) backgroundColor.b++;
			else if (next.b < backgroundColor.b) backgroundColor.b--;
		}
		backgroundTimer.restart();
	}
}

Vector2f V2f(float x, float y)
{
	return Vector2f(WINDOW_WIDTH / 1920.f * x, WINDOW_HEIGHT / 1080.f * y);
}