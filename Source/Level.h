#pragma once

class Brick;
class Paddle;
class Game;
class Powerup;
class Ball;
class Bullet;

class Level
{
public:
	Level(Game* pGame, int chanceToSpawnPowerups = 4);
	~Level();

	// Load Level Data
	void Load(const char* file, float gameSpace = 1660.0f, float offset = 125.0f);

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& m_Window);

	void OnMouseEvent(sf::Event& event);
	void OnKeyboardEvent();

	// Getters
	Paddle* GetPaddle()					 { return m_pPaddle; }
	Brick* GetPaddleBrick()				 { return m_Bricks[0]; }
	Ball* GetBall()						 { return m_pBall; }
	std::vector<Brick*> GetBricks()		 { return m_Bricks; }
	std::vector<Powerup*> GetPowerups()  { return m_Powerups; }
	std::vector<Bullet*> GetBullets()    { return m_Bullets; }
	const char* GetLevelFilename()		 { return m_LevelFileName; }
	float GetGameSpace()				 { return m_GameSpace; }
	float GetBrickOffset()				 { return m_BrickOffset; }
	int GetChanceToSpawnPowerups()		 { return m_ChanceToSpawnPowerups; }

	// Handles Level Start logic
	void OnLevelStart();

	// Deactivates powerups
	void RemoveAllPowerups();

	void SetIsVictory(bool bVictory) { m_IsVictory = bVictory; }
	bool IsVictory();

	// Ball's collision logic with breakable bricks
	void HandleBreakableBrickCollision(Brick* pBrick);

	void DamagePlayer(int damage);

private:
	// Initialize level data
	void InitLevel(std::vector<std::vector<unsigned int>> tileData, float gameSpace, float offset);

	// Finds the nearest brick
	Brick* FindNearestBrickOnCircleSweep(LineSegment path, float radius, vec2& outIntersectionPoint, vec2& outNormal);
	void UpdateBall(float deltaTime);

	bool SpawnPowerup(vec2 spawnPos, int chanceToSpawnIn = 1);

private:
	Game* m_pGame = nullptr;
	Ball* m_pBall = nullptr;
	Paddle* m_pPaddle = nullptr;

	// Vectors that hold objects
	std::vector<Powerup*> m_Powerups;
	std::vector<Brick*> m_Bricks;
	std::vector<Bullet*> m_Bullets;

	vec2 m_BrickSize = vec2(0,0);

	// Whether it's ready to reset the game with R.
	bool m_IsReadyToReset;

	bool m_IsVictory;

	unsigned int numOfBricksLeft;

	// Chance to Spawn Powerups in. 1 = 100%, 2 = 50%...
	int m_ChanceToSpawnPowerups;

	// Level's file name
	const char* m_LevelFileName;

	// Tween for Wall Size on level start
	Tween m_WallSizeTween;
	float m_WallSizeYValue;

	// Game Space for the level
	float m_GameSpace;
	// Bricks x offset for the level
	float m_BrickOffset;	
};