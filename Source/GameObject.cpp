#include "GamePCH.h"

#include "GameObject.h"

GameObject::GameObject()
{
	m_Active = false;

	m_Position = vec2(0, 0);
}

GameObject::~GameObject()
{

}

void GameObject::Update(float deltaTime)
{
	if (m_Active == false)
		return;

	// Change position based on Velocity * deltaTime
	//m_Position += m_Velocity * deltaTime;
}

void GameObject::Damage(int damageAmount)
{
	m_Health = Math::Clamp(m_Health -= damageAmount, 0, m_MaxHealth);
}
