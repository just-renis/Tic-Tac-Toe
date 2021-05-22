#include "LeaderBoard.h"

// ---------------------------------------- CONSTRUCTORS/DESTRUCTORS -----------------------------------------------
Leaderboard::Leaderboard(Font* font, const string& ButtonText, Vector2f ButtonTextPos, Vector2f ButtonBoxPos, Vector2f BoxSize, Color BoxColor, Vector2f BoardPos, Vector2f BoardSize, Color BoardColor, const string& FileName, int LeaderboardSize, bool HighlightedInserting, Vector2f NamePos, Color NameColor, Color NameOutlineColor, int TextSize, float NameThickness, Vector2f ScorePos, Color ScoreColor, Color ScoreOutlineColor, float ScoreThickness, Color HighlightedColor, int LeaderboardSections, bool MultiplySections, Vector2f SectionChangerPos, const string& SectionNames)
{
	this->FileName = FileName;
	this->LeaderboardSize = LeaderboardSize;
	this->HighlightedInserting = HighlightedInserting;
	this->LeaderboardSections = LeaderboardSections;
	this->MultiplySections = MultiplySections;
	this->ReleasedArrow = true;
	for (int i = 0; i < LeaderboardSections; i++)
	{
		PlayersScores.push_back(vector<int>());
		PlayersStats.push_back(vector<pair<Text, Text>>());
		WhichWasHighlighted.push_back(-1);
	}
	lb.HighlightedColor = HighlightedColor;
	lb.NameOutlineColor = NameOutlineColor;
	lb.NameColor = NameColor;
	lb.NamePos = NamePos;
	lb.NameThickness = NameThickness;
	lb.ScoreColor = ScoreColor;
	lb.ScorePos = ScorePos;
	lb.ScoreThickness = ScoreThickness;
	lb.ScoreOutlineColor = ScoreOutlineColor;
	lb.TextSize = TextSize;
	if (MultiplySections) CreateChangerOfSections(font, SectionChangerPos, SectionNames);
	CreateButton(font, ButtonText, ButtonTextPos, ButtonBoxPos, BoxSize, BoxColor);
	CreateBoard(BoardPos, BoardSize, BoardColor);
	ReadFromFile(font);
}

// -------------------------------------------------------------- PUBLIC FUNCTIONS --------------------------------------------------------------------------
void Leaderboard::InsertPlayer(const string& name, const string& score, Font* font, bool Max, int Which)
{
	int RealScore;
	string time = score;
	if (IsScore(time)) RealScore = stoi(time);
	else
	{
		RealScore = TimeConverter(time, 3);
		time = ScoreToTime(RealScore);
	}
	int where = FindWhere(RealScore, Max, Which);
	if (where != -1)
	{
		PlayersScores[Which].insert(PlayersScores[Which].begin() + where, RealScore); // <----------- Inserting new player score.
		if (HighlightedInserting)
		{
			ChangeBackColor(lb.NameColor, lb.ScoreColor, Which);
			PlayersStats[Which].insert(PlayersStats[Which].begin() + where, make_pair(TextSetter(font, lb.TextSize, lb.NameThickness, lb.HighlightedColor, lb.NameOutlineColor, to_string(where + 1) + ". " + name, Vector2f(lb.NamePos.x, lb.NamePos.y + where * (lb.TextSize + (WINDOW_HEIGHT / 216.f)))), TextSetter(font, lb.TextSize, lb.ScoreThickness, lb.HighlightedColor, lb.ScoreOutlineColor, time, Vector2f(lb.ScorePos.x, lb.ScorePos.y + where * (lb.TextSize + (WINDOW_HEIGHT / 216.f)))))); // <------------ Inserting new player stats.
			WhichWasHighlighted[Which] = where;
		}
		else PlayersStats[Which].insert(PlayersStats[Which].begin() + where, make_pair(TextSetter(font, lb.TextSize, lb.NameThickness, lb.NameColor, lb.NameOutlineColor, to_string(where + 1) + ". " + name, Vector2f(lb.NamePos.x, lb.NamePos.y + where * (lb.TextSize + (WINDOW_HEIGHT / 216.f)))), TextSetter(font, lb.TextSize, lb.ScoreThickness, lb.ScoreColor, lb.ScoreOutlineColor, time, Vector2f(lb.ScorePos.x, lb.ScorePos.y + where * (lb.TextSize + (WINDOW_HEIGHT / 216.f)))))); // <------------ Inserting new player stats.
		PushingDownwards(where, Which); // <------------------------------ Pushing all scores down.
	}
}

Leaderboard::~Leaderboard()
{
	ofstream fr(FileName);
	string name, score;
	for (int i = 0; i < PlayersStats.size(); i++)
	{
		for (int j = 0; j < PlayersStats[i].size(); j++)
		{
			name = PlayersStats[i][j].first.getString();
			score = PlayersStats[i][j].second.getString();
			fr << name << "~" << score << "~" << endl;
		}
		fr << "~" << endl;
	}
	fr.close();
}

void Leaderboard::DrawLeaderboard(RenderWindow& window, const Event& e, Color BackgroundColor, int& Which)
{
	Vector2f MousePos = (Vector2f)Mouse::getPosition(window);
	window.clear(BackgroundColor);
	window.draw(Board);
	window.draw(Button.second);
	window.draw(Button.first);
	if (MultiplySections)
	{
		SectionChangerConfig(Which);
		window.draw(ChangerButton.second);
		window.draw(ChangerButton.first);
		window.draw(Arrows.first);
		window.draw(Arrows.second);
		ChangeMode(MousePos, Which);
		if (e.type == Event::MouseButtonReleased) ReleasedArrow = true;
	}
	for (int i = 0; i < PlayersScores[Which].size(); i++)
	{
		window.draw(PlayersStats[Which][i].first);
		window.draw(PlayersStats[Which][i].second);
	}
}

bool Leaderboard::ClickedButton(Vector2f MousePos)
{
	if (Button.second.getGlobalBounds().contains(MousePos)) return true;
	return false;
}

// -------------------------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------------------------
int Leaderboard::FindWhere(int score, bool Max, int Which)
{
	if (Max)
	{
		for (int i = 0; i < PlayersScores[Which].size(); i++) if (PlayersScores[Which][i] < score) return i;
	}
	else
	{
		for (int i = 0; i < PlayersScores[Which].size(); i++) if (PlayersScores[Which][i] > score) return i;
	}
	if (PlayersScores[Which].size() < LeaderboardSize) return (int)PlayersScores[Which].size(); // <---------------- If not found return at the last position.
	else return -1; // <------- If not found return nowhere.
}

void Leaderboard::PushingDownwards(int toWhere, int Which)
{
	string Replacer;
	for (int i = (int)PlayersScores[Which].size() - 1; i > toWhere; i--)
	{
		PlayersStats[Which][i].first.move(0.f, lb.TextSize + (WINDOW_HEIGHT / 216.f));
		PlayersStats[Which][i].second.move(0.f, lb.TextSize + (WINDOW_HEIGHT / 216.f));
		Replacer = PlayersStats[Which][i].first.getString();
		if (i > 9) Replacer.replace(0, 2, to_string(i + 1));
		else if (i == 9) Replacer.replace(0, 1, "10");
		else Replacer.replace(0, 1, to_string(i + 1));
		PlayersStats[Which][i].first.setString(Replacer);
	}
	if (PlayersScores[Which].size() > LeaderboardSize) PlayersScores[Which].pop_back(), PlayersStats[Which].pop_back();
}

void Leaderboard::CreateButton(Font* font, const string& ButtonText, Vector2f ButtonTextPos, Vector2f ButtonBoxPos, Vector2f BoxSize, Color BoxColor)
{
	this->Button.first = TextSetter(font, FOw(30), FOw(3.f), Color::Black, Color::White, ButtonText, ButtonTextPos);
	this->Button.second = RectangleSetter(BoxSize, ButtonBoxPos, BoxColor, Color::White, FOw(4.f));
}

void Leaderboard::CreateBoard(Vector2f BoardPos, Vector2f BoardSize, Color BoardColor)
{
	this->Board = RectangleSetter(BoardSize, BoardPos, BoardColor, Color::White, FOw(10.f));
}

void Leaderboard::CreateChangerOfSections(Font* font, Vector2f ChangerButtonPos, const string& SectionNames)
{
	this->ChangerButton.first = TextSetter(font, FOw(25), 0.f, Color::Black, Color::Transparent, "", Vector2f(ChangerButtonPos.x, ChangerButtonPos.y - (WINDOW_HEIGHT / 216.f)));
	this->ChangerButton.second = RectangleSetter(V2f(200.f, 25.f), ChangerButtonPos, Color::White, Color::Black, FOw(3.f));	
	this->Arrows.first = ShapeSetter(WINDOW_WIDTH / 128.f, V2f(901.f, 364.f), Color::Black, Color::Transparent, 0.f, 3);
	this->Arrows.second = ShapeSetter(WINDOW_WIDTH / 128.f, V2f(1074.f, 364.f), Color::Black, Color::Transparent, 0.f, 3);
	this->Arrows.first.setOrigin(Arrows.first.getRadius(), Arrows.first.getRadius());
	this->Arrows.second.setOrigin(Arrows.second.getRadius(), Arrows.second.getRadius());
	this->Arrows.first.rotate(270.f);
	this->Arrows.second.rotate(90.f);
	string SectionName;
	for (int i = 0; i < SectionNames.size(); i++)
	{
		if (SectionNames[i] != '~') SectionName += SectionNames[i];
		else
		{
			this->SectionChangerNames.push_back(SectionName);
			SectionName.clear();
		}
	}
	this->SectionChangerNames.push_back(SectionName);
}

void Leaderboard::ReadFromFile(Font* font)
{
	int i = 0, Which = 0;
	ifstream fd(FileName);
	string name, score;
	while (!fd.eof())
	{
		getline(fd, name, '~');
		while (name == "")
		{
			fd.ignore(1, '\n');
			getline(fd, name, '~');
			Which++;
			i = 0;
			if (Which >= LeaderboardSections) break;
		}
		getline(fd, score, '~');
		fd.ignore(1, '\n');
		if (name != "")
		{
			PlayersStats[Which].push_back(make_pair(TextSetter(font, lb.TextSize, lb.NameThickness, lb.NameColor, lb.ScoreColor, name, Vector2f(lb.NamePos.x, lb.NamePos.y + i * (lb.TextSize + (WINDOW_HEIGHT / 216.f)))), TextSetter(font, lb.TextSize, lb.ScoreThickness, lb.ScoreColor, lb.ScoreOutlineColor, score, Vector2f(lb.ScorePos.x, lb.ScorePos.y + i * (lb.TextSize + (WINDOW_HEIGHT / 216.f))))));
			PlayersScores[Which].push_back(TimeConverter(score, WhatState(score)));
			i++;
		}
	}
	fd.close();
}

int Leaderboard::TimeConverter(const string& time, int State)
{
	int seconds = 0;
	bool NextOne = false, Breaked = false;
	string Converter;
	for (int i = 0; i < time.size(); i++)
	{
		if (time[i] >= '0' && time[i] <= '9') Converter += time[i], NextOne = true;
		else if (State == 3 && NextOne) seconds += stoi(Converter) * 3600, Converter.clear(), State--, NextOne = false;
		else if (State == 2 && NextOne) seconds += stoi(Converter) * 60, Converter.clear(), State--, NextOne = false;
		else if (State == 1 && NextOne)
		{
			seconds += stoi(Converter);
			Breaked = true;
			break;
		}
	}
	if (!Breaked) seconds += stoi(Converter);
	return seconds;
}

void Leaderboard::ChangeBackColor(Color NameColor, Color ScoreColor, int Which)
{
	if (WhichWasHighlighted[Which] != -1)
	{
		PlayersStats[Which][WhichWasHighlighted[Which]].first.setFillColor(NameColor);
		PlayersStats[Which][WhichWasHighlighted[Which]].second.setFillColor(ScoreColor);
	}
}

string Leaderboard::ScoreToTime(int score)
{
	string Time;
	if (score < 60) Time = to_string(score) + " secs";
	else if (score < 3600) Time = to_string(score / 60) + " mins " + to_string(score % 60) + " secs";
	else Time = to_string(score / 3600) + " hours " + to_string(score % 3600 / 60) + " mins " + to_string(score % 60) + " secs";
	return Time;
}

bool Leaderboard::IsScore(const string& score)
{
	string Letters = "ABCDEFGHIJKLMNOPRSTUVZYQXWabcdefghijklmnoprstuvzyqx";
	for (int i = 0; i < score.size(); i++)
	{
		for (int j = 0; j < Letters.size(); j++)
		{
			if (score[i] == Letters[j]) return false;
		}
	}
	return true;
}

void Leaderboard::SectionChangerConfig(int Which)
{
	this->ChangerButton.first.setString(SectionChangerNames[Which]);
	this->ChangerButton.first.setPosition(Vector2f(this->ChangerButton.second.getPosition().x + this->ChangerButton.first.getGlobalBounds().width / 2, this->ChangerButton.first.getPosition().y));
}

int Leaderboard::WhatState(const string& score)
{
	for (int j = 0; j < score.size(); j++)
	{
		if (score[j] == 'h') return 3;
		else if (score[j] == 'm') return 2;
	}
	return 1;
}

void Leaderboard::ChangeMode(Vector2f MousePos, int& Which)
{
	if (ReleasedArrow && Mouse::isButtonPressed(Mouse::Left))
	{
		if (this->Arrows.first.getGlobalBounds().contains(MousePos))
		{
			Which--;
			if (Which < 0) Which = LeaderboardSections - 1;
		}
		else if (this->Arrows.second.getGlobalBounds().contains(MousePos))
		{
			Which++;
			if (Which == LeaderboardSections) Which = 0;
		}
		this->ChangerButton.first.setString(SectionChangerNames[Which]);
		ReleasedArrow = false;
	}
}