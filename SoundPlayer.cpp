#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(SoundBuffer* buffer, const string& name)
{
	this->sound.setBuffer(*buffer);
	if (name == "PlayerHurt") this->sound.setVolume(20.f);
	else if (name == "BoulderTrap") this->sound.setVolume(50.f);
}

void SoundPlayer::playSound(deque<SoundPlayer>& sounds)
{
	sounds.back().sound.play();
}

void SoundPlayer::deleteIfStoppedSound(deque<SoundPlayer>& sounds)
{
	while (sounds.size() > 0)
	{
		if (sounds[0].sound.getStatus() == Sound::Stopped) sounds.pop_front();
		else break;
	}
}