#pragma once
#include "Program.h"

Text TextSetter(Font* font, int charSize, float outlineThickness, Color fillColor, Color outlineColor, const string& string, Vector2f pos);
RectangleShape RectangleSetter(Vector2f size, Vector2f pos, Color fillColor, Color outlineColor, float outlineThickness);
CircleShape ShapeSetter(float radius, Vector2f pos, Color fillColor, Color outlineColor, float outlineThickness, int points);
void backgroundColorChanger(Color& backgroundColor, Color& next, Clock& backgroundTimer, bool& newBackgroundColorNeeded);
Vector2f V2f(float x, float y);

typedef unordered_map<string, Texture> TexturesMap;
typedef unordered_map<string, SoundBuffer> SoundsMap;

template<class T> T FOh(T Num)
{
	return WINDOW_HEIGHT / 1080 * Num;
}

template<class T> T FOw(T Num)
{
	return WINDOW_WIDTH / 1920 * Num;
}