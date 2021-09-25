#include "GamePCH.h"

#include "Ball.h"
#include "Game.h"
#include "GameManager.h"
#include "Level.h"
#include "Paddle.h"
#include "TextureManager.h"

Ball::Ball(Game* pGame)
{
	m_pGame = pGame;

 	m_Radius = 18.0f;
	m_BallShape = sf::CircleShape(BALL_RADIUS);

	// Circle shape setup
	m_BallShape.setOrigin(sf::Vector2f(m_BallShape.getRadius(), m_BallShape.getRadius()));
	m_BallShape.setFillColor(sf::Color::Green);
	m_BallShape.setOutlineColor(sf::Color::Black);
	m_BallShape.setOutlineThickness(3.0f);
	m_BallShape.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
	m_BallShape.setTexture(&m_pGame->GetTextureManager()->GetBallTexture());

	m_CanPassThrough = false;

	m_Speed = BALL_SPEED;

	SetPosition(-500, 0);
}

Ball::~Ball()
{

}

void Ball::Update(float deltaTime)
{
	// have the circle shape follow the Ball's position
	m_BallShape.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
	// Rotate the ball
	m_BallShape.rotate(360.0f * deltaTime);

	// if out of bounds, deactivate, damage player and remove all powerups.
	if (m_Position.x < m_pGame->GetLeftBoundary() - 50.0f || m_Position.x > m_pGame->GetRightBoundary() + 50.0f || m_Position.y > m_pGame->GetBottomBoundary() + 50.0f)
	{
		SetActive(false);
		m_pGame->GetActiveLevel()->DamagePlayer(1);
		m_pGame->GetGameManager()->UpdateInGameUI();
		m_pGame->GetActiveLevel()->RemoveAllPowerups();
	}
}

void Ball::Draw(sf::RenderWindow& m_Window)
{
	m_Window.draw(m_BallShape);
}

void Ball::SetRadius(float newRadius)
{
	m_BallShape.setRadius(newRadius);
	m_BallShape.setOrigin(sf::Vector2f(newRadius, newRadius));
}