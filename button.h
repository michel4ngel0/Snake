#pragma once

#include "types.h"
#include <SFML/Graphics.hpp>
#include <queue>

class Button : public sf::Sprite {
private:
	menu_message message_;

public:
	Button(sf::Vector2f, sf::Texture&, menu_message);

	void register_click(const sf::Vector2f&, std::queue<menu_message>&);
};
