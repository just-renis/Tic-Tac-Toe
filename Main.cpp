#include "Starting.h"
#include "Main1.h"
#include "Main2.h"
#include "Main3.h"
#include "Main4.h"
#include "MainP1.h"
#include "MainP2.h"
#include "MainP3.h"

void drawLoading(float percentage, RenderWindow& window, Text& loadingPercentage, const RectangleShape& loadingBarFrame, RectangleShape& loadingBar);

int main()
{
	RenderWindow window{ VideoMode((int)WINDOW_WIDTH, (int)WINDOW_HEIGHT), "SIMPLE GAMES", Style::Fullscreen };
	View view{ Vector2f{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, Vector2f{WINDOW_WIDTH, WINDOW_HEIGHT} };
	window.setView(view);
	int mainState{ 0 }, depthState{ 0 };
	window.setKeyRepeatEnabled(false);
	Event e;
	Font* font, font1;
	font1.loadFromFile("Fonts/l-regular.ttf");
	font = &font1;

	// --------------------------- LOADING STUFF --------------------------------
	Text loadingPercentage{ TextSetter(font, FOw(50), FOw(5.f), Color::Black, Color::White, "", V2f(900.f, 503.f)) };
	RectangleShape loadingBarFrame{ RectangleSetter(V2f(1000.f, 50.f), V2f(460.f, 515.f), Color::Transparent, Color::White, FOw(3.f)) };
	RectangleShape loadingBar{ RectangleSetter(Vector2f(0.f, WINDOW_HEIGHT / 21.6f), V2f(460.f, 515.f), Color::Red, Color::Transparent, 0.f) };
	float percentage{ 0.f };

	// --------------------------- FILES NAMES AND TEXTURES STUFF ------------------------------------
	vector<pair<string, string>> filesTexs, filesBuffers;
	TexturesMap Texs;
	SoundsMap Buffers;
	filesTexs.push_back(make_pair("game1", "Textures/FirstGame.png"));
	filesTexs.push_back(make_pair("game2", "Textures/SecondGame.png"));
	filesTexs.push_back(make_pair("game3", "Textures/ThirdGame.png"));
	filesTexs.push_back(make_pair("game4", "Textures/FourthGame.png"));
	filesTexs.push_back(make_pair("program1", "Textures/FirstProgram.png"));
	filesTexs.push_back(make_pair("ZeroTex", "Textures/0.png"));
	filesTexs.push_back(make_pair("CrossTex", "Textures/Cross.png"));
	filesTexs.push_back(make_pair("MineSweeperTex", "Textures/MineSweeper.png"));
	filesTexs.push_back(make_pair("Check", "Textures/Check.png"));
	filesTexs.push_back(make_pair("MineSweeperEmoji", "Textures/MineSweeperEmoji.png"));
	filesTexs.push_back(make_pair("PlusSign", "Textures/Plus.png"));
	filesTexs.push_back(make_pair("MinusSign", "Textures/Minus.png"));
	filesTexs.push_back(make_pair("Player4", "Textures/Player4.png"));
	filesTexs.push_back(make_pair("Wall1", "Textures/Wall1.png"));
	filesTexs.push_back(make_pair("WallBackground", "Textures/WallBackground.png"));
	filesTexs.push_back(make_pair("coin", "Textures/coin.png"));
	filesTexs.push_back(make_pair("HeartUI", "Textures/HeartUI.png"));
	filesTexs.push_back(make_pair("HeartCollectible", "Textures/HeartCollectible.png"));
	filesTexs.push_back(make_pair("PotionUI", "Textures/PotionUI.png"));
	filesTexs.push_back(make_pair("SpikesTrap", "Textures/SpikesTrap.png"));
	filesTexs.push_back(make_pair("SpearTrap", "Textures/SpearTrap.png"));
	filesTexs.push_back(make_pair("BoulderSpawner", "Textures/BoulderSpawner.png"));
	filesTexs.push_back(make_pair("BoulderTrap", "Textures/BoulderTrap.png"));

	// ------------------------ SOUNDS -----------------------------------------
	filesBuffers.push_back(make_pair("Walking", "Audio/Walking.wav"));
	filesBuffers.push_back(make_pair("CollectCoin", "Audio/CollectCoin.wav"));
	filesBuffers.push_back(make_pair("CollectHeart", "Audio/CollectHeart.wav"));
	filesBuffers.push_back(make_pair("SpikeTrap", "Audio/SpikeTrap.wav"));
	filesBuffers.push_back(make_pair("PlayerHurt", "Audio/PlayerHurt.wav"));
	filesBuffers.push_back(make_pair("Death", "Audio/Death.wav"));
	filesBuffers.push_back(make_pair("DrinkPotion", "Audio/DrinkPotion.wav"));
	filesBuffers.push_back(make_pair("Reborn", "Audio/Reborn.wav"));
	filesBuffers.push_back(make_pair("SpearTrap", "Audio/SpearTrap.wav"));
	filesBuffers.push_back(make_pair("BoulderTrap", "Audio/BoulderTrap.wav"));
	filesBuffers.push_back(make_pair("BoulderCrash", "Audio/BoulderCrash.wav"));

	// -------------------------------- TEXTURE LOADING SCREEN ----------------------------
	while (window.isOpen() && percentage < 100)
	{
		Texture texture;
		SoundBuffer buffer;
		int size{ (int)filesTexs.size() };
		while (window.pollEvent(e))
		{
			if (Event::Closed) window.close();
			else if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
		}
		for (int i = 0; i < size; i++)
		{
			texture.loadFromFile(filesTexs[i].second);
			percentage = ((float)(i + 1) / float(size + filesBuffers.size())) * 100;
			drawLoading(percentage, window, loadingPercentage, loadingBarFrame, loadingBar);
			Texs[filesTexs[i].first] = texture;
		}
		for (int i = size; i < size + filesBuffers.size(); i++)
		{
			buffer.loadFromFile(filesBuffers[i - size].second);
			percentage = ((float)(i + 1) / float(size + filesBuffers.size())) * 100;
			drawLoading(percentage, window, loadingPercentage, loadingBarFrame, loadingBar);
			Buffers[filesBuffers[i - size].first] = buffer;
		}
	}

	// ------------------------------ GOING INTO STARTING SCREEN --------------------------------------
	while (window.isOpen())
	{
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) window.close();
		}
		switch (mainState)
		{
			case 0: StartingLoop(window, e, mainState, depthState, Texs, font); break;
			case 1: Main1Loop(window, e, mainState, depthState, Texs, font); break;
			case 2: Main2Loop(window, e, mainState, depthState, Texs, font); break;
			case 3: Main3Loop(window, e, mainState, depthState, font, Texs); break;
			case 4: Main4Loop(window, e, mainState, depthState, font, Texs, Buffers); break;
			case 5: MainP1Loop(window, e, mainState, depthState, Texs, Buffers, font); break;
			case 6: MainP2Loop(window, e, mainState, depthState, font); break;
			case 7: MainP3Loop(window, e, mainState, depthState, font);
		}
	}
	return 0;
}

void drawLoading(float percentage, RenderWindow& window, Text& loadingPercentage, const RectangleShape& loadingBarFrame, RectangleShape& loadingBar)
{
	loadingPercentage.setString(to_string((int)percentage) + "%");
	loadingBar.setSize(Vector2f(percentage * (WINDOW_WIDTH / 192.f), WINDOW_HEIGHT / 21.6f));
	window.clear(Color::Black);
	window.draw(loadingBarFrame);
	window.draw(loadingBar);
	window.draw(loadingPercentage);
	window.display();
}