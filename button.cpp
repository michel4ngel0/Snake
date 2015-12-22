#include "button.h"
#include "menu.h"
#include <SFML/Graphics.hpp>

Button::Button(sf::Vector2f position, sf::Texture& texture, menu_message message) {
	setPosition(position);
	setTexture(texture);
	message_ = message;
}

void Button::register_click(const sf::Vector2f& click,
							std::queue<menu_message>& messages) {
	const sf::Vector2f& position = getPosition();
	const sf::Vector2u& size = getTexture()->getSize();
	if (click.x >= position.x && click.x <= position.x + size.x &&
		click.y >= position.y && click.y <= position.y + size.y) {
		
		messages.push(message_);
	}
}