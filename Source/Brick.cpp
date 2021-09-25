#include "GamePCH.h"

#include "Brick.h"

Brick::Brick() :
	m_BrickStartFallTween(Back::Out, m_BrickStartFallValue, GetPosition().y, 200, 2.0f, -1.0f),
	m_DestroyTween(Linear::Lerp, m_DestroySizeX, BRICK_HEIGHT, 0.0f, 0.1f),
	m_BrickFallDownTween(Linear::Lerp, m_BrickFallDownValue, GetPosition().y, 1600, Math::RandomFloatInRange(1.0f, 5.0f))
{
	isMainBrick = false;
	m_IsVerticalWall = false;
}

Brick::~Brick()
{

}

void Brick::Draw(sf::RenderWindow& m_Window)
{
	if (m_BrickShape.getSize().x > 0)
		m_Window.draw(m_BrickShape);
}

void Brick::Update(float deltaTime)
{
	m_BrickShape.setPosition(m_Position.x, m_Position.y);

	aabb.m_Upper = m_Position + GetSize() / 2;
	aabb.m_Lower = m_Position - GetSize() / 2;

	// Destroy Tween on Ball Collision
	if (m_DestroyTween.IsRunning())
	{
		m_DestroyTween.Update(deltaTime);
		m_BrickShape.setSize(sf::Vector2f(m_Size.x, m_DestroySizeX));

		if (m_DestroySizeX <= 5.0f)
			SetActive(false);
	}

	// Fall Down tween on Start
	if (m_BrickStartFallTween.IsRunning())
	{
		SetPosition(m_Position.x, m_BrickStartFallValue);
		m_BrickStartFallTween.Update(deltaTime);
	}

	// Fall Down tween on Game Over
	if (m_BrickFallDownTween.IsRunning())
	{
		SetPosition(m_Position.x, m_BrickFallDownValue);
		m_BrickFallDownTween.Update(deltaTime);
	}
}

void Brick::SetSize(vec2 newSize)
{
	m_BrickShape.setSize(sf::Vector2f(newSize.x, newSize.y));
	m_Size = newSize;
	m_BrickShape.setOrigin(newSize.x / 2.0f, newSize.y / 2.0f);
	
}

void Brick::SetSizeX(float newSizeX)
{
	m_BrickShape.setSize(sf::Vector2f(newSizeX, m_Size.y));
	m_Size.x = newSizeX;
	m_BrickShape.setOrigin(m_Size.x / 2.0f, m_Size.y / 2.0f);
}

void Brick::SetOutlineColorAndThickness(sf::Color color, float thickness)
{
	m_BrickShape.setOutlineColor(color);
	m_BrickShape.setOutlineThickness(thickness);
}

void Brick::StartDestroy()
{
	m_DestroyTween.Start();
}

bool Brick::IsDestroyed()
{
	return m_DestroyTween.IsFinished();
}

