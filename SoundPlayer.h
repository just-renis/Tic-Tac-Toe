#pragma once
#include "VoidFunctions.h"

class SoundPlayer
{
public:
	SoundPlayer(SoundBuffer* buffer, const string& name);
	static void playSound(deque<SoundPlayer>& sounds);
	static void deleteIfStoppedSound(deque<SoundPlayer>& sounds);
private:
	Sound sound;
};