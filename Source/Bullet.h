#pragma once

#include "GameObject.h"

class Game;

class Bullet : public GameObject
{
public:
	Bullet(Game* pGame);
	~Bullet();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& m_Window);

	void Spawn();

private:
	sf::RectangleShape m_BulletShape;

	Game* m_pGame;
};