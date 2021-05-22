#pragma once
#include "SFML/Network.hpp"
#include "Power.h"

void Server();
void Client(Font* font);
void PlayerMovement(Event e, Player& player, bool Backwards);
Vector2f CoinNewPosition();
Vector2f PowerUpNewPosition(vector<Player> Players, vector<Power> PowerUps);
vector<string> FindingHighestScores(vector<Player> Players);
void DrawHighestScores(RenderWindow& window, vector<Player>& Players, vector<string> TopNames, int Me);

void MainMulti1Loop(int& MainState, int& DepthState, unordered_map<string, Texture> texs, Font* font)
{
	char consoleInput;
	cout << "Write 's' to host server or write 'j' to join the server" << endl;
	cin >> consoleInput;
	if (consoleInput == 's') Server();
	else if (consoleInput == 'j') Client(font);
}

void Server()
{
	TcpListener listener;
	SocketSelector selector;
	bool ServerIsRunning = true, CanStart = false, something = false, ServerStarted = false;
	vector<TcpSocket*> Clients;
	listener.listen(53000);
	selector.add(listener);
	int consoleInput, count = 0, Limit;
	float SpawnPowerUp = 1.f;
	Clock TimePassed;
	cout << "How many players will be?" << endl;
	cin >> consoleInput;
	cout << "Score till win?" << endl;
	cin >> Limit;
	while (ServerIsRunning)
	{
		if (ServerStarted)
		{
			if (TimePassed.getElapsedTime().asSeconds() >= SpawnPowerUp) // Can spawn power up, asking locations.
			{
				Packet packet;
				packet << "GiveLoc";
				Clients[0]->send(packet);
				packet.clear();
				TimePassed.restart();
				SpawnPowerUp += 1.f;
			}
		}
		if (selector.wait())
		{
			if (!CanStart && selector.isReady(listener)) // Clients join in.
			{
				TcpSocket* socket = new TcpSocket;
				listener.accept(*socket);
				Packet packet;
				string id;
				if (socket->receive(packet) == Socket::Done) packet >> id;
				packet.clear();
				cout << id << " has joined the server" << endl;
				Clients.push_back(socket);
				selector.add(*socket);
				if (consoleInput == Clients.size()) CanStart = true;
				packet << CanStart;
				for (int i = 0; i < Clients.size(); i++) Clients[i]->send(packet);
				packet.clear();
				if (CanStart)
				{
					something = true;
					for (int i = 0; i < Clients.size(); i++) // Player size packet sending.
					{
						packet << i << consoleInput << Limit;
						Clients[i]->send(packet);
						packet.clear();
					}
				}
			}
			else if (something)
			{
				for (int i = 0; i < Clients.size(); i++) // Receiving color packets and ID.
				{
					if (selector.isReady(*Clients[i]))
					{
						Packet packet;
						count++;
						if (Clients[i]->receive(packet) == Socket::Done)
						{
							for (int j = 0; j < Clients.size(); j++) // Sending color packets and ID.
							{
								Clients[j]->send(packet);
							}
						}
					}
				}
				if (count == Clients.size())
				{
					Packet packet;
					Vector2f CoinPos;
					CoinPos = Vector2f(float(rand() % int(WINDOW_WIDTH - FO(15.f))), float(rand() % int(WINDOW_HEIGHT - FO(15.f))));
					packet << CoinPos.x << CoinPos.y;
					for (int i = 0; i < Clients.size(); i++) Clients[i]->send(packet);
					something = false;
					ServerStarted = true;
					TimePassed.restart();
				}
			}
			else // 1 client sends new data to other clients.
			{
				for (int i = 0; i < Clients.size(); i++)
				{
					if (selector.isReady(*Clients[i]))
					{
						string Content;
						Packet packet, sendPacket;
						if (Clients[i]->receive(packet) == Socket::Done)
						{
							packet >> Content;
							if (Content == "PlayerPos")
							{
								Vector2f pos;
								packet >> pos.x >> pos.y;
								sendPacket << Content << i << pos.x << pos.y;
							}
							else if (Content == "CoinPos")
							{
								Vector2f pos;
								int Multiplier;
								packet >> pos.x >> pos.y >> Multiplier;
								sendPacket << Content << i << pos.x << pos.y << Multiplier;
							}
							else if (Content == "GiveLoc") // Receive power up location and sending out.
							{
								Vector2f pos;
								string type;
								packet >> pos.x >> pos.y >> type;
								sendPacket << "CreatePowerUp" << pos.x << pos.y << type;
							}
							else if (Content == "DecreaseSpawn") SpawnPowerUp -= 1.f;
							else if (Content == "Power") // Power ups.
							{
								string type;
								int Erase;
								packet >> type >> Erase;
								if (type == "Freeze")
								{
									float Time = 2.f;
									sendPacket << Content << type << Erase << Time;
								}
								else if (type == "Invisible")
								{
									sendPacket << Content << type << Erase << i;
								}
								else
								{
									sendPacket << Content << type << Erase;
								}
							}
							else if (Content == "FreezedColor" || Content == "ChangeBackColor")
							{
								sendPacket << Content << i;
							}
							if (Content == "CoinPos")
							{
								for (int j = 0; j < Clients.size(); j++) // Sending the data to everyone.
								{
									Clients[j]->send(sendPacket);
								}
							}
							else
							{
								for (int j = 0; j < Clients.size(); j++) // Sending the data to everyone except sender.
								{
									if (i != j) Clients[j]->send(sendPacket);
								}
							}
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < Clients.size(); i++) delete Clients[i];
}

void Client(Font* font)
{
	TcpSocket socket;
	string consoleInput;
	vector<Power> PowerUps;
	bool ClientOnline = true, GameStarted = false, Backwards = false;
	int Me, ColorNumber, ScoreLimit;
	string id;
	vector<Player> Players;
	vector<Vector2f> Res; // Resolution difference multipliers.
	vector<string> TopNames;
	RectangleShape coin;
	coin = RectangleSetter(V2f(25.f, 25.f), Vector2f(), Color::Yellow, Color::White, FO(2.f));
	cout << "Enter your name: ";
	cin >> id;
	cout << endl << "Choose the color by number: 1 Red, 2 Green, 3 Blue, 4 Yellow, 5 White, 6 Cyan" << endl;
	cin >> ColorNumber;
	cout << endl << "Write server ip: ";
	cin >> consoleInput;
	while (socket.connect(consoleInput, 53000, seconds(60.f)) != Socket::Done)
	{
		cout << "Reconnecting..." << endl;
	}
	cout << endl << "Joined the server, waiting for others..." << endl;
	Packet packet;
	packet << id;
	socket.send(packet);
	packet.clear();

	while (!GameStarted)
	{
		if (socket.receive(packet) == Socket::Done)
		{
			packet >> GameStarted;
			packet.clear();
		}
	}
	if (socket.receive(packet) == Socket::Done)
	{
		int PlayerSize, r, g, b;
		string PlayerId;
		packet >> Me >> PlayerSize >> ScoreLimit; // Receiving me and player size packet.
		packet.clear();
		switch (ColorNumber) // Player color sending to other players.
		{
			case 1:
				packet << 255 << 0 << 0;
				break;
			case 2:
				packet << 0 << 255 << 0;
				break;
			case 3:
				packet << 0 << 0 << 255;
				break;
			case 4:
				packet << 255 << 255 << 0;
				break;
			case 5:
				packet << 255 << 255 << 255;
				break;
			default:
				packet << 0 << 255 << 255;
		}
		packet << id << WINDOW_WIDTH << WINDOW_HEIGHT;
		socket.send(packet);
		packet.clear();
		for (int i = 0; i < PlayerSize; i++)
		{
			float Width, Height;
			if (socket.receive(packet) == Socket::Done)
			{
				packet >> r >> g >> b >> PlayerId >> Width >> Height;
				packet.clear();
			}
			Res.push_back(Vector2f(WINDOW_WIDTH / Width, WINDOW_HEIGHT / Height));
			Players.push_back(Player(font, PlayerId, Color(r, g, b)));
			cout << PlayerId << endl;
			if (i < 3) TopNames.push_back(Players[i].ID);
			Players[i].body.setFillColor(Color(r, g, b));
			Players[i].body.setSize(V2f(40.f, 40.f));
		}
		if (socket.receive(packet) == Socket::Done)
		{
			Vector2f CoinPos;
			packet >> CoinPos.x >> CoinPos.y;
			packet.clear();
			coin.setPosition(CoinPos.x * Res[0].x, CoinPos.y * Res[0].y);
		}
	}
	
	// --------------------------------------------------------------------------- GAME STARTS ---------------------------------------------------------------------------------
	RenderWindow window(VideoMode(int(WINDOW_WIDTH), int(WINDOW_HEIGHT)), "MULTIPLAYER GAME", Style::Fullscreen);
	Event e;
	float FreezedSeconds = 0.f, SpeedSeconds = 0.f, DoubleCoinSeconds = 0.f, BlindSeconds = 0.f, InvisibleSeconds = 0.f, BackwardsSeconds = 0.f;
	socket.setBlocking(false);

	// -------------------------- BACKGROUND COLOR CHANGER -------------------------
	bool NewBackgroundColorNeeded = true;
	string Winner;
	Color BackgroundColor = Color::Black, Next;
	Clock BackgroundTimer;

	while (ClientOnline)
	{
		if (!Players[Me].StillOn(FreezedSeconds, 0))
		{
			if (FreezedSeconds > 0.f)
			{
				Players[Me].body.setFillColor(Players[Me].BodyColor);
				packet << "ChangeBackColor";
				socket.send(packet);
				packet.clear();
			}
			FreezedSeconds = 0.f;
		}
		if (!Players[Me].StillOn(SpeedSeconds, 1)) Players[Me].Speed = 1.f;
		if (!Players[Me].StillOn(BackwardsSeconds, 5)) BackwardsSeconds = 0.f, Backwards = false;
		if (InvisibleSeconds > 0.f && !Players[Me].StillOn(InvisibleSeconds, 4)) // If invisibility ends.
		{
			InvisibleSeconds = 0.f;
			packet << "ChangeBackColor";
			socket.send(packet);
			packet.clear();
		}
		while (window.pollEvent(e))
		{
			if (Event::Closed) window.close();
			else if (e.type == Event::KeyPressed)
			{
				if (!Players[Me].StillOn(FreezedSeconds, 0))
				{
					PlayerMovement(e, Players[Me], Backwards); // Moving real player and if he can move.
				}
			}
		}
		
		if (Players[Me].PlayerMoved) // If real player moved.
		{
			packet << "PlayerPos" << Players[Me].body.getPosition().x << Players[Me].body.getPosition().y;
			socket.send(packet);
			packet.clear();
			Players[Me].PlayerMoved = false;
			for (int i = 0; i < PowerUps.size(); i++) // If player takes power up.
			{
				if (Players[Me].body.getGlobalBounds().intersects(PowerUps[i].body.getGlobalBounds()))
				{
					packet << "Power" << PowerUps[i].type << i;
					if (PowerUps[i].type == "2xSpeed") Players[Me].ResetTime(1), SpeedSeconds = 3.f, Players[Me].Speed = 2.f;
					else if (PowerUps[i].type == "2xCoin") Players[Me].ResetTime(2), DoubleCoinSeconds = 10.f;
					else if (PowerUps[i].type == "Blind") Players[Me].ResetTime(3), BlindSeconds = 10.f;
					else if (PowerUps[i].type == "Invisible") Players[Me].ResetTime(4), InvisibleSeconds = 15.f;
					PowerUps.erase(PowerUps.begin() + i);
					socket.send(packet);
					packet.clear();
					packet << "DecreaseSpawn";
					socket.send(packet);
					packet.clear();
					break;
				}
			}
			if (Players[Me].body.getGlobalBounds().intersects(coin.getGlobalBounds())) // If real player takes coin.
			{
				int Multiplier = 1;
				if (Players[Me].StillOn(DoubleCoinSeconds, 2)) Multiplier = 2;
				coin.setPosition(CoinNewPosition());
				packet << "CoinPos" << coin.getPosition().x << coin.getPosition().y << Multiplier;
				socket.send(packet);
				packet.clear();
			}
		}
		if (socket.receive(packet) == Socket::Done) // If packet received from other players.
		{
			string Content;
			int Which;
			Vector2f pos;
			packet >> Content;
			if (Content == "PlayerPos") // Any players changed positions.
			{
				packet >> Which >> pos.x >> pos.y;
				Players[Which].body.setPosition(pos.x * Res[Which].x, pos.y * Res[Which].y);
			}
			else if (Content == "CoinPos") // Any players took the coin.
			{
				int Multiplier;
				packet >> Which >> pos.x >> pos.y >> Multiplier;
				Players[Which].Score += Multiplier;
				Players[Which].ScoreText.setString(Players[Which].ID + ": " + to_string(Players[Which].Score));
				coin.setPosition(pos.x * Res[Which].x, pos.y * Res[Which].y);
				TopNames = FindingHighestScores(Players);
				if (Players[Which].Score >= ScoreLimit) ClientOnline = false, Winner = Players[Which].ID;
			}
			else if (Content == "GiveLoc") // Sending position of new power up.
			{
				string type;
				packet.clear();
				pos = PowerUpNewPosition(Players, PowerUps);
				int randNumber = rand() % 37;
				if (randNumber < 6) type = "Freeze";
				else if (randNumber < 15) type = "2xCoin";
				else if (randNumber < 18) type = "Push";
				else if (randNumber < 26) type = "2xSpeed";
				else if (randNumber < 29) type = "Invisible";
				else if (randNumber < 35) type = "Blind";
				else type = "Backwards";
				packet << Content << pos.x << pos.y << type;
				socket.send(packet);
				PowerUps.push_back(Power(pos, type));
			}
			else if (Content == "CreatePowerUp")
			{
				string type;
				packet >> pos.x >> pos.y >> type;
				PowerUps.push_back(Power(Vector2f(pos.x * Res[Which].x, pos.y * Res[Which].y), type));
			}
			else if (Content == "Power") // Some player took power up.
			{
				string type;
				int ErasePowerUp;
				packet >> type >> ErasePowerUp;
				PowerUps.erase(PowerUps.begin() + ErasePowerUp);
				if (type == "Freeze") // If power up was freeze.
				{
					packet >> FreezedSeconds;
					packet.clear();
					Players[Me].body.setFillColor(Color(0, 0, 139));
					packet << "FreezedColor"; // Sending freezed color.
					socket.send(packet);
					Players[Me].ResetTime(0);
				}
				else if (type == "Push")
				{
					packet.clear();
					int x, y;
					x = rand() % 500 + 50;
					y = rand() % 500 + 50;
					if (rand() % 2 == 0) x *= -1;
					if (rand() % 2 == 0) y *= -1;
					Players[Me].body.move(V2f(float(x), float(y)));
					Players[Me].YouText.move(V2f(float(x), float(y)));
					if (Players[Me].body.getPosition().x < 0.f) Players[Me].body.setPosition(0.f, Players[Me].body.getPosition().y), Players[Me].YouText.setPosition(FO(5.f), Players[Me].YouText.getPosition().y);
					else if (Players[Me].body.getPosition().x > WINDOW_WIDTH - Players[Me].body.getGlobalBounds().width) Players[Me].body.setPosition(WINDOW_WIDTH - Players[Me].body.getGlobalBounds().width, Players[Me].body.getPosition().y), Players[Me].YouText.setPosition(WINDOW_WIDTH - Players[Me].body.getGlobalBounds().width + FO(5.f), Players[Me].YouText.getPosition().y);
					if (Players[Me].body.getPosition().y < 0.f) Players[Me].body.setPosition(Players[Me].body.getPosition().x, 0.f), Players[Me].YouText.setPosition(Players[Me].YouText.getPosition().x, FO(10.f));
					else if (Players[Me].body.getPosition().y > WINDOW_HEIGHT - Players[Me].body.getGlobalBounds().height) Players[Me].body.setPosition(Players[Me].body.getPosition().x, WINDOW_HEIGHT - Players[Me].body.getGlobalBounds().height), Players[Me].YouText.setPosition(Players[Me].YouText.getPosition().x, WINDOW_HEIGHT - Players[Me].body.getGlobalBounds().height + FO(10.f));
					packet << "PlayerPos" << Players[Me].body.getPosition().x << Players[Me].body.getPosition().y;
					socket.send(packet);
				}
				else if (type == "Invisible")
				{
					packet >> Which;
					Players[Which].body.setFillColor(Color::Transparent), Players[Which].body.setOutlineColor(Color::Transparent);
				}
				else if (type == "Backwards")
				{
					BackwardsSeconds = 20.f;
					Backwards = true;
					Players[Me].ResetTime(5);
				}
			}
			else if (Content == "FreezedColor") // Make other player freezed color.
			{
				int Which;
				packet >> Which;
				Players[Which].body.setFillColor(Color(0, 0, 139));
			}
			else if (Content == "ChangeBackColor") // Make other player change back to their original color.
			{
				int Which;
				packet >> Which;
				Players[Which].SetBodyColor();
			}
			packet.clear();
		}

		// ------------------------------------------------------------ DRAW ----------------------------------------------------
		if (!Players[Me].StillOn(BlindSeconds, 3))
		{
			window.clear(BackgroundColor);
			BackgroundColorChanger(BackgroundColor, Next, BackgroundTimer, NewBackgroundColorNeeded);
			for (int i = 0; i < Players.size(); i++) window.draw(Players[i].body); // Draw all players.
			for (int i = 0; i < PowerUps.size(); i++) window.draw(PowerUps[i].body);
			window.draw(Players[Me].body); // Draw real player.
			DrawHighestScores(window, Players, TopNames, Me);
			window.draw(coin);
			window.draw(Players[Me].YouText);
		}
		else window.clear(Color::Black);
		window.display();
	}
	Text WinnerText = TextSetter(font, FO(150), FO(10.f), Color::White, Color::Blue, Winner + " is the WINNER", V2f(100.f, 400.f));
	Clock WindowCloses;
	while (window.isOpen())
	{
		if (WindowCloses.getElapsedTime().asSeconds() >= 10.f) window.close();
		while (window.pollEvent(e))
		{
			if (Event::Closed) window.close();
		}
		window.clear(Color::Black);
		window.draw(WinnerText);
		window.display();
	}
}

void PlayerMovement(Event e, Player& player, bool Backwards)
{
	if (e.key.code == Keyboard::W)
	{
		if (!Backwards) player.UpMovement();
		else player.DownMovement();
	}
	else if (e.key.code == Keyboard::S)
	{
		if (!Backwards) player.DownMovement();
		else player.UpMovement();
	}
	else if (e.key.code == Keyboard::A)
	{
		if (!Backwards) player.LeftMovement();
		else player.RightMovement();
	}
	else if (e.key.code == Keyboard::D)
	{
		if (!Backwards) player.RightMovement();
		else player.LeftMovement();
	}
}

Vector2f CoinNewPosition()
{
	return Vector2f(float(rand() % int(WINDOW_WIDTH - FO(25.f))), float(rand() % int(WINDOW_HEIGHT - FO(25.f))));
}

Vector2f PowerUpNewPosition(vector<Player> Players, vector<Power> PowerUps)
{
	Vector2f pos;
	while (true)
	{
		pos = Vector2f(float(rand() % int(WINDOW_WIDTH - FO(30.f))), float(rand() % int(WINDOW_HEIGHT - FO(30.f))));
		for (int i = 0; i < PowerUps.size(); i++)
		{
			if (PowerUps[i].body.getGlobalBounds().contains(pos)) break;
			if (i + 1 == PowerUps.size()) return pos;
		}
		if (PowerUps.size() == 0) return pos;
	}
}

vector<string> FindingHighestScores(vector<Player> Players)
{
	vector<int> TopScores;
	vector<string> Which;
	int score;
	for (int i = 0; i < 3; i++) TopScores.push_back(-1), Which.push_back("");
	for (int i = 0; i < Players.size(); i++)
	{
		score = Players[i].Score;
		if (score > TopScores[0])
		{
			TopScores[2] = TopScores[1];
			Which[2] = Which[1];
			TopScores[1] = TopScores[0];
			Which[1] = Which[0];
			TopScores[0] = score;
			Which[0] = Players[i].ID;
		}
		else if (score > TopScores[1])
		{
			TopScores[2] = TopScores[1];
			Which[2] = Which[1];
			TopScores[1] = score;
			Which[1] = Players[i].ID;
		}
		else if (score > TopScores[2]) TopScores[2] = score, Which[2] = Players[i].ID;
	}
	if (Players.size() < 3) Which.pop_back();
	return Which;
}

void DrawHighestScores(RenderWindow& window, vector<Player>& Players, vector<string> TopNames, int Me)
{
	bool InLeaderboard = false;
	for (int i = 0; i < TopNames.size(); i++)
	{
		if (TopNames[i] == Players[Me].ID) InLeaderboard = true;
		for (int j = 0; j < Players.size(); j++)
		{
			if (TopNames[i] == Players[j].ID)
			{
				Players[j].ScoreText.setPosition(V2f(420.f * (i + 1), 5.f));
				window.draw(Players[j].ScoreText);
				break;
			}
		}
	}
	if (!InLeaderboard)
	{
		Players[Me].ScoreText.setPosition(0.f, FO(1035.f));
		window.draw(Players[Me].ScoreText);
	}
}