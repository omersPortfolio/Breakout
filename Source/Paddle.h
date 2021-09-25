#pragma once

#include "GameObject.h"

class Game;
class Bullet;
class Ball;

class Paddle : public GameObject
{
public:
	Paddle(Game* pGame);
	~Paddle();

	virtual void Update(float deltaTime) override;

	void OnKeyboardEvent();

	// Movement input
	void HandleMovement(float deltaTime);

	vec2 GetSpawnPosition() { return m_SpawnPosition; }

	void Damage(int damageAmount);

	void SetCanShootBullets(bool bCanShoot) { m_CanShootBullets = bCanShoot; }
	bool CanShootBullets() { return m_CanShootBullets; }

	void SpawnBall(Ball* pBall, bool isSpawnDirectionRandom = true);

private:
	void SpawnBullet(Bullet* pBullet);
	

private:
	Game* m_pGame = nullptr;

	vec2 m_SpawnPosition;
	vec2 m_Direction;

	bool m_CanShootBullets;

	float m_BulletFireTimer;
	float m_FireRate;
};