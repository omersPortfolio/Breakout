#pragma once


class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void LoadFromFile(sf::SoundBuffer& soundBuffer, const std::string& filename);

	// Did not have the time to figure out a single Play function for any sound
	void PlayHitSound(float volume = 1.0f, float pitch = 1.0f);
	void PlayPowerupSpawnSound(float volume = 1.0f, float pitch = 1.0f);
	void PlayPowerupActivateSound(float volume = 1.0f, float pitch = 1.0f);
	void PlayBrickFallDownSound(float volume = 1.0f, float pitch = 1.0f);
	void PlayApplauseSound(float volume = 1.0f, float pitch = 1.0f);
	void PlayGameOverSound(float volume = 1.0f, float pitch = 1.0f);

	void StopAllSounds();

private:
	sf::SoundBuffer powerupActivateSoundBuffer;
	sf::Sound powerupActivateSound;

	sf::SoundBuffer hitSoundBuffer;
	sf::Sound hitSound;

	sf::SoundBuffer powerupSpawnSoundBuffer;
	sf::Sound powerupSpawnSound;

	sf::SoundBuffer brickFallDownSoundBuffer;
	sf::Sound brickFallDownSound;

	sf::SoundBuffer applauseSoundBuffer;
	sf::Sound applauseSound;

	sf::SoundBuffer gameOverSoundBuffer;
	sf::Sound gameOverSound;
};