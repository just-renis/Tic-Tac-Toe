#pragma once
#include "Program.h"

class P1_Tile
{
private:
	RectangleShape tile;
	int fromStart;
	bool isObstacle;
	bool isFinish;
	bool isStart;
	bool alreadyChecked;
	static Vector2i finishTile;
	static Vector2i startTile;
	static Color obstacleColor;
	static Color startColor;
	static Color finishColor;
	static Color pathColor;
	static Vector2i previousPos;
	static int width;
	static int height;
// ------------------------------------------------------------- FUNCTIONS -------------------------------------------------------------
public:
	P1_Tile(const RectangleShape& rect, bool isObstacle);
	static Vector2i getStartTileLocation(const vector<vector<P1_Tile>>& map);
	static void mapSizeSetter(int width, int height);
	static void resetPreviousPos(Vector2i pos);
	static void draw(RenderWindow& window, const vector<vector<P1_Tile>>& map, FloatRect screenView);
	static void clear(vector<vector<P1_Tile>>& map, FloatRect& screenView, View& view, bool& foundFinish, vector<Vector2i>& toColor, bool& pause, bool& drawUI31);
	static void reset(vector<vector<P1_Tile>>& map, bool& foundFinish, vector<Vector2i>& toColor);
	static void screenBorders(const vector<vector<P1_Tile>>& map, View& view);
	static void constructing(RenderWindow& window, vector<vector<P1_Tile>>& map, Color color);
	static bool programCanStart();
	static void findingAlgorithm(vector<vector<P1_Tile>>& map, vector<Vector2i>& toColor, bool& foundFinish, bool& drawUI31);
	static void colorsSetter(Color obstacleColor, Color startColor, Color finishColor, Color pathColor);
private:
	static void calculatingTiles(vector<vector<P1_Tile>>& map);
	static void foundWay(vector<vector<P1_Tile>>& map);
};