#pragma once

#include "GameObject.h"

class Brick;
class Game;

enum class PowerupType
{
	PaddleSizeShifter,
	BallSizeShifter,
	BallSpeedShifter,
	BallPassThrough,
	Bullet,
	None
};

class Powerup : public GameObject
{
public:
	Powerup(Game* pGame, PowerupType powerupType, sf::Color color, float duration);
	~Powerup();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& m_Window);

	void ActivatePowerups();
	void DeactivateAllPowerups();
	bool IsOtherPowerupActive();

	// Getters
	sf::RectangleShape& GetShape()					{ return m_PowerupShape; }
	PowerupType GetPowerupType()					{ return m_PowerupType; }
	bool IsActivated()								{ return m_IsActivated; }

	// Setters
	void SetPowerupType(PowerupType newPowerupType) { m_PowerupType = newPowerupType; }
	void SetActivated(bool bActivate)				{ m_IsActivated = bActivate; }

private:
	Game* m_pGame = nullptr;

	sf::RectangleShape m_PowerupShape;
	sf::Text powerupText;
	sf::Font m_Font;

	std::string name;

	PowerupType m_PowerupType;

	float m_Duration;
	float m_InitialDuration;

	bool m_IsActivated;

	// Powerup Tweens //
	Tween m_PaddleSizeUpTween;
	float m_SizeUpXValue;

	Tween m_PaddleSizeDownTween;
	float m_SizeDownXValue;

	Tween m_BallSizeUpTween;
	float m_BallSizeUpRadius;

	Tween m_BallSizeDownTween;
	float m_BallSizeDownRadius;
};