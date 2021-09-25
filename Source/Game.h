#pragma once

class RenderWindow;
class Paddle;
class Ball;
class Brick;
class GameManager;
class AudioManager;
class Powerup;
class Level;
class TextureManager;

class Game
{
public:
	Game();
	~Game();

	// Main Game Loop
	void RunGame();

	void Draw();
	void Update(float deltaTime);

	// Screen Boundaries
	float GetLeftBoundary();
	float GetRightBoundary();
	float GetBottomBoundary();

	vec2 GetScreenSize() { return m_WindowSize; }

	// Mouse Event
	void OnMouseButtonEvent(sf::Event& event);
	// Keyboard Event
	void OnKeyboardEvent();

	// TODO: Move all the object logic into Level

	GameManager* GetGameManager() { return m_pGameManager; }
	AudioManager* GetAudioManager() { return m_pAudioManager; }
	TextureManager* GetTextureManager() { return m_pTextureManager; }

	Level* GetActiveLevel();
	Level* GetLevel(unsigned int index);
	std::vector<Level*> GetLevels();
	void LoadLevel(unsigned int levelToLoad);
	void LoadNextLevel();

	sf::RenderWindow &GetWindow() { return m_Window; }

	void HandleGameOver();

private:
	sf::RenderWindow m_Window;
	vec2 m_WindowSize;

	std::vector<Level*> m_Levels;

	GameManager* m_pGameManager = nullptr;
	AudioManager* m_pAudioManager = nullptr;
	TextureManager* m_pTextureManager = nullptr;

	unsigned int m_CurrentLevel;
};