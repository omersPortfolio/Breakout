#pragma once

#include "GameObject.h"

class Game;

class Ball : public GameObject
{
public:
	Ball(Game* pGame);
	~Ball();

	// Update and Draw
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& m_Window);

	sf::CircleShape GetShape() { return m_BallShape; }
	const float GetRadius() { return m_BallShape.getRadius(); }

	void SetRadius(float newRadius);

	bool CanPassThrough() { return m_CanPassThrough; }
	void SetCanPassThrough(bool bCanPassThrough) { m_CanPassThrough = bCanPassThrough; }

private:
	Game* m_pGame = nullptr;

	sf::CircleShape m_BallShape;


	float m_Radius;

	bool m_CanPassThrough;

	vec2 m_DefaultVelocity;
};