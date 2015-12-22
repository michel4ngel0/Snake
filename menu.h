#pragma once

#include "types.h"
#include <SFML\Graphics.hpp>
#include <queue>
#include <vector>

class Button;

class Menu {
private:
	menu_state state_;
	std::queue<menu_message> message_buffer_;
	sf::Texture texture_background_;
	sf::Texture texture_help_;
	std::vector<sf::Texture> textures_buttons_;
	std::vector<Button> buttons_;
	sf::Sprite background_;
	sf::Sprite help_;

public:
	Menu();
	~Menu();

	game_state step();
	void draw(sf::RenderWindow&) const;
	void register_click(const sf::Vector2f&);
};
