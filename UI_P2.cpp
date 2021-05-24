#include "UI_P2.h"

void UI_P2::addElement(const Text& text, bool drawIt, const string& string)
{
	this->texts[string] = make_pair(drawIt, text); // Creating "string" key and making pair of is it drawable text now or not and the text itself.
}

void UI_P2::addElement(const RectangleShape& rect, bool drawIt, const string& string)
{
	this->rects[string] = make_pair(drawIt, rect); // Creating "string" key and making pair of is it drawable rectangle now or not and the rectangle itself.
}

void UI_P2::drawUI(RenderWindow& window, UI_P2& UI)
{
	for (UI.rectsIt = UI.rects.begin(); UI.rectsIt != UI.rects.end(); UI.rectsIt++) // Draw all rectangles.
	{
		if (UI.rectsIt->second.first) window.draw(UI.rectsIt->second.second); // If it can be drawed then draw rectangle.
	}
	for (UI.textsIt = UI.texts.begin(); UI.textsIt != UI.texts.end(); UI.textsIt++) // Draw all texts.
	{
		if (UI.textsIt->second.first) window.draw(UI.textsIt->second.second); // If it can be draw then draw text.
	}
}