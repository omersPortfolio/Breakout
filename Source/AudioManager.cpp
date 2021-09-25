#include "GamePCH.h"

#include "AudioManager.h"

AudioManager::AudioManager()
{
	LoadFromFile(hitSoundBuffer, "hitSound.wav");
	LoadFromFile(powerupSpawnSoundBuffer, "powerupSpawn.wav");
	LoadFromFile(powerupActivateSoundBuffer, "powerupActivate.wav");
	LoadFromFile(brickFallDownSoundBuffer, "brickFallDown.wav");
	LoadFromFile(applauseSoundBuffer, "applause.wav");
	LoadFromFile(gameOverSoundBuffer, "gameOver.wav");

	hitSound.setBuffer(hitSoundBuffer);
	powerupSpawnSound.setBuffer(powerupSpawnSoundBuffer);
	powerupActivateSound.setBuffer(powerupActivateSoundBuffer);
	brickFallDownSound.setBuffer(brickFallDownSoundBuffer);
	brickFallDownSound.setBuffer(brickFallDownSoundBuffer);
	applauseSound.setBuffer(applauseSoundBuffer);
	gameOverSound.setBuffer(gameOverSoundBuffer);
}

AudioManager::~AudioManager()
{
	
}

void AudioManager::LoadFromFile(sf::SoundBuffer& soundBuffer, const std::string& filename)
{
	if (!soundBuffer.loadFromFile("../Audio/" + filename))
		return;
}

void AudioManager::PlayHitSound(float volume, float pitch)
{
	hitSound.stop();
	hitSound.play();
	hitSound.setVolume(volume);
	hitSound.setPitch(pitch);
}

void AudioManager::PlayPowerupSpawnSound(float volume /*= 1*/, float pitch)
{
	powerupSpawnSound.stop();
	powerupSpawnSound.play();
	powerupSpawnSound.setVolume(volume);
	powerupSpawnSound.setPitch(pitch);
}

void AudioManager::PlayPowerupActivateSound(float volume /*= 1*/, float pitch /*= 1*/)
{
	powerupActivateSound.stop();
	powerupActivateSound.play();
	powerupActivateSound.setVolume(volume);
	powerupActivateSound.setPitch(pitch);
}

void AudioManager::PlayBrickFallDownSound(float volume /*= 1*/, float pitch /*= 1*/)
{
	brickFallDownSound.stop();
	brickFallDownSound.play();
	brickFallDownSound.setVolume(volume);
	brickFallDownSound.setPitch(pitch);
}

void AudioManager::PlayApplauseSound(float volume /*= 1*/, float pitch /*= 1*/)
{
	applauseSound.stop();
	applauseSound.play();
	applauseSound.setVolume(volume);
	applauseSound.setPitch(pitch);
}

void AudioManager::PlayGameOverSound(float volume /*= 1*/, float pitch /*= 1*/)
{
	gameOverSound.stop();
	gameOverSound.play();
	gameOverSound.setVolume(volume);
	gameOverSound.setPitch(pitch);
}

void AudioManager::StopAllSounds()
{
	brickFallDownSound.stop();
	hitSound.stop();
	powerupActivateSound.stop();
	powerupSpawnSound.stop();
	applauseSound.stop();
}
