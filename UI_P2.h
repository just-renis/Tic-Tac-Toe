#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

using namespace std;
using namespace sf;

class UI_P2
{
public:
	void addElement(const Text& text, bool drawIt, const string& string); // Adding text.
	void addElement(const RectangleShape& rect, bool drawIt, const string& string); // Adding rectangle.
	static void drawUI(RenderWindow& window, UI_P2& UI); // Drawing all UI.
private:
	unordered_map<string, pair<bool, Text>> texts; // Texts container.
	unordered_map<string, pair<bool, RectangleShape>> rects; // Rectangle container.
	unordered_map<string, pair<bool, Text>>::iterator textsIt; // Iterator of the texts container.
	unordered_map<string, pair<bool, RectangleShape>>::iterator rectsIt; // Iterator of the rectangle container.
};