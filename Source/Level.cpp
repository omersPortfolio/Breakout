#include "GamePCH.h"

#include "Level.h"
#include "Brick.h"
#include "Paddle.h"
#include "Game.h"
#include "Ball.h"
#include "GameManager.h"
#include "AudioManager.h"
#include "Powerup.h"
#include "Bullet.h"
#include "TextureManager.h"

Level::Level(Game* pGame, int chanceToSpawnPowerups) :
	m_pGame(pGame),
	m_WallSizeTween(Linear::Lerp, m_WallSizeYValue, 0.0f, (float)m_pGame->GetWindow().getSize().y, 1.0f, -0.5f)
{
	// Chance to Spawn Powerups in. 1 = 100%, 2 = 50%...
	m_ChanceToSpawnPowerups = chanceToSpawnPowerups;

	// Create the Paddle
	m_pPaddle = new Paddle(pGame);

	// Create the Ball
	m_pBall = new Ball(pGame);
	m_pBall->SetActive(false);

	// Pool the Powerups
	for (unsigned int i = 0; i < NUM_POWERUPS; i++)
	{
		Powerup* pPowerUp = nullptr;
		int randomNum = Math::RandomIntInRange(1, 5);
		if (randomNum == 1)
		{
			pPowerUp = new Powerup(pGame, PowerupType::PaddleSizeShifter, sf::Color::Green, 5.0f);
			m_Powerups.push_back(pPowerUp);
			pPowerUp->SetActive(false);
		}
		else if (randomNum == 2)
		{
			pPowerUp = new Powerup(pGame, PowerupType::BallSpeedShifter, sf::Color::Blue, 3.0f);
			m_Powerups.push_back(pPowerUp);
			pPowerUp->SetActive(false);
		}
		else if (randomNum == 3)
		{
			pPowerUp = new Powerup(pGame, PowerupType::BallPassThrough, sf::Color::Yellow, 3.5f);
			m_Powerups.push_back(pPowerUp);
			pPowerUp->SetActive(false);
		}
		else if (randomNum == 4)
		{
			pPowerUp = new Powerup(pGame, PowerupType::BallSizeShifter, sf::Color::Black, 5.0f);
			m_Powerups.push_back(pPowerUp);
			pPowerUp->SetActive(false);
		}
		else if (randomNum == 5)
		{
			pPowerUp = new Powerup(pGame, PowerupType::Bullet, sf::Color::Red, 4.0f);
			m_Powerups.push_back(pPowerUp);
			pPowerUp->SetActive(false);
		}
	}

	// Pool bullets
	for (unsigned int i = 0; i < NUM_BULLETS; i++)
	{
		Bullet* pBullet = new Bullet(pGame);
		m_Bullets.push_back(pBullet);
	}
}

Level::~Level()
{
	SafeDelete(m_pBall);

	SafeDelete(m_pPaddle);

	for (Brick* pBrick : m_Bricks)
	{
		SafeDelete(pBrick);
	}

	for (Powerup* pPowerup : m_Powerups)
	{
		SafeDelete(pPowerup);
	}

	for (Bullet* pBullet : m_Bullets)
	{
		SafeDelete(pBullet);
	}
}

void Level::Load(const char* file, float gameSpace, float offset)
{
	m_LevelFileName = file;
	m_GameSpace = gameSpace;
	m_BrickOffset = offset;
	m_Bricks.clear();

	unsigned int tileCode;
	std::string line;
	/*	Input stream class to operate on files.
		Objects of this class maintain a filebuf object as their internal stream buffer, 
		which performs input/output operations on the file they are associated with (if any). */
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;

	if (fstream)
	{
		// read each line in the .level file
		while (std::getline(fstream, line))
		{
			/*	Input stream class to operate on strings.
				Objects of this class use a string buffer that contains a sequence of characters. 
				This sequence of characters can be accessed directly as a string object, using member str. */ 
			std::istringstream sstream(line);
			std::vector<unsigned int> row;

			// extract each value into row array
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);
			}
			// when done with a row, push the row into tileData
			tileData.push_back(row);
		}
		// when done reading the .level file and if tileData is not empty,
		// Initialize the level
		if (tileData.size() > 0)
		{
			InitLevel(tileData, gameSpace, offset);
		}
	}
}

void Level::InitLevel(std::vector<std::vector<unsigned int>> tileData, float gameSpace, float offset)
{
	// Get screen size
	vec2 screenSize((float)m_pGame->GetWindow().getSize().x, (float)m_pGame->GetWindow().getSize().y);
	float wallThickness = WALL_THICKNESS;

	/* Player, Walls and Bricks are all Brick objects.
	   Player, with an exception, is a paddle that has a brick shape attached on it.
	   This is so the ball does not need to check for collision with a 'Paddle' object beside the Bricks.
	   Player's game logic is handled inside Paddle */
	{
		// Allocate 4 bricks for player's brick and 3 walls.
		for (int i = 0; i < 4; i++)
		{
			Brick* pBrick = new Brick();
			m_Bricks.push_back(pBrick);
		}

		// Brick[0] is Player Paddle
		m_Bricks[0]->SetActive(true);
		m_Bricks[0]->SetPosition(vec2(m_pPaddle->GetSpawnPosition().x, m_pPaddle->GetSpawnPosition().y));
		m_Bricks[0]->SetColor(sf::Color::Black);
		m_Bricks[0]->SetOutlineColorAndThickness(sf::Color::Red, 5.0f);
		m_Bricks[0]->SetSize(vec2(PADDLE_WIDTH, PADDLE_HEIGHT));
		m_Bricks[0]->SetIsMainBrick(false);
		m_Bricks[0]->SetBreakable(false);

		// Top wall
		m_Bricks[1]->SetActive(true);
		m_Bricks[1]->SetPosition(vec2(screenSize.x / 2, 5.0f));
		m_Bricks[1]->SetSize(vec2(screenSize.x, wallThickness));
		m_Bricks[1]->SetColor(sf::Color::Black);
		m_Bricks[1]->SetIsMainBrick(false);
		m_Bricks[1]->SetOutlineColorAndThickness(sf::Color::Red, 3.0f);
		m_Bricks[1]->SetBreakable(false);

		// Left wall
		m_Bricks[2]->SetActive(true);
		m_Bricks[2]->SetPosition(vec2(5.0f, screenSize.y / 2));
		m_Bricks[2]->SetSize(vec2(wallThickness, screenSize.y));
		m_Bricks[2]->SetColor(sf::Color::Black);
		m_Bricks[2]->SetOutlineColorAndThickness(sf::Color::Red, 3.0f);
		m_Bricks[2]->SetIsMainBrick(false);
		m_Bricks[2]->SetIsVerticalWall(true);
		m_Bricks[2]->SetBreakable(false);

		// Right wall
		m_Bricks[3]->SetActive(true);
		m_Bricks[3]->SetPosition(vec2(screenSize.x - 5.0f, screenSize.y / 2));
		m_Bricks[3]->SetSize(vec2(wallThickness, screenSize.y));
		m_Bricks[3]->SetColor(sf::Color::Black);
		m_Bricks[3]->SetIsMainBrick(false);
		m_Bricks[3]->SetOutlineColorAndThickness(sf::Color::Red, 3.0f);
		m_Bricks[3]->SetIsVerticalWall(true);
		m_Bricks[3]->SetBreakable(false);
	}

	// num of bricks on Y
	unsigned int height = static_cast<unsigned int>(tileData.size());
	// num of bricks on X
	unsigned int width = static_cast<unsigned int>(tileData[0].size());

	//float gameSpace = screenSize.x - 120 * 2 - wallThickness * 2;

	vec2 brickSize(gameSpace / width, 60);
	m_BrickSize = brickSize;

	// Base spawn position for the bricks
	vec2 basePosition(WALL_THICKNESS + offset + (brickSize.x / 2), -BRICK_SPAWN_OFFSET + 50);
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			// if 0 is read, continue. These are empty spots
			if (tileData[y][x] == 0)
				continue;

			Brick* pBrick = new Brick();
			pBrick->SetActive(true);
			pBrick->SetPosition(basePosition + vec2((brickSize.x * x), (brickSize.y * y)));
			pBrick->SetSize(brickSize);
			pBrick->SetOutlineColorAndThickness(sf::Color::Black, 4.0f);

			// 1: solid bricks
			if (tileData[y][x] == 1)
			{
				pBrick->SetBreakable(false);
				pBrick->GetShape().setFillColor(sf::Color(125, 125, 125));
			}
			// > 1: Breakable bricks
			else if (tileData[y][x] > 1)
			{
				pBrick->SetBreakable(true);
				switch(tileData[y][x])
				{
				case 2:
					pBrick->GetShape().setFillColor(sf::Color(255, 121, 0)); // Heat wave color / Orange
					break;
				case 3:
					pBrick->GetShape().setFillColor(sf::Color(157, 2, 8)); // Red
					break;
				case 4:
					pBrick->GetShape().setFillColor(sf::Color(123, 44, 191)); // French violet
					break;
				case 5:
					pBrick->GetShape().setFillColor(sf::Color(0, 250, 125)); // Light Green
					break;
				}
			}

			pBrick->SetIsMainBrick(true);
			m_Bricks.push_back(pBrick);
		}
	}

	numOfBricksLeft = m_Bricks.size() - 5;
}

void Level::DamagePlayer(int damage)
{
	m_pPaddle->Damage(damage);
}

void Level::Update(float deltaTime)
{
	// if not in main menu or controls menu
	if (m_pGame->GetGameManager()->GetGameState() != GameState::MainMenu || m_pGame->GetGameManager()->GetGameState() != GameState::ControlsMenu)
	{
		// Update the bricks
		for (Brick* pBrick : m_Bricks)
		{
			if (pBrick->IsActive())
			{
				if (pBrick->IsVerticalWall())
				{
					pBrick->SetSize(vec2(WALL_THICKNESS, m_WallSizeYValue));
				}
				else if (pBrick->IsMainBrick())
				{
					pBrick->SetSize(vec2(m_BrickSize.x, BRICK_HEIGHT));
				}
				pBrick->Update(deltaTime);
			}
		}
	}

	// if In Game
	if (m_pGame->GetGameManager()->GetGameState() == GameState::InGame)
	{
		// Set game state to victory if all bricks are destroyed
		/*if (IsVictory())
		{
			m_pGame->GetGameManager()->SetGameStateToVictory();
		}*/

		// Set the first brick's position to Paddle's position.
		m_Bricks[0]->SetPosition(m_pPaddle->GetPosition());

		// Update paddle if it's active
		if (m_pPaddle->IsActive())
		{
			m_pPaddle->Update(deltaTime);
		}

		// Update active powerups
		for (Powerup* pPowerup : m_Powerups)
		{
			pPowerup->Update(deltaTime);
		}

		// Update ball if it's active
		if (m_pBall->IsActive())
		{
			UpdateBall(deltaTime);
		}

		// Update wallSizeTween if it's started
		m_WallSizeTween.Update(deltaTime);
		// Set game reset to ready when wallSizeTween ends.
		if (m_WallSizeTween.IsFinished())
		{
			m_IsReadyToReset = true;
		}

		// Update active bullets
		for (Bullet* pBullet : m_Bullets)
		{
			if (pBullet->IsActive())
			{
				pBullet->Update(deltaTime);
			}
		}
	}
}

void Level::Draw(sf::RenderWindow& m_Window)
{
	// if we're not in main menu or controls menu
	if (m_pGame->GetGameManager()->GetGameState() != GameState::MainMenu && m_pGame->GetGameManager()->GetGameState() != GameState::ControlsMenu)
	{
		// Draw bricks
		for (Brick* pBrick : m_Bricks)
		{
			// Wait slightly for walls to start their size Tween before drawing.
			if (pBrick->IsActive() && m_WallSizeYValue > 5.0f)
				pBrick->Draw(m_Window);
		}

		// if we're not in gameover screen
		if (m_pGame->GetGameManager()->GetGameState() != GameState::GameOver)
		{
			// Draw the ball
			if (m_pBall->IsActive())
				m_pBall->Draw(m_Window);

			// Draw Powerups
			for (Powerup* pPowerup : m_Powerups)
			{
				if (pPowerup->IsActive())
					pPowerup->Draw(m_Window);
			}

			// Draw Bullets
			for (Bullet* pBullet : m_Bullets)
			{
				if (pBullet->IsActive())
					pBullet->Draw(m_Window);
			}
		}
	}
}

void Level::OnKeyboardEvent()
{
	if (m_pGame->GetGameManager()->GetGameState() == GameState::InGame)
	{
		m_pPaddle->OnKeyboardEvent();

		// Open up Pause Menu on Escape
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_pGame->GetGameManager()->SetGameStateToPauseMenu();
		}
		
		// Cheats //
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			// Reset level if level is ready for it.
			if (m_IsReadyToReset)
			{
				OnLevelStart();
				m_IsReadyToReset = false;
			}
		}
		// Restore player's health
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
		{
			m_pPaddle->SetHealth(m_pPaddle->GetMaxHealth());
			m_pGame->GetGameManager()->UpdateInGameUI();
		}
		// Make player be able to shoot bullets
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
		{
			m_pPaddle->SetCanShootBullets(true);
		}
		// Spawn Random Powerups
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
		{
			SpawnPowerup(vec2(Math::RandomFloatInRange(0, m_pGame->GetScreenSize().x), m_pGame->GetScreenSize().y / 2), 1);
		}
		// Spawn Ball towards the mouse position
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
		{
			m_pPaddle->SpawnBall(m_pBall, false);
		}
	}
}

void Level::OnMouseEvent(sf::Event& event)
{
	// Pause Menu Buttons
	if (m_pGame->GetGameManager()->GetGameState() == GameState::PauseMenu)
	{
		if (m_pGame->GetGameManager()->GetResumeButton_Pause().IsClicked(m_pGame->GetWindow(), event))
		{
			m_pGame->GetGameManager()->SetGameStateToInGame();

		}
		else if (m_pGame->GetGameManager()->GetBackToMainMenuButton_Pause().IsClicked(m_pGame->GetWindow(), event))
		{
			m_pGame->GetGameManager()->SetGameStateToMainMenu();
		}
	}
	// Game Over Screen Buttons
	else if (m_pGame->GetGameManager()->GetGameState() == GameState::GameOver)
	{
		if (m_pGame->GetGameManager()->GetRestartLevelButton_GameOver().IsClicked(m_pGame->GetWindow(), event))
		{
			OnLevelStart();
		}
		else if (m_pGame->GetGameManager()->GetBackToMainMenuButton_GameOver().IsClicked(m_pGame->GetWindow(), event))
		{
			m_pGame->GetGameManager()->SetGameStateToMainMenu();
		}
	}
}

bool Level::SpawnPowerup(vec2 spawnPos, int chanceToSpawnIn /* = 1 */)
{
	if (Math::RandomIntInRange(1, m_ChanceToSpawnPowerups) == 1)
	{
		// Set the first powerup to be active and push a duplicate to the end and erase the first one.
		m_Powerups[0]->SetActive(true);
		m_Powerups[0]->SetPosition(spawnPos);
		/*m_Powerups.push_back(m_Powerups[0]);
		m_Powerups.erase(m_Powerups.begin());*/
		std::rotate(m_Powerups.begin(), m_Powerups.begin() + 1, m_Powerups.end());

		return true;
	}
	return false;
}

void Level::RemoveAllPowerups()
{
	//Powerup* pPowerup : m_Powerups
	for(unsigned int i = 0; i < m_Powerups.size(); i++)
	{
		if (m_Powerups[i]->IsActivated())
		{
			m_Powerups[i]->DeactivateAllPowerups();
		}
	}
}

bool Level::IsVictory()
{
	for (Brick* pBrick : this->m_Bricks)
	{
		if (pBrick->IsMainBrick() && pBrick->IsBreakable() && pBrick->IsActive())
		{
			return false;
		}
	}
	return true;
}

void Level::HandleBreakableBrickCollision(Brick* pBrick)
{
	if (pBrick->IsBreakable())
	{
		pBrick->StartDestroy();

		// if a powerup spawns
		if (SpawnPowerup(vec2(pBrick->GetPosition().x, pBrick->GetPosition().y + 30)))
		{
			// Play the Powerup sound
			m_pGame->GetAudioManager()->PlayPowerupSpawnSound(1.5f, Math::RandomFloatInRange(0.75f, 1.25f));
		}
		// if not, play Hit Sound.
		else
		{
			m_pGame->GetAudioManager()->PlayHitSound(1.5f, Math::RandomFloatInRange(0.75f, 1.25f));
		}
	}
	// if the brick is solid
	else
	{
		// Play Hit Sound
		m_pGame->GetAudioManager()->PlayHitSound(1.5f, Math::RandomFloatInRange(0.75f, 1.25f));
	}
}

void Level::OnLevelStart()
{
	// Load the level with its properties
	Load(m_LevelFileName, m_GameSpace, m_BrickOffset);
	
	m_WallSizeTween.Restart();

	for (Brick* pBrick : m_Bricks)
	{
		if (pBrick->IsMainBrick())
		{
			pBrick->GetBrickStartFallTween().SetStartValue(pBrick->GetPosition().y);
			pBrick->GetBrickStartFallTween().SetDuration(Math::RandomFloatInRange(1.5f, 2.0f));
			pBrick->GetBrickStartFallTween().SetEndValue(pBrick->GetPosition().y + BRICK_SPAWN_OFFSET);
			pBrick->GetBrickStartFallTween().Restart();
		}
	}

	for (Powerup* pPowerup : m_Powerups)
	{
		pPowerup->SetActive(false);
		pPowerup->SetPosition(-500, 0);
	}

	m_pGame->GetAudioManager()->StopAllSounds();
	m_pGame->GetAudioManager()->PlayBrickFallDownSound(4.0f);
	m_pGame->GetGameManager()->SetGameStateToInGame();
	m_pBall->SetActive(false);
	m_pPaddle->SetHealth(m_pPaddle->GetMaxHealth());
	m_pPaddle->SetPosition(m_pPaddle->GetSpawnPosition());
	m_pGame->GetGameManager()->UpdateInGameUI();
	m_pPaddle->SetCanShootBullets(false);
}

Brick* Level::FindNearestBrickOnCircleSweep(LineSegment path, float radius, vec2& outIntersectionPoint, vec2& outNormal)
{
	Brick* pClosestBrick = nullptr;
	float closestDistance = FLT_MAX;

	for (Brick* pBrick : m_Bricks)
	{
		if (pBrick->IsActive() == false)
			continue;

		AABB box = pBrick->GetAABB();

		vec2 point;
		vec2 normal;

		/*	Calculate intersection times with each side's plane IF the path from current point to future point crosses that plane.
		*	Check each side's quadrant for single-side intersection, return the intersection.
		*	Determine the corner that the circle is colliding with.
		*	Solve the triangle between the current point, the corner, and the intersecting center (radius away from the corner).
		*	Calculate time, normal, and intersection center. */
		bool hit = box.DoesIntersectWithCircle(path, radius + 10.0f, point, normal);

		if (hit)
		{
			float distance = point.DistanceSquared(path.m_Start);
			if (distance < closestDistance)
			{
				pClosestBrick = pBrick;
				closestDistance = distance;

				outIntersectionPoint = point;
				outNormal = normal;
			}
		}
	}
	return pClosestBrick;
}

void Level::UpdateBall(float deltaTime)
{
	m_pBall->Update(deltaTime);

	vec2 position = m_pBall->GetPosition();
	vec2 velocity = m_pBall->GetVelocity();

	// we know where the circle wants to go. We know the intersection point. We reflect off of the intersection point.
	// we move 80% of the vector and reflect the 20% of the vector, which is displacementRemaining.
	vec2 displacementRemaining = velocity * deltaTime;

	// Keep looping, until there are no more collisions,
	// reducing the displacementRemaining each time.
	bool collided = true;
	while (collided)
	{
		vec2 intersectionPoint;
		vec2 normal;
		LineSegment path(position, position + displacementRemaining);
		Brick* pBrickHit = FindNearestBrickOnCircleSweep(path, m_pBall->GetRadius() + 5.0f, intersectionPoint, normal);
		
		// if the ball hit a brick
		if (pBrickHit != nullptr)
		{
			// This is the leftover velocity in a sense. the direction of the ball.
			displacementRemaining = path.m_End - intersectionPoint;

			// if the ball does not have PassThrough Powerup
			if (!m_pBall->CanPassThrough())
			{
				// Reflect the velocity and displacementRemaining off the normal.
				
				/*	reflection = dir - 2 * normal * (dir . normal)
				*						
				*					 	^	
				* 					  \	| /
				* 					   \|/
				* 						\
				* 						.\
				* 
				* 	. = DotProduct result (negative). Vector pointing down the normal vector
				*	Then we double the length of it with and flip it .
				*	Dir gives us the reflection vector
				*	dir = dir - 2 * normal * (dir . normal) */
				velocity -= 2  * normal * (velocity.DotProduct(normal));
				displacementRemaining -= 2 * normal * (displacementRemaining.DotProduct(normal));

				position = intersectionPoint;
				
				HandleBreakableBrickCollision(pBrickHit);
			}
			// if the ball does have PassThrough Powerup
			else
			{
				// if the brick is not breakable and is not a main brick.
				if (!pBrickHit->IsMainBrick())
				{
					// Reflect the velocity and displacementRemaining off the normal.
					velocity -= 2 * normal * (velocity.DotProduct(normal));
					displacementRemaining -= 2 * normal * (displacementRemaining.DotProduct(normal));

					position = intersectionPoint;

					m_pGame->GetAudioManager()->PlayHitSound(1.5f, Math::RandomFloatInRange(0.75f, 1.25f));
				}
				else
				{
					SpawnPowerup(vec2(pBrickHit->GetPosition().x, pBrickHit->GetPosition().y + 30));
					pBrickHit->SetActive(false);
				}
			}
			
			numOfBricksLeft--;
			if (numOfBricksLeft <= 0)
			{
				m_pGame->GetGameManager()->SetGameStateToVictory();
			}
		}
		else
		{
			// No collision, so move the full amount and exit this loop.
			position = path.m_End;
			collided = false;
		}
	}

	m_pBall->SetPosition(position);
	m_pBall->SetVelocity(velocity);
}

