#include "GamePCH.h"

#include "Button.h"

Button::Button(std::string buttonText, vec2 position, sf::Font& font, unsigned int fontSize, sf::Color textColor, vec2 buttonOverlaySize /*= vec2(400, 150)*/, sf::Color buttonColor /*= sf::Color::Transparent*/)
{
	m_ButtonOverlay.setFillColor(buttonColor);
	m_ButtonOverlay.setOutlineColor(sf::Color::Black);
	buttonColor == sf::Color::Transparent ? m_ButtonOverlay.setOutlineThickness(0.0f) : m_ButtonOverlay.setOutlineThickness(5.0f);
	m_ButtonColor = buttonColor;
	m_ButtonOverlay.setSize(sf::Vector2f(buttonOverlaySize.x, buttonOverlaySize.y));
	m_ButtonOverlay.setPosition(sf::Vector2f(position.x, position.y));
	m_ButtonOverlay.setOrigin(m_ButtonOverlay.getGlobalBounds().width / 2.0f, m_ButtonOverlay.getGlobalBounds().height / 2.0f);

	m_ButtonText.setString(buttonText);
	m_ButtonText.setFont(font);
	m_ButtonText.setCharacterSize(fontSize);
	m_ButtonText.setFillColor(textColor);
	m_ButtonText.setOutlineThickness(5);
	m_ButtonText.setOutlineColor(sf::Color::Black);
	m_ButtonText.setStyle(sf::Text::Bold);
	m_ButtonText.setPosition(m_ButtonOverlay.getPosition().x, m_ButtonOverlay.getPosition().y - 20);
	m_ButtonText.setOrigin(m_ButtonText.getGlobalBounds().width / 2.0f, m_ButtonText.getGlobalBounds().height / 2.0f);
}

Button::~Button()
{

}

void Button::Draw(sf::RenderTarget& window)
{
	window.draw(m_ButtonText);
	window.draw(m_ButtonOverlay);
}

void Button::SetColor(sf::Color color)
{
	m_ButtonColor = color;
}

bool Button::IsClicked(sf::RenderWindow& window, sf::Event& event)
{
	vec2 mousePosition = vec2((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);
	if (m_ButtonOverlay.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
	{
		m_ButtonOverlay.setFillColor(sf::Color(20, 20, 20, 125));
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			return true;
		}
	}
	else
	{
		m_ButtonOverlay.setFillColor(m_ButtonColor);
		return false;
	}
	return false;
}
