#include "menu.h"
#include "button.h"

Menu::Menu() {
	texture_background_.loadFromFile("graphics/menu.png");
	texture_help_.loadFromFile("graphics/menu_help.png");

	background_.setTexture(texture_background_);
	background_.setPosition(0.f, 0.f);
	help_.setTexture(texture_help_);
	help_.setPosition(100.f, 180.f);
	
	textures_buttons_.resize(4);
	textures_buttons_[0].loadFromFile("graphics/newgame.bmp");
	textures_buttons_[1].loadFromFile("graphics/help.bmp");
	textures_buttons_[2].loadFromFile("graphics/exit.bmp");
	textures_buttons_[3].loadFromFile("graphics/back.bmp");

	buttons_.push_back(Button(sf::Vector2f(300, 250),
		textures_buttons_[0],
		menu_message::new_game));
	buttons_.push_back(Button(sf::Vector2f(300, 350),
		textures_buttons_[1],
		menu_message::help));
	buttons_.push_back(Button(sf::Vector2f(300, 450),
		textures_buttons_[2],
		menu_message::exit));
	buttons_.push_back(Button(sf::Vector2f(200, 450),
		textures_buttons_[3],
		menu_message::back));
	
	state_ = menu_state::main;
}

Menu::~Menu() {
	buttons_.clear();
	textures_buttons_.clear();
}

game_state Menu::step() {
	if (!message_buffer_.empty()) {
		menu_message message = message_buffer_.front();
		message_buffer_.pop();

		switch (message) {
		case menu_message::new_game:
			while (!message_buffer_.empty())
				message_buffer_.pop();
			return game_state::game;
		case menu_message::help:
			state_ = menu_state::help;
			break;
		case menu_message::exit:
			return game_state::closing;
		case menu_message::back:
			state_ = menu_state::main;
			break;
		}
	}

	return game_state::menu;
}

void Menu::draw(sf::RenderWindow& window) const {
	window.draw(background_);
	if (state_ == menu_state::main) {
		window.draw(buttons_[0]);
		window.draw(buttons_[1]);
		window.draw(buttons_[2]);
	}
	else {
		window.draw(help_);
		window.draw(buttons_[3]);
	}
}

void Menu::register_click(const sf::Vector2f& click) {
	if (state_ == menu_state::main) {
		buttons_[0].register_click(click, message_buffer_);
		buttons_[1].register_click(click, message_buffer_);
		buttons_[2].register_click(click, message_buffer_);
	}
	else {
		buttons_[3].register_click(click, message_buffer_);
	}
}