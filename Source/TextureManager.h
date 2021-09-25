#pragma once

class TextureManager
{
public:
	TextureManager();

	void LoadTextures();

	const sf::Texture& GetBallTexture() { return m_BallTexture; }

private:
	sf::Texture m_BallTexture;
};