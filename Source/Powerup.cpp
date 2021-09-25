#include "GamePCH.h"

#include "Powerup.h"
#include "Brick.h"
#include "Game.h"
#include "Ball.h"
#include "Level.h"
#include "AudioManager.h"
#include "Paddle.h"

Powerup::Powerup(Game* pGame, PowerupType powerupType, sf::Color color, float duration) :
	m_PaddleSizeUpTween(Back::Out, m_SizeUpXValue, PADDLE_WIDTH, 250, 0.75f),
	m_PaddleSizeDownTween(Back::Out, m_SizeDownXValue, 250, PADDLE_WIDTH, 0.75f),
	m_BallSizeUpTween(Linear::Lerp, m_BallSizeUpRadius, BALL_RADIUS, 40.0f, 0.25f),
	m_BallSizeDownTween(Linear::Lerp, m_BallSizeDownRadius, 40.0f, BALL_RADIUS, 0.25f)
{
	m_pGame = pGame;

	m_PowerupType = powerupType;

	m_PowerupShape = sf::RectangleShape(sf::Vector2f(100, 40));
	m_PowerupShape.setOrigin(m_PowerupShape.getSize().x / 2.0f, m_PowerupShape.getSize().y / 2.0f);
	m_PowerupShape.setOutlineColor(sf::Color::Black);
	m_PowerupShape.setOutlineThickness(5.0f);

	m_PowerupShape.setFillColor(color);

	m_Duration = duration;
	m_InitialDuration = m_Duration;
	m_IsActivated = false;
	m_Speed = Math::RandomFloatInRange(220, 700.0f);

	// Powerup Text on the powerup shape //
	m_Font.loadFromFile("../Fonts/pingwing.ttf");
	powerupText.setFont(m_Font);
	powerupText.setCharacterSize(14);
	powerupText.setFillColor(sf::Color::White);
	powerupText.setOutlineColor(sf::Color::Black);
	powerupText.setOutlineThickness(2);
	powerupText.setPosition(m_Position.x, m_Position.y);
	powerupText.setOrigin(m_PowerupShape.getPosition().x / 2, m_PowerupShape.getPosition().y / 2);

	// Powerup text based on the PowerupType
	switch (m_PowerupType)
	{
	case PowerupType::PaddleSizeShifter:
		powerupText.setString("Paddle Size");
		break;
	case PowerupType::BallSizeShifter:
		powerupText.setString("Ball Size");
		break;
	case PowerupType::BallSpeedShifter:
		powerupText.setString("Ball Speed");
		break;
	case PowerupType::BallPassThrough:
		powerupText.setString("Pass Through");
		break;
	case PowerupType::Bullet:
		powerupText.setString("Bullet");
		break;
	}

	name = powerupText.getString();
}

Powerup::~Powerup()
{

}

void Powerup::Update(float deltaTime)
{
	// Have the powerup shape follow the powerup's position 
	m_PowerupShape.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
	powerupText.setPosition(m_Position.x - m_PowerupShape.getSize().x / 2 + 5, m_Position.y);
	m_Position.y += m_Speed * deltaTime;

	if (GetPosition().y >= m_pGame->GetBottomBoundary())
	{
		SetActive(false);
	}

	// if overlapped with the Paddle Brick
	if (GetShape().getGlobalBounds().intersects(m_pGame->GetActiveLevel()->GetPaddleBrick()->GetShape().getGlobalBounds()))
	{
		// Activate the powerup
		ActivatePowerups();
	}

	// If Activated
	if (IsActivated())
	{
		// Start the timer for duration
		m_Duration -= deltaTime;

		// Deactivate the powerup when it reaches 0
		if (m_Duration <= 0.0f)
		{
			DeactivateAllPowerups();
		}
	}

	// Update the tweens if any of them is running
	if (m_PaddleSizeUpTween.IsRunning())
	{
		m_pGame->GetActiveLevel()->GetPaddleBrick()->SetSizeX(m_SizeUpXValue);
		m_PaddleSizeUpTween.Update(deltaTime);
	}

	if (m_PaddleSizeDownTween.IsRunning())
	{
		m_pGame->GetActiveLevel()->GetPaddleBrick()->SetSizeX(m_SizeDownXValue);
		m_PaddleSizeDownTween.Update(deltaTime);
	}

	if (m_BallSizeUpTween.IsRunning())
	{
		m_pGame->GetActiveLevel()->GetBall()->SetRadius(m_BallSizeUpRadius);
		m_BallSizeUpTween.Update(deltaTime);
	}

	if (m_BallSizeDownTween.IsRunning())
	{
		m_pGame->GetActiveLevel()->GetBall()->SetRadius(m_BallSizeDownRadius);
		m_BallSizeDownTween.Update(deltaTime);
	}
}

void Powerup::ActivatePowerups()
{
	SetActive(false);

	m_Duration = m_InitialDuration;

	// If another powerup of same type is not activated already
	if (!IsOtherPowerupActive())
	{
		// Play Powerup Activation Sound
		m_pGame->GetAudioManager()->PlayPowerupActivateSound(Math::RandomFloatInRange(0.75f, 1.25f), Math::RandomFloatInRange(0.75f, 1.25f));

		// Get a reference to the ball
		Ball* pBall = m_pGame->GetActiveLevel()->GetBall();
		SetActivated(true);

		// Activate the powerup based on Type
		switch (m_PowerupType)
		{
		case PowerupType::PaddleSizeShifter:
			m_PaddleSizeUpTween.Start();
			break;
		case PowerupType::Bullet:
			m_pGame->GetActiveLevel()->GetPaddle()->SetCanShootBullets(true);
			break;
		case PowerupType::BallSizeShifter:
			if (pBall->IsActive())
			{
				pBall->SetRadius(m_BallSizeUpRadius);
				pBall->GetShape().setOrigin(sf::Vector2f(pBall->GetShape().getRadius(), pBall->GetShape().getRadius()));
				m_BallSizeUpTween.Start();
			}
			break;
		case PowerupType::BallSpeedShifter:
			if (pBall->IsActive())
			{
				if (!pBall->IsVelocityMultiplied())
				{
					pBall->MultiplyVelocity(1.2f);
					pBall->SetIsVelocityMultiplied(true);
				}
			}
			break;
		case PowerupType::BallPassThrough:
			if (pBall->IsActive())
			{
				pBall->SetCanPassThrough(true);
			}
			break;
		}
	}
}

void Powerup::Draw(sf::RenderWindow& m_Window)
{
	m_Window.draw(m_PowerupShape);
	m_Window.draw(powerupText);
}

void Powerup::DeactivateAllPowerups()
{
	if (IsActive())
		SetActive(false);

	SetPosition(-500, 0);

	SetActivated(false);
	m_Duration = m_InitialDuration;

	if (GetPowerupType() == PowerupType::PaddleSizeShifter)
	{
		Brick* pPaddleBrick = m_pGame->GetActiveLevel()->GetPaddleBrick();
		pPaddleBrick->SetSizeX(m_SizeDownXValue);
		m_PaddleSizeDownTween.Start();
	}
	else if (GetPowerupType() == PowerupType::Bullet)
	{
		Paddle* pPaddle = m_pGame->GetActiveLevel()->GetPaddle();
		pPaddle->SetCanShootBullets(false);
	}
	else
	{
		Ball* pBall = m_pGame->GetActiveLevel()->GetBall();
		if (GetPowerupType() == PowerupType::BallSizeShifter)
		{
			m_BallSizeDownTween.Start();
			pBall->SetRadius(BALL_RADIUS);
			pBall->GetShape().setOrigin(sf::Vector2f(pBall->GetShape().getRadius(), pBall->GetShape().getRadius()));
		}
		else if (GetPowerupType() == PowerupType::BallSpeedShifter)
		{
			pBall->MultiplyVelocity(0.84f);
			pBall->SetIsVelocityMultiplied(false);
			
		}
		else if (GetPowerupType() == PowerupType::BallPassThrough)
		{
			if (pBall->CanPassThrough())
			{
				pBall->SetCanPassThrough(false);
			}
		}
	}
}

bool Powerup::IsOtherPowerupActive()
{
	for (Powerup* pPowerup : m_pGame->GetActiveLevel()->GetPowerups())
	{
		if (pPowerup == this)
			continue;

		if (pPowerup->IsActivated())
		{
			if (pPowerup->GetPowerupType() == GetPowerupType())
			{
				return true;
			}
		}
	}
	return false;
}
