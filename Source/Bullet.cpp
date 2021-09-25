#include "GamePCH.h"

#include "Bullet.h"
#include "Level.h"
#include "Paddle.h"
#include "Game.h"
#include "Brick.h"

Bullet::Bullet(Game* pGame)
{
	m_pGame = pGame;

	m_BulletShape = sf::RectangleShape(sf::Vector2f(5, 15));
	m_BulletShape.setOrigin(m_BulletShape.getSize().x / 2.0f, m_BulletShape.getSize().y / 2.0f);
	m_BulletShape.setFillColor(sf::Color::Red);
	m_BulletShape.setOutlineColor(sf::Color::Black);
	m_BulletShape.setOutlineThickness(5.0f);

	SetSpeed(600.0f);
	SetPosition(-500, 0);
	SetDirection(vec2(0, -1));
	SetActive(false);
	SetVelocity(m_Direction * m_Speed);
}

Bullet::~Bullet()
{

}

void Bullet::Update(float deltaTime)
{
	m_BulletShape.setPosition(m_Position.x, m_Position.y);

	m_Position += m_Velocity * deltaTime;

	for (Brick* pBrick : m_pGame->GetActiveLevel()->GetBricks())
	{
		if (pBrick->IsActive())
		{
			if (m_BulletShape.getGlobalBounds().intersects(pBrick->GetShape().getGlobalBounds()))
			{
				SetActive(false);
				if (pBrick->IsBreakable())
				{
					m_pGame->GetActiveLevel()->HandleBreakableBrickCollision(pBrick);
				}
			}
		}
	}

	if (m_Position.y < -50.0f)
	{
		SetActive(false);
	}
	

}

void Bullet::Draw(sf::RenderWindow& m_Window)
{
	m_Window.draw(m_BulletShape);
}

void Bullet::Spawn()
{
	vec2 spawnPos = m_pGame->GetActiveLevel()->GetPaddle()->GetPosition();
	spawnPos.y -= 30.0f;

	SetActive(true);
	SetPosition(spawnPos);
	SetVelocity(m_Direction * m_Speed);
}

