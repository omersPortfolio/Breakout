#pragma once

#include "GameObject.h"
#include "vec2.h"
#include "AABB.h"

class Tween;

class Brick : public GameObject
{
public:
	Brick();
	~Brick();

	void Draw(sf::RenderWindow& m_Window);

	void Update(float deltaTime);

	// Starts the destroy Tween
	void StartDestroy();
	bool IsDestroyed();

	// Getters
	virtual vec2 GetSize()			{ return vec2(m_BrickShape.getSize().x, m_BrickShape.getSize().y); }
	sf::RectangleShape& GetShape()	{ return m_BrickShape; }
	AABB GetAABB()					{ return aabb; }
	Tween& GetBrickFallDownTween()	{ return m_BrickFallDownTween; }
	Tween& GetBrickStartFallTween() { return m_BrickStartFallTween; }
	float GetBrickStartFallValue()	{ return m_BrickStartFallValue; }
	bool IsMainBrick()				{ return isMainBrick; }
	bool IsVerticalWall()			{ return m_IsVerticalWall; }
	bool IsBreakable()				{ return m_Breakable; }
	
	// Setters
	void SetSize(vec2 newSize);
	void SetSizeX(float newSizeX);
	void SetIsMainBrick(bool bMainBrick) { isMainBrick = bMainBrick;  }
	void SetIsVerticalWall(bool bWall) { m_IsVerticalWall = bWall; }
	void SetColor(sf::Color color) { m_BrickShape.setFillColor(color); }
	void SetOutlineColorAndThickness(sf::Color color, float thickness);
	void SetBreakable(bool bDestructible) { m_Breakable = bDestructible; }


private:

	sf::RectangleShape m_BrickShape;

	// Axis-Aligned Bounding Box component //
	AABB aabb;

	// Brick properties //
	bool isMainBrick;
	bool m_Breakable;
	bool m_IsVerticalWall;

	// Brick Tweens //
	Tween m_DestroyTween;
	float m_DestroySizeX;

	Tween m_BrickFallDownTween;
	float m_BrickFallDownValue;

	Tween m_BrickStartFallTween;
	float m_BrickStartFallValue;
};