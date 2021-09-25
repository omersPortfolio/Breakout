#include "GamePCH.h"

#include "TextureManager.h"

TextureManager::TextureManager()
{
	LoadTextures();
}

void TextureManager::LoadTextures()
{
	// Decided to just give the ball a texture.
	if (!m_BallTexture.loadFromFile("../Textures/Water.png"))
		std::cout << "Error loading Water.png" << std::endl;

	m_BallTexture.setSmooth(true);
}
