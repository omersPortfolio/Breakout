#pragma once

class Button
{
public:
	Button() {}
	Button(std::string buttonText, vec2 position, sf::Font& font, unsigned int fontSize, sf::Color textColor, vec2 buttonOverlaySize = vec2(400, 150), sf::Color buttonColor = sf::Color::Transparent);
	~Button();

	void Draw(sf::RenderTarget& window);

	void SetColor(sf::Color color);

	bool IsClicked(sf::RenderWindow& window, sf::Event& event);

private:
	sf::Text m_ButtonText;
	sf::Color m_ButtonColor;

	sf::RectangleShape m_ButtonOverlay;
};