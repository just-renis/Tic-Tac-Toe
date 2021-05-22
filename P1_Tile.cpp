#include "P1_Tile.h"
#include "VoidFunctions.h"

Vector2i P1_Tile::finishTile = Vector2i(-1, -1);
Vector2i P1_Tile::startTile = Vector2i(-1, -1);
Vector2i P1_Tile::previousPos = Vector2i();
Color P1_Tile::obstacleColor = Color::Black;
Color P1_Tile::startColor = Color::Red;
Color P1_Tile::finishColor = Color::Green;
Color P1_Tile::pathColor = Color::White;
int P1_Tile::width = 0;
int P1_Tile::height = 0;

P1_Tile::P1_Tile(const RectangleShape& rect, bool isObstacle)
{
	this->tile = rect;
	this->isObstacle = isObstacle;
	this->isFinish = false;
	this->isStart = false;
	this->fromStart = 0;
	this->alreadyChecked = false;
}

Vector2i P1_Tile::getStartTileLocation(const vector<vector<P1_Tile>>& map)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++) if (map[i][j].isStart) return Vector2i(j, i);
	}
}

void P1_Tile::mapSizeSetter(int width, int height)
{
	P1_Tile::width = width;
	P1_Tile::height = height;
}

void P1_Tile::resetPreviousPos(Vector2i pos)
{
	P1_Tile::previousPos = pos;
}

void P1_Tile::draw(RenderWindow& window, const vector<vector<P1_Tile>>& map, FloatRect screenView)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (screenView.intersects(map[i][j].tile.getGlobalBounds())) window.draw(map[i][j].tile);
		}
	}
}

void P1_Tile::clear(vector<vector<P1_Tile>>& map, FloatRect& screenView, View& view, bool& foundFinish, vector<Vector2i>& toColor, bool& pause, bool& drawUI31)
{
	map.clear();
	screenView = FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT);
	view.reset(screenView);
	P1_Tile::finishTile = Vector2i(-1, -1);
	P1_Tile::startTile = Vector2i(-1, -1);
	foundFinish = false;
	pause = false;
	drawUI31 = false;
	toColor.clear();
}

void P1_Tile::reset(vector<vector<P1_Tile>>& map, bool& foundFinish, vector<Vector2i>& toColor)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j].tile.getFillColor() == P1_Tile::startColor || map[i][j].tile.getFillColor() == P1_Tile::finishColor)
			{
				map[i][j].tile.setFillColor(pathColor);
				map[i][j].alreadyChecked = false;
				map[i][j].fromStart = 0;
			}
		}
	}
	map[startTile.y][startTile.x].tile.setFillColor(P1_Tile::startColor);
	map[startTile.y][startTile.x].fromStart = 1;
	map[finishTile.y][finishTile.x].tile.setFillColor(P1_Tile::finishColor);
	foundFinish = false;
	toColor.clear();
}

void P1_Tile::screenBorders(const vector<vector<P1_Tile>>& map, View& view)
{
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		view.move(Vector2f(0.f, -view.getSize().y / (view.getSize().y / (1080.f / 216.f))));
		if (view.getCenter().y - WINDOW_HEIGHT / 2 < 0.f)
		{
			view.setCenter(Vector2f(view.getCenter().x, WINDOW_HEIGHT / 2));
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::S))
	{
		view.move(Vector2f(0.f, view.getSize().y / (view.getSize().y / (1080.f / 216.f))));
		if (view.getCenter().y + WINDOW_HEIGHT / 2 > map[map.size() - 1][0].tile.getPosition().y + map[0][0].tile.getGlobalBounds().height - 2.f)
		{
			if (WINDOW_HEIGHT / height <= 20) view.setCenter(Vector2f(view.getCenter().x, map[map.size() - 1][0].tile.getPosition().y + map[0][0].tile.getGlobalBounds().height - 2.f - WINDOW_HEIGHT / 2));
			else view.setCenter(Vector2f(view.getCenter().x, WINDOW_HEIGHT / 2));
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::A))
	{
		view.move(Vector2f(-view.getSize().x / (view.getSize().x / (1920.f / 384.f)), 0.f));
		if (view.getCenter().x - WINDOW_WIDTH / 2 < 0.f)
		{
			view.setCenter(Vector2f(WINDOW_WIDTH / 2, view.getCenter().y));
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		view.move(Vector2f(view.getSize().x / (view.getSize().x / (1920.f / 384.f)), 0.f));
		if (view.getCenter().x + WINDOW_WIDTH / 2 > map[0][map[0].size() - 1].tile.getPosition().x + map[0][0].tile.getGlobalBounds().width - 2.f)
		{
			if (WINDOW_WIDTH / width <= 20) view.setCenter(Vector2f(map[0][map[0].size() - 1].tile.getPosition().x + map[0][0].tile.getGlobalBounds().width - 2.f - WINDOW_WIDTH / 2, view.getCenter().y));
			else view.setCenter(Vector2f(WINDOW_WIDTH / 2, view.getCenter().y));
		}
	}
}

void P1_Tile::constructing(RenderWindow& window, vector<vector<P1_Tile>>& map, Color color)
{
	Vector2f MousePos = window.mapPixelToCoords(Mouse::getPosition(window));
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j].tile.getGlobalBounds().contains(MousePos))
			{
				Color tileColor = map[i][j].tile.getFillColor();
				if (previousPos.x != j || previousPos.y != i)
				{
					previousPos.x = j, previousPos.y = i;
					if (color == obstacleColor) // Color == obstacles color.
					{
						if (tileColor != finishColor && tileColor != startColor)
						{
							if (tileColor == obstacleColor) map[i][j].tile.setFillColor(pathColor), map[i][j].isObstacle = false;
							else map[i][j].tile.setFillColor(color), map[i][j].isObstacle = true;
						}
						return;
					}
					else if (color == finishColor) // Color == Finish color.
					{
						if (tileColor == finishColor)
						{
							map[i][j].tile.setFillColor(pathColor);
							finishTile = Vector2i(-1, -1);
							map[i][j].isFinish = false;
						}
						else if (tileColor != startColor && finishTile.x == -1)
						{
							map[i][j].tile.setFillColor(color);
							finishTile = Vector2i(j, i);
							map[i][j].isFinish = true;
						}
						return;
					}
					else if (tileColor != finishColor) // Color == Start color.
					{
						if (tileColor == startColor)
						{
							map[i][j].tile.setFillColor(pathColor);
							startTile = Vector2i(-1, -1);
							map[i][j].fromStart = 0;
							map[i][j].isStart = false;
						}
						else if (startTile.x == -1)
						{
							map[i][j].tile.setFillColor(color);
							startTile = Vector2i(j, i);
							map[i][j].fromStart = 1;
							map[i][j].isStart = true;
						}
						return;
					}
				}
			}
		}
	}
}

bool P1_Tile::programCanStart()
{
	if (P1_Tile::finishTile.x != -1 && P1_Tile::startTile.x != -1) return true;
	return false;
}

void P1_Tile::findingAlgorithm(vector<vector<P1_Tile>>& map, vector<Vector2i>& toColor, bool& foundFinish, bool& drawUI31)
{
	int x, y;
	size_t searchSize = toColor.size();
	for (int i = 0; i < searchSize; i++)
	{
		x = toColor[i].x;
		y = toColor[i].y;
		if (!map[y][x].alreadyChecked)
		{
			for (int k = x - 1; k <= x + 1; k += 2) // Checking left and right.
			{
				if (k >= 0 && k < P1_Tile::width)
				{
					if (y >= 0 && y < P1_Tile::height)
					{
						if (!map[y][k].isFinish && !map[y][k].isObstacle)
						{
							map[y][k].tile.setFillColor(startColor);
							toColor.push_back(Vector2i(k, y));
						}
						else if (map[y][k].isFinish) foundFinish = true;
					}
				}
			}
			for (int k = y - 1; k <= y + 1; k += 2) // Checking up and down.
			{
				if (x >= 0 && x < P1_Tile::width)
				{
					if (k >= 0 && k < P1_Tile::height)
					{
						if (!map[k][x].isFinish && !map[k][x].isObstacle)
						{
							map[k][x].tile.setFillColor(startColor);
							toColor.push_back(Vector2i(x, k));
						}
						else if (map[k][x].isFinish) foundFinish = true;
					}
				}
			}
		}
		map[y][x].alreadyChecked = true;
	}
	calculatingTiles(map);
	if (searchSize == 0) foundFinish = true, drawUI31 = true; // If there is no path from start to finish.
	else if (!foundFinish) toColor.erase(toColor.begin(), toColor.begin() + searchSize); // If still searching for finish.
	else foundWay(map), toColor.clear(); // If found the way.
}

void P1_Tile::colorsSetter(Color obstacleColor, Color startColor, Color finishColor, Color pathColor)
{
	P1_Tile::obstacleColor = obstacleColor;
	P1_Tile::startColor = startColor;
	P1_Tile::finishColor = finishColor;
	P1_Tile::pathColor = pathColor;
}

void P1_Tile::calculatingTiles(vector<vector<P1_Tile>>& map)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j].tile.getFillColor() == P1_Tile::startColor) map[i][j].fromStart++;
		}
	}
}

void P1_Tile::foundWay(vector<vector<P1_Tile>>& map)
{
	Vector2i coords = P1_Tile::finishTile;
	int x, y, number = 2;
	bool foundIt = false;
	while (coords != P1_Tile::startTile)
	{
		x = coords.x;
		y = coords.y;
		for (int k = x - 1; k <= x + 1; k += 2) // Searching left and right.
		{
			if (k >= 0 && k < width)
			{
				if (y >= 0 && y < height)
				{
					if (map[y][k].fromStart == number)
					{
						number++;
						foundIt = true;
						coords = Vector2i(k, y);
						map[y][k].tile.setFillColor(finishColor);
						break;
					}
				}
			}
		}
		if (!foundIt)
		{
			for (int k = y - 1; k <= y + 1; k += 2) // Searching down and up.
			{
				if (x >= 0 && x < width)
				{
					if (k >= 0 && k < height)
					{
						if (map[k][x].fromStart == number)
						{
							number++;
							coords = Vector2i(x, k);
							map[k][x].tile.setFillColor(finishColor);
							break;
						}
					}
				}
			}
		}
		foundIt = false;
	}
}