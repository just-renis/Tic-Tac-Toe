#include "Level.h"

Level::Level()
	: level{1}
{
}

void Level::play(RenderWindow& window, G1_Player& player, bool& MainLoop, TexturesMap& Texs, SoundsMap& Audio, UI_ftt& UI)
{
	Clock movementClock;
	Clock deltaTimeClock;
	Clock intersectionOccurenceTimer;
	Sprite background;
	string collectedItem = "";
	FloatRect text;

	// -------------------------- BACKGROUND FOR LEADERBOARD -------------------------
	bool newBackgroundColorNeeded = true;
	Color backgroundColor = Color::Black, nextColor;
	Clock backgroundTimer;

	// ------------------------------------------------------------- SOUNDS CONFIG -------------------------------------------------------------------------------

	// -------------------------------------------------------------- LEVELS ---------------------------------------------------------------------------------------
	if (this->level == 1)
	{
		background.setTexture(Texs["WallBackground"]);
		background.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(V2f(1920.f, 1040.f))));
		for (int i = 0; i < 2; i++)
		{
			for (int j = 1; j <= 10; j++) this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(40.f * j, 40.f * i)));
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++) this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(40.f * j, 180.f + 40.f * i)));
		}
		this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(280.f, 160.f)));
		this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(380.f, 160.f)));
		this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(480.f, 160.f)));
		this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(580.f, 160.f)));
		this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(380.f, 240.f)));
		this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(130.f, 200.f)));
		this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(130.f, 160.f)));
		this->obstacles.push_back(new Obstacle(&Texs["Wall1"], V2f(220.f, 160.f)));
		this->collectibles.push_back(Collectible("coin", &Texs["coin"], V2f(300.f, 300.f)));
		this->collectibles.push_back(Collectible("coin", &Texs["coin"], V2f(300.f, 400.f)));
		this->collectibles.push_back(Collectible("coin", &Texs["coin"], V2f(400.f, 300.f)));
		this->collectibles.push_back(Collectible("coin", &Texs["coin"], V2f(400.f, 400.f)));
		this->collectibles.push_back(Collectible("heart", &Texs["HeartCollectible"], V2f(200.f, 400.f))); 
		this->collectibles.push_back(Collectible("heart", &Texs["HeartCollectible"], V2f(320.f, 300.f)));
	//	this->collectibles.push_back(Collectible("heart", &Texs["HeartCollectible"], V2f(240.f, 300.f)));
	//	this->collectibles.push_back(Collectible("heart", &Texs["HeartCollectible"], V2f(260.f, 300.f)));
	//	this->collectibles.push_back(Collectible("heart", &Texs["HeartCollectible"], V2f(280.f, 300.f)));
	//	this->collectibles.push_back(Collectible("heart", &Texs["HeartCollectible"], V2f(300.f, 300.f)));
		for (int i = 1; i < 101; i++) this->collectibles.push_back(Collectible("coin", &Texs["coin"], V2f(400.f + 2 * i, 400.f)));
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				traps.spikes.push_back(new SpikeTrap(&Texs["SpikesTrap"], V2f(860.f + 40.f * j, 75.f + 40.f * i), 2.f));
			}
		}
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				traps.spikes.push_back(new SpikeTrap(&Texs["SpikesTrap"], V2f(1260.f + 40.f * j, 475.f + 40.f * i), rand() % 6 + 1.f));
			}
		}
		traps.spikes.push_back(new SpikeTrap(&Texs["SpikesTrap"], V2f(170.f, 75.f), 2.f));
		traps.spears.push_back(new SpearTrap(&Texs["SpearTrap"], V2f(170.f, 55.f), 0.02f, 2.f));
		traps.spikes.push_back(new SpikeTrap(&Texs["SpikesTrap"], V2f(280.f, 90.f), 2.f));
		traps.spears.push_back(new SpearTrap(&Texs["SpearTrap"], V2f(250.f, 55.f), 0.02f, 2.f));
		this->spawners.push_back(Spawner(&Texs["BoulderSpawner"], V2f(240.f, 180.f), 1.f, 'd', 50.f));
		this->spawners.push_back(Spawner(&Texs["BoulderSpawner"], V2f(150.f, 180.f), 1.f, 'u', 50.f));
		this->spawners.push_back(Spawner(&Texs["BoulderSpawner"], V2f(400.f, 260.f), 1.f, 'l', 50.f));
		this->spawners.push_back(Spawner(&Texs["BoulderSpawner"], V2f(150.f, 220.f), 1.f, 'r', 50.f));
	}
	Event e;
	while (window.isOpen() && MainLoop)
	{
		Time dt = deltaTimeClock.restart();
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) window.close();
			else if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Escape)
				{
					MainLoop = false;
					break;
				}
				else if (e.key.code == Keyboard::E) player.usePotion(this->newSound);
			}
		}
		if (player.checkMovement()) // Player movement.
		{
			char dir{'n'}; // None direction.
			if (Keyboard::isKeyPressed(Keyboard::W)) dir = 'w';
			else if (Keyboard::isKeyPressed(Keyboard::A)) dir = 'a';
			else if (Keyboard::isKeyPressed(Keyboard::S)) dir = 's';
			else if (Keyboard::isKeyPressed(Keyboard::D)) dir = 'd';
			else player.idle();
			if (dir != 'n') player.movement(dir, this->obstacles); // If player been moved then move him and check intersections.
		}
		Collectible::intersects(player.getBody(), collectibles, collectedItem, this->newSound); // Collecting collectibles if can.
		player.collectedItem(collectedItem);
		if (intersectionOccurenceTimer.getElapsedTime().asSeconds() >= 0.04f) intersectsTrap(player, traps), intersectionOccurenceTimer.restart();

		// ------------------------------------------------------- SOUND STUFF ----------------------------------------------------------------------------------
		this->soundHandler(Audio);

		// -------------------------------------------------- DRAW ------------------------------------------
		backgroundColorChanger(backgroundColor, nextColor, backgroundTimer, newBackgroundColorNeeded);
		window.clear(backgroundColor);
		window.draw(background);
		UI_ftt::Draw(window, UI, player); // Draw UI.
		Obstacle::Draw(window, this->obstacles); // Draw obstacles.
		Spawner::Draw(spawners, window); // Draw spawners.
		spawnerLogic(Texs, player.getBody().getPosition()); // Spawners logic.
		drawTraps(window, traps); // Draw traps.
		trapsLogic(player, dt); // Traps logic.
		BoulderTrap::BouldersLogic(traps.boulders, this->obstacles); // Boulders logic.
		BoulderTrap::BouldersLogic(traps.boulders, this->traps.boulders); // Boulders logic.
		Collectible::Draw(window, this->collectibles); // Draw collectibles.
		player.drawPlayer(window);
		window.display();
	}
}

Level::~Level()
{
	for (SpikeTrap* trap : this->traps.spikes) delete trap;
	for (SpearTrap* trap : this->traps.spears) delete trap;
	for (BoulderTrap* trap : this->traps.boulders) delete trap;
	for (Obstacle* obstacle : this->obstacles) delete obstacle;
}

// ----------------------------------------------------------------- PRIVATE FUNCTIONS -------------------------------------------------------------------

void Level::drawTraps(RenderWindow& window, const Traps& trap)
{
	Trap::Draw(window, trap.spikes, this->newSound);
	Trap::Draw(window, trap.spears, this->newSound);
	Trap::Draw(window, trap.boulders, this->newSound);
}

void Level::intersectsTrap(G1_Player& player, const Traps& traps)
{
	player.getDamage(Trap::intersects(player.getBody(), traps.spikes), this->newSound);
	player.getDamage(Trap::intersects(player.getBody(), traps.spears), this->newSound);
	player.getDamage(Trap::intersects(player.getBody(), traps.boulders), this->newSound);
}

void Level::trapsLogic(const G1_Player& player, Time dt)
{
	for (SpikeTrap* trap : traps.spikes) trap->Logic(player.getBody().getPosition(), this->newSound);
	for (SpearTrap* trap : traps.spears) trap->Logic(player.getBody().getPosition(), this->newSound);
	for (BoulderTrap* trap : traps.boulders)
	{
		if (!trap->isCrashed())
		{
			int which = trap->whichSpawnerFrom(); // From which spawner this trap has been spawned.
			if (trap->isInSpawner())
			{
				if (this->spawners[which].isReady())
				{
					trap->gettingOutOfSpawner(this->spawners[which].getSpawnerFrame());
					trap->Logic(player.getBody().getPosition(), this->newSound, dt, true);
				}
			}
			else trap->Logic(player.getBody().getPosition(), this->newSound, dt, false);
		}
		else if (!trap->isCrashedSound()) this->newSound += "BoulderCrash ", trap->setCrashedSoundFalse();
	}
}

void Level::soundHandler(SoundsMap& Audio)
{
	while (this->newSound.size() > 0)
	{
		size_t find = this->newSound.find(" ");
		string sound = string(this->newSound.begin(), this->newSound.begin() + find);
		this->newSound.erase(this->newSound.begin(), this->newSound.begin() + find + 1);
		this->sounds.emplace_back(SoundPlayer(&Audio[sound], sound));
		SoundPlayer::playSound(this->sounds);
	}
	SoundPlayer::deleteIfStoppedSound(this->sounds);
}

void Level::spawnerLogic(TexturesMap& Texs, Vector2f playerPos)
{
	for (int i = 0; i < this->spawners.size(); i++)
	{
		if (this->spawners[i].getType() == "BoulderTrap")
		{
			BoulderTrap* trap{ nullptr };
			ifCanSpawnTrap(trap, playerPos, i, Texs);
		}
	}
}