#pragma once

#include "vec2.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update(float deltaTime);

	// Getters
	vec2 GetPosition()			{ return m_Position; }
	vec2 GetVelocity()			{ return m_Velocity; }
	vec2 GetDirection()			{ return m_Direction; }
	virtual vec2 GetSize()		{ return m_Size; }
	bool IsActive()				{ return m_Active; }
	float GetSpeed()			{ return m_Speed; }
	int GetHealth()				{ return m_Health; }
	int GetMaxHealth()			{ return m_MaxHealth; }
	bool IsVelocityMultiplied()	{ return m_IsVelocityMultiplied; }

	// Setters
	void SetPosition(vec2 position)					{ m_Position = position; }
	void SetPosition(float x, float y)				{ m_Position.x = x, m_Position.y = y; }
	void SetPosition(sf::Vector2f position)			{ m_Position.Set(position.x, position.y); }
	void SetPositionX(float x)						{ m_Position.x = x; }
	void SetPositionY(float y)						{ m_Position.y = y; }
	void SetVelocity(vec2 velocity)					{ m_Velocity = velocity; }
	void MultiplyVelocity(float scale)				{ m_Velocity *= scale; }
	void SetIsVelocityMultiplied(bool bMultiplied)  { m_IsVelocityMultiplied = bMultiplied; }
	void SetDirection(vec2 direction)				{ m_Direction = direction; }
	void SetActive(bool bActive)					{ m_Active = bActive; }
	void SetSpeed(float newSpeed)					{ m_Speed = newSpeed; }
	void SetHealth(int newHealth)					{ m_Health = newHealth; }
	void SetMaxHealth(int newMaxHealth)				{ m_MaxHealth = newMaxHealth; }
	void SetSize(vec2 newSize) {}
	void SetSizeX(float newSizeX) {}
	void SetDefaultPosition(vec2 newPosition)		{ m_DefaultPosition = newPosition; }

	virtual void Damage(int damageAmount);

protected:
	vec2 m_Velocity;
	bool m_IsVelocityMultiplied;
	vec2 m_Position;
	vec2 m_Direction;
	bool m_Active;

	float m_Speed;
	int m_Health;
	int m_MaxHealth;
	vec2 m_Size;

	vec2 m_DefaultPosition;
	vec2 m_DefaultSize;
};