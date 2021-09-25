#include "GamePCH.h"

#include "GameManager.h"
#include "Paddle.h"
#include "Game.h"
#include "Brick.h"
#include "Level.h"
#include "AudioManager.h"

GameManager::GameManager(Game* pGame)
{
	m_pGame = pGame;

	m_Font.loadFromFile("../Fonts/pingwing.ttf");
	m_DefaultFontSize = 24;

	m_WindowSize = vec2((float)m_pGame->GetWindow().getSize().x, (float)m_pGame->GetWindow().getSize().y);

	// Initialize menu items
	InitMainMenuItems();
	InitPauseMenuItems();
	InitControlsMenuItems();
	InitGameOverItems();
	InitInGameItems();
	InitVictoryItems();
}

GameManager::~GameManager()
{

}

void GameManager::InitMainMenuItems()
{
	Button m_MainMenuButton("BREAKOUT", vec2(m_WindowSize.x / 2, 100), m_Font, 96, sf::Color::Red);
	m_MainMenuButtons.push_back(m_MainMenuButton);

	Button level1Button("Level 1", vec2(250, m_WindowSize.y / 2 - 100), m_Font, 48, sf::Color::White, vec2(300, 120), sf::Color(125, 125, 125, 125));
	m_MainMenuButtons.push_back(level1Button);

	Button level2Button("Level 2", vec2(600, m_WindowSize.y / 2 - 100), m_Font, 48, sf::Color::White, vec2(300, 120), sf::Color(125, 125, 125, 125));
	m_MainMenuButtons.push_back(level2Button);

	Button level3Button("Level 3", vec2(250, m_WindowSize.y / 2 + 100), m_Font, 48, sf::Color::White, vec2(300, 120), sf::Color(125, 125, 125, 125));
	m_MainMenuButtons.push_back(level3Button);

	Button level4Button("Level 4", vec2(600, m_WindowSize.y / 2 + 100), m_Font, 48, sf::Color::White, vec2(300, 120), sf::Color(125, 125, 125, 125));
	m_MainMenuButtons.push_back(level4Button);

	Button creditButton("Omer Kocar", vec2(m_WindowSize.x - 200.0f, m_WindowSize.y - 50), m_Font, 48, sf::Color::Red);
	m_MainMenuButtons.push_back(creditButton);

	Button controlsButton("Controls", vec2(250, m_WindowSize.y / 2 + 300), m_Font, 48, sf::Color::White, vec2(300, 120), sf::Color(125, 125, 125, 125));
	m_MainMenuButtons.push_back(controlsButton);
}

void GameManager::InitControlsMenuItems()
{
	Button backButton("BACK", vec2(m_WindowSize.x / 2, m_WindowSize.y - 100), m_Font, 48, sf::Color::White, vec2(300, 120), sf::Color(125, 125, 125, 125));
	m_ControlsMenuButtons.push_back(backButton);

	Button wasdButton("WASD - Move", vec2(400, 150), m_Font, 48, sf::Color::White);
	m_ControlsMenuButtons.push_back(wasdButton);

	Button spawnBallButton("LMB / Space - Spawn Ball", vec2(400, 300), m_Font, 48, sf::Color::White);
	m_ControlsMenuButtons.push_back(spawnBallButton);

	Button shootBulletsButton("C - Shoot Bullets", vec2(400, 450), m_Font, 48, sf::Color::White);
	m_ControlsMenuButtons.push_back(shootBulletsButton);

	Button escButton("Esc - Pause Menu", vec2(400, 600), m_Font, 48, sf::Color::White);
	m_ControlsMenuButtons.push_back(escButton);

	Button cheatsText("CHEATS", vec2(m_WindowSize.x / 2 + 200, 150), m_Font, 54, sf::Color::White);
	m_ControlsMenuButtons.push_back(cheatsText);

	Button resetButton("R - Restart Level", vec2(m_WindowSize.x / 2 + 200, 300), m_Font, 32, sf::Color::White);
	m_ControlsMenuButtons.push_back(resetButton);

	Button f1Text("F1 - Restore Health", vec2(m_WindowSize.x / 2 + 200, 400), m_Font, 32, sf::Color::White);
	m_ControlsMenuButtons.push_back(f1Text);

	Button f2Text("F2 - Allow Bullets", vec2(m_WindowSize.x / 2 + 200, 500), m_Font, 32, sf::Color::White);
	m_ControlsMenuButtons.push_back(f2Text);

	Button f3Text("F3 - Spawn Powerups", vec2(m_WindowSize.x / 2 + 200, 600), m_Font, 32, sf::Color::White);
	m_ControlsMenuButtons.push_back(f3Text);

	Button f4Text("F4 - Spawn Ball towards Mouse Position", vec2(m_WindowSize.x / 2 + 350, 700), m_Font, 32, sf::Color::White);
	m_ControlsMenuButtons.push_back(f4Text);
}

void GameManager::InitPauseMenuItems()
{
	Button bg("", vec2(m_WindowSize.x / 2, m_WindowSize.y / 2), m_Font, 48, sf::Color::White, vec2(m_WindowSize.x, m_WindowSize.y), sf::Color(0, 0, 0, 170));
	m_PauseMenuButtons.push_back(bg);

	Button m_PauseMenuButton("PAUSE MENU", vec2(m_WindowSize.x / 2, 100), m_Font, 96, sf::Color::Red);
	m_PauseMenuButtons.push_back(m_PauseMenuButton);

	Button m_ResumeButton("RESUME ", vec2(m_WindowSize.x / 2, m_WindowSize.y / 2 - 100), m_Font, 48, sf::Color::White, vec2(300, 120), sf::Color(125, 125, 125, 125));
	m_PauseMenuButtons.push_back(m_ResumeButton);

	Button m_BackToMainMenuButton("MAIN MENU", vec2(m_WindowSize.x / 2, m_WindowSize.y / 2 + 100), m_Font, 48, sf::Color::White, vec2(300, 120), sf::Color(125, 125, 125, 125));
	m_PauseMenuButtons.push_back(m_BackToMainMenuButton);
}

void GameManager::InitInGameItems()
{
	healthText.setString(std::string("Health: " + std::to_string(m_pGame->GetActiveLevel()->GetPaddle()->GetHealth())));
	healthText.setFont(m_Font);
	healthText.setCharacterSize(48);
	healthText.setFillColor(sf::Color::Red);
	healthText.setStyle(sf::Text::Bold);
	healthText.setPosition(sf::Vector2f(25, 25));
	inGameUITexts.push_back(healthText);
}

void GameManager::InitGameOverItems()
{
	Button m_GameOverButton("GAME OVER", vec2(m_WindowSize.x / 2, 100), m_Font, 96, sf::Color::Red);
	m_GameOverButtons.push_back(m_GameOverButton);

	Button m_RestartLevelButton("RESTART LEVEL", vec2(m_WindowSize.x / 2, m_WindowSize.y / 2 - 100), m_Font, 96, sf::Color::White, vec2(700, 150), sf::Color(125, 125, 125, 125));
	m_GameOverButtons.push_back(m_RestartLevelButton);

	Button m_MainMenuButton("MAIN MENU", vec2(m_WindowSize.x / 2, m_WindowSize.y / 2 + 100), m_Font, 72, sf::Color::White, vec2(400, 150), sf::Color(125, 125, 125, 125));
	m_GameOverButtons.push_back(m_MainMenuButton);
}

void GameManager::InitVictoryItems()
{
	Button m_VictoryButton("VICTORY", vec2(m_WindowSize.x / 2, 100), m_Font, 96, sf::Color::Red);
	m_VictoryButtons.push_back(m_VictoryButton);

	Button m_ContinueButton("CONTINUE", vec2(m_WindowSize.x / 2, m_WindowSize.y / 2 - 100), m_Font, 96, sf::Color::White, vec2(500, 150), sf::Color(125, 125, 125, 125));
	m_VictoryButtons.push_back(m_ContinueButton);

	Button m_RestartLevelButton("RESTART LEVEL", vec2(m_WindowSize.x / 2, m_WindowSize.y / 2 + 100), m_Font, 72, sf::Color::White, vec2(700, 150), sf::Color(125, 125, 125, 125));
	m_VictoryButtons.push_back(m_RestartLevelButton);

	Button m_MainMenuButton("MAIN MENU", vec2(m_WindowSize.x / 2, m_WindowSize.y / 2 + 300), m_Font, 72, sf::Color::White, vec2(400, 150), sf::Color(125, 125, 125, 125));
	m_VictoryButtons.push_back(m_MainMenuButton);
}

void GameManager::SetGameStateToMainMenu()
{
	gameState = GameState::MainMenu;

	m_pGame->GetAudioManager()->StopAllSounds();
}

void GameManager::SetGameStateToControlsMenu()
{
	gameState = GameState::ControlsMenu;
}

void GameManager::SetGameStateToPauseMenu()
{
	gameState = GameState::PauseMenu;
}

void GameManager::SetGameStateToInGame()
{
	gameState = GameState::InGame;
}

void GameManager::UpdateInGameUI()
{
	inGameUITexts.clear();
	healthText.setString(std::string("Health: " + std::to_string(m_pGame->GetActiveLevel()->GetPaddle()->GetHealth())));
	inGameUITexts.push_back(healthText);
}

void GameManager::SetGameStateToGameOver()
{
	gameState = GameState::GameOver;

	m_pGame->GetAudioManager()->PlayGameOverSound();

	// Have bricks start falling down
	for (Brick* pBrick : m_pGame->GetActiveLevel()->GetBricks())
	{
		if (pBrick->IsMainBrick())
		{
			pBrick->GetBrickFallDownTween().SetStartValue(pBrick->GetPosition().y);
			pBrick->GetBrickFallDownTween().Start();
		}
	}
}

void GameManager::SetGameStateToVictory()
{
	gameState = GameState::Victory;

	m_pGame->GetAudioManager()->PlayApplauseSound();
}

void GameManager::Draw(sf::RenderTarget& window)
{
	// Draw particular buttons based on GameState
	switch (gameState)
	{
	case GameState::MainMenu:
		for (Button button : m_MainMenuButtons)
		{
			button.Draw(window);
		}
		break;
	case GameState::InGame:
		for (sf::Text text : inGameUITexts)
		{
			window.draw(text);
		}
		break;
	case GameState::PauseMenu:
		for (Button button : m_PauseMenuButtons)
		{
			button.Draw(window);
		}
		break;
	case GameState::GameOver:
		for (Button& button : m_GameOverButtons)
		{
			button.Draw(window);
		}
		break;
	case GameState::ControlsMenu:
		for (Button button : m_ControlsMenuButtons)
		{
			button.Draw(window);
		}
		break;
	case GameState::Victory:
		for (Button button : m_VictoryButtons)
		{
			button.Draw(window);
		}
		break;
	}
}

unsigned int GameManager::GetFontSize()
{
	return m_DefaultFontSize;
}

void GameManager::SetFontSize(unsigned int newSize)
{
	m_DefaultFontSize = newSize;
}
