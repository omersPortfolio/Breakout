#include "GamePCH.h"

#include "Paddle.h"
#include "Game.h"
#include "Level.h"
#include "Bullet.h"
#include "Ball.h"
#include "GameManager.h"
#include "Brick.h"


Paddle::Paddle(Game* pGame)
{
	m_pGame = pGame;

	SetActive(true);
	
	m_Speed = PADDLE_MAX_SPEED;

	m_Direction = vec2(0.0f, 0.0f);
	m_SpawnPosition = vec2((float)m_pGame->GetWindow().getSize().x / 2, (float)m_pGame->GetWindow().getSize().y - 50);
	m_Position = m_SpawnPosition;
	
	m_MaxHealth = 3;
	m_Health = m_MaxHealth;

	m_CanShootBullets = false;
	m_FireRate = 0.2f;
}

Paddle::~Paddle()
{

}

void Paddle::Update(float deltaTime)
{
	HandleMovement(deltaTime);	

	// Keep the Paddle within the game's boundaries //
	if (m_Position.x < m_pGame->GetLeftBoundary() + 75)
	{
		m_Position.x = m_pGame->GetLeftBoundary() + 75;
	}
	if (m_Position.x > m_pGame->GetRightBoundary() - 75)
	{
		m_Position.x = m_pGame->GetRightBoundary() - 75;
	}

	// Timer for bullet fire rate
	if (m_BulletFireTimer < m_FireRate)
	{
		m_BulletFireTimer += deltaTime;
	}
}

void Paddle::HandleMovement(float deltaTime)
{
	m_Direction = vec2(0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_Direction.x -= 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_Direction.x += 1;
	}

	// Smooth Movement logic //
	float drag = -PADDLE_DEFAULT_DRAG;
	float acceleration = m_Direction.x * PADDLE_DEFAULT_ACCELERATION;

	if (acceleration != 0.0f)
	{
		drag = (acceleration - (acceleration * PADDLE_DEFAULT_DRAG)) / acceleration;
	}

	m_Velocity.x += acceleration + (m_Velocity.x * drag) * powf(deltaTime, 0.5f);
	m_Velocity.x = Math::Clamp(m_Velocity.x, -m_Speed, m_Speed);

	m_Position.x += m_Velocity.x * deltaTime;
}

void Paddle::OnKeyboardEvent()
{
	if (m_pGame->GetGameManager()->GetGameState() == GameState::InGame)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && m_BulletFireTimer > m_FireRate)
		{
			if (CanShootBullets())
			{
				Bullet* pBullet = nullptr;

				for (int i = 0; i < NUM_BULLETS; i++)
				{
					if (m_pGame->GetActiveLevel()->GetBullets()[i]->IsActive() == false)
					{
						pBullet = m_pGame->GetActiveLevel()->GetBullets()[i];
						break;
					}
				}

				if (pBullet)
				{
					m_BulletFireTimer = 0.0f;
					SpawnBullet(pBullet);
				}
			}
		}
		// Spawn ball if all bricks are done falling down at level start
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_pGame->GetActiveLevel()->GetBricks().back()->GetBrickStartFallTween().IsFinished())
		{
			Ball* pBall = m_pGame->GetActiveLevel()->GetBall();
			if (pBall->IsActive() == false)
			{
				SpawnBall(pBall);
			}
		}
	}
}


void Paddle::Damage(int damageAmount)
{
	// Clamp health between newHealth and maxHealth
	GameObject::Damage(damageAmount);
	
	// Set game state to game over if health is 0.
	if (m_Health <= 0)
	{
		m_pGame->HandleGameOver();
	}
}

void Paddle::SpawnBullet(Bullet* pBullet)
{
	vec2 spawnPos = GetPosition();
	spawnPos.y -= 30.0f;

	pBullet->SetActive(true);
	pBullet->SetPosition(spawnPos);
}

void Paddle::SpawnBall(Ball* pBall, bool isSpawnDirectionRandom /* = true */)
{
	vec2 spawnPos = GetPosition();
	spawnPos.y -= 40.0f;

	pBall->SetActive(true);
	pBall->SetPosition(spawnPos);

	if (isSpawnDirectionRandom)
	{
		// Random Direction //
		float minAngle = Math::DegreesToRadians(45);
		float maxAngle = Math::DegreesToRadians(180.0f - minAngle);
		float angle = Math::RandomFloatInRange(minAngle, maxAngle);

		vec2 dir = vec2(cos(angle), sin(angle));

		pBall->SetVelocity(-dir.GetNormalized() * pBall->GetSpeed());
	}
	else
	{
		// Direction in Mouse position //
		vec2 mousePosition = vec2((float)sf::Mouse::getPosition(m_pGame->GetWindow()).x, (float)sf::Mouse::getPosition(m_pGame->GetWindow()).y);
		vec2 direction = spawnPos - mousePosition;
		if (direction.y < 0)
		{
			direction = vec2(-0.5f, 1.0f);
		}

		direction.Normalize();
		pBall->SetVelocity(-direction * pBall->GetSpeed());
	}
}
