#pragma once
#include "Button.h"

enum class GameState
{
	MainMenu,
	ControlsMenu,
	InGame,
	PauseMenu,
	Victory,
	GameOver
};

class Game;
class Button;

class GameManager
{
public:
	GameManager(Game* pGame);

	~GameManager();

	// Initialize the Buttons //
	void InitMainMenuItems();
	void InitControlsMenuItems();
	void InitPauseMenuItems();
	void InitInGameItems();
	void InitGameOverItems();
	void InitVictoryItems();

	// Change Game State //
	void SetGameStateToMainMenu();
	void SetGameStateToControlsMenu();
	void SetGameStateToPauseMenu();
	void SetGameStateToInGame();
	void SetGameStateToGameOver();
	void SetGameStateToVictory();

	// Update health text on Player Damage
	void UpdateInGameUI();

	GameState GetGameState() { return gameState; }
	void SetGameState(GameState newState) { gameState = newState; }

	void Draw(sf::RenderTarget& window);

	unsigned int GetFontSize();

	void SetFontSize(unsigned int newSize);

	// Getters for the Buttons
	Button& GetLevel1Button_Main()				{ return m_MainMenuButtons[1];		}
	Button& GetLevel2Button_Main()				{ return m_MainMenuButtons[2];		}
	Button& GetLevel3Button_Main()				{ return m_MainMenuButtons[3];		}
	Button& GetLevel4Button_Main()				{ return m_MainMenuButtons[4];		}
	Button& GetControlsMenuButton_Main()		{ return m_MainMenuButtons.back();  }
	Button& GetBackButton_Controls()			{ return m_ControlsMenuButtons[0];  }
	Button& GetResumeButton_Pause()				{ return m_PauseMenuButtons[2];		}
	Button& GetBackToMainMenuButton_Pause()		{ return m_PauseMenuButtons[3];		}
	Button& GetRestartLevelButton_GameOver()	{ return m_GameOverButtons[1];		}
	Button& GetBackToMainMenuButton_GameOver()	{ return m_GameOverButtons[2];		}
	Button& GetContinueButton_Victory()			{ return m_VictoryButtons[1];		}
	Button& GetRestartLevelButton_Victory()		{ return m_VictoryButtons[2];		}
	Button& GetBackToMainMenuButton_Victory()	{ return m_VictoryButtons[3];		}

private:
	Game* m_pGame;
	vec2 m_WindowSize;

	GameState gameState;

	// Button vectors for each screen
	std::vector<Button> m_MainMenuButtons;
	std::vector<Button> m_PauseMenuButtons;
	std::vector<Button> m_GameOverButtons;
	std::vector<Button> m_ControlsMenuButtons;
	std::vector<Button> m_VictoryButtons;

	std::vector<sf::Text> inGameUITexts;
	sf::Text healthText;
	sf::Font m_Font;

	unsigned int m_DefaultFontSize;
};