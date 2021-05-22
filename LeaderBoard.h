#pragma once
#include "Program.h"
#include <fstream>
#include "UI.h"
#include "VoidFunctions.h"

class Leaderboard
{
// ----------------------- DATA STRUCTURES -------------------------
private:
	string FileName;
	int LeaderboardSize;
	bool HighlightedInserting;
	int LeaderboardSections;
	vector<int> WhichWasHighlighted;
	vector<vector<pair<Text, Text>>> PlayersStats;
	vector<vector<int>> PlayersScores;
	RectangleShape Board;
	pair<Text, RectangleShape> Button;
	bool MultiplySections;
	pair<Text, RectangleShape> ChangerButton;
	vector<string> SectionChangerNames;
	pair<CircleShape, CircleShape> Arrows;
	bool ReleasedArrow;
	struct StatsCustom
	{
		Color NameColor, ScoreColor, HighlightedColor, NameOutlineColor, ScoreOutlineColor;
		float NameThickness, ScoreThickness;
		int TextSize;
		Vector2f NamePos, ScorePos;
	};
	StatsCustom lb;

// --------------------------- FUNCTIONS -------------------------
public:
	Leaderboard(Font* font, const string& ButtonText, Vector2f ButtonTextPos, Vector2f ButtonBoxPos, Vector2f BoxSize, Color BoxColor, Vector2f BoardPos, Vector2f BoardSize, Color BoardColor, const string& FileName, int LeaderboardSize, bool HighlightedInserting, Vector2f NamePos, Color NameColor, Color NameOutlineColor, int TextSize, float NameThickness, Vector2f ScorePos, Color ScoreColor, Color ScoreOutlineColor, float ScoreThickness, Color HighlightedColor, int LeaderboardSections, bool MultiplySections, Vector2f SectionChangerPos, const string& SectionNames);
	~Leaderboard();
	void InsertPlayer(const string& name, const string& score, Font* font, bool Max, int Which);
	void DrawLeaderboard(RenderWindow& window, const Event& e, Color BackgroundColor, int& Which);
	bool ClickedButton(Vector2f MousePos);
private:
	int FindWhere(int score, bool Max, int Which); // <------------------------------ Where to put new score.
	void PushingDownwards(int toWhere, int Which); // <----------------- Move all player stuff down from coming new player stuff.
	void CreateButton(Font* font, const string& string, Vector2f ButtonTextPos, Vector2f ButtonBoxPos, Vector2f BoxSize, Color BoxColor);
	void CreateBoard(Vector2f BoardPos, Vector2f BoardSize, Color BoardColor);
	void CreateChangerOfSections(Font* font, Vector2f ChangerButtonPos, const string& SectionNames);
	void ReadFromFile(Font* font);
	int TimeConverter(const string& time, int State); // <------------------ Time is converted to seconds.
	void ChangeBackColor(Color NameColor, Color ScoreColor, int Which); // <------------------------ Change back highlighted color to normal.
	string ScoreToTime(int score);
	bool IsScore(const string& score);
	void SectionChangerConfig(int Which);
	int WhatState(const string& score);
	void ChangeMode(Vector2f MousePos, int& Which);
};