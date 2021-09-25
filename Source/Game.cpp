// Written by Omer Kocar

#include "GamePCH.h"

#include "Game.h"
#include "Ball.h"
#include "GameManager.h"
#include "AudioManager.h"
#include "Level.h"
#include "TextureManager.h"

Game::Game()
{
	//sf::VideoMode(1920, 1080), "Breakout Game - Omer Kocar"
	m_Window.create(sf::VideoMode::getDesktopMode(), "Breakout Game - Omer Kocar", sf::Style::Fullscreen);
	m_Window.setFramerateLimit(60);

	m_WindowSize = vec2((float)m_Window.getSize().x, (float)m_Window.getSize().y);

	float screenSizeX = (float)m_Window.getSize().x;
	float defaultOffset = 125.0f;

	m_pTextureManager = new TextureManager();

	// Load Levels //
	Level* level1 = new Level(this, 2);
	level1->Load("../Levels/Level1.level", screenSizeX * 0.4f, 600);
	m_Levels.push_back(level1);

	Level* level2 = new Level(this, 3);
	level2->Load("../Levels/Level2.level");
	m_Levels.push_back(level2);

	Level* level3 = new Level(this, 4);
	level3->Load("../Levels/Level3.level");
	m_Levels.push_back(level3);

	// Has one breakable brick. Aimed to test the victory screen.
	Level* level4 = new Level(this);
	level4->Load("../Levels/Level4.level", screenSizeX * 0.2f, 700);
	m_Levels.push_back(level4);

	// // // // // // // //

	m_CurrentLevel = 0;

	m_pAudioManager = new AudioManager();

	m_pGameManager = new GameManager(this);
	m_pGameManager->SetGameStateToMainMenu();
}

Game::~Game()
{
	SafeDelete( m_pGameManager );

	SafeDelete(m_pTextureManager);

	for (Level* pLevel : m_Levels)
	{
		SafeDelete( pLevel );
	}

	SafeDelete(m_pAudioManager);
}

void Game::RunGame()
{
	sf::Clock clock;
	sf::Event event;
	while (m_Window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

		while (m_Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_Window.close();	
		}

		m_Window.clear(sf::Color(234, 241, 200, 255));

		Draw();

		if (m_Window.hasFocus())
		{
			Update(deltaTime);

			OnKeyboardEvent();
			OnMouseButtonEvent(event);
		}
		
		m_Window.display();	
	}
}

void Game::Update(float deltaTime)
{
	// Update the current level
	GetActiveLevel()->Update(deltaTime);
}

void Game::Draw()
{
	// Draw the current level
	GetActiveLevel()->Draw(m_Window);

	m_pGameManager->Draw(m_Window);
}

void Game::OnKeyboardEvent()
{	
	GetActiveLevel()->OnKeyboardEvent();
}

void Game::OnMouseButtonEvent(sf::Event& event)
{
	GetActiveLevel()->OnMouseEvent(event);

	// Main Menu Buttons
	if (m_pGameManager->GetGameState() == GameState::MainMenu)
	{
		if (m_pGameManager->GetLevel1Button_Main().IsClicked(m_Window, event))
		{
			LoadLevel(0);
			GetActiveLevel()->GetBall()->SetSpeed(BALL_SPEED + 100.0f);
		}
		else if (m_pGameManager->GetLevel2Button_Main().IsClicked(m_Window, event))
		{
			LoadLevel(1);
		}
		else if (m_pGameManager->GetLevel3Button_Main().IsClicked(m_Window, event))
		{
			LoadLevel(2);
		}
		else if (m_pGameManager->GetLevel4Button_Main().IsClicked(m_Window, event))
		{
			LoadLevel(3);
		}
		else if (m_pGameManager->GetControlsMenuButton_Main().IsClicked(m_Window, event))
		{
			m_pGameManager->SetGameStateToControlsMenu();
		}
	}
	// Control Menu Buttons
	else if (m_pGameManager->GetGameState() == GameState::ControlsMenu)
	{
		if (m_pGameManager->GetBackButton_Controls().IsClicked(m_Window, event))
		{
			m_pGameManager->SetGameStateToMainMenu();
		}
	}
	// Victory Menu Buttons
	else if (m_pGameManager->GetGameState() == GameState::Victory)
	{
		if (m_pGameManager->GetContinueButton_Victory().IsClicked(m_Window, event))
		{
			LoadNextLevel();
		}
		else if (m_pGameManager->GetRestartLevelButton_Victory().IsClicked(m_Window, event))
		{
			LoadLevel(m_CurrentLevel);
		}
		else if (m_pGameManager->GetBackToMainMenuButton_Victory().IsClicked(m_Window, event))
		{
			m_pGameManager->SetGameStateToMainMenu();
		}
	}
}

void Game::LoadLevel(unsigned int levelToLoad)
{
	// Set the index to levelToLoad
	m_CurrentLevel = levelToLoad;
	// Load the level with the given index
	m_Levels.at(levelToLoad)->OnLevelStart();
}

void Game::LoadNextLevel()
{
	// if we're in the final level, load the first one.
	if (m_CurrentLevel == m_Levels.size() - 1)
	{
		m_CurrentLevel = 0;
		m_Levels.at(m_CurrentLevel)->OnLevelStart();
	}
	// if we're not, load the next level
	else
	{
		m_CurrentLevel++;
		m_Levels.at(m_CurrentLevel)->OnLevelStart();
	}
}

Level* Game::GetActiveLevel()
{
	return m_Levels[m_CurrentLevel];
}

Level* Game::GetLevel(unsigned int index)
{
	return m_Levels.at(index);
}

std::vector<Level*> Game::GetLevels()
{
	return m_Levels;
}

void Game::HandleGameOver()
{
	m_pGameManager->SetGameStateToGameOver();
	m_pAudioManager->StopAllSounds();
	GetActiveLevel()->RemoveAllPowerups();
}

float Game::GetLeftBoundary()
{
	return WALL_THICKNESS / 2;
}

float Game::GetRightBoundary()
{
	return m_Window.getSize().x - WALL_THICKNESS / 2;
}

float Game::GetBottomBoundary()
{
	return m_Window.getSize().y + WALL_THICKNESS;
}