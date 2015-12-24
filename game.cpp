#include "game.h"
#include "menu.h"
#include "board.h"
#include <cassert>
#include <chrono>
#include <iostream>

Game* Game::singleton = nullptr;

Game* Game::get_game_object() {
	if (Game::singleton == nullptr)
		Game::singleton = new Game();

	return Game::singleton;
}

Game::Game() {
	state_ = game_state::not_started;
	menu_ = nullptr;
	board_ = nullptr;
}

int Game::run() {
	assert(state_ == game_state::not_started);

	auto previous_time =
		std::chrono::high_resolution_clock::now();
	decltype(previous_time) time;
	double time_counter = 0.0;
	unsigned long frame_counter = 0;

	Menu* menu = nullptr;
	Board* board = nullptr;

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Snake");
	
	while (window.isOpen()) {
		time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> dt =
			std::chrono::duration_cast<std::chrono::duration<double>>(time - previous_time);
		previous_time = time;

		frame_counter += 1;
		time_counter += dt.count();
		if (time_counter >= 1.0) {
			std::cerr << "FPS: " << frame_counter << std::endl;
			frame_counter = 0;
			time_counter -= 1.0;
		}
		
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				state_ = game_state::closing;
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					state_ = game_state::closing;
				}
				else if (state_ == game_state::game) {
					if (event.key.code == sf::Keyboard::Up)
						board->register_input(direction::up);
					if (event.key.code == sf::Keyboard::Down)
						board->register_input(direction::down);
					if (event.key.code == sf::Keyboard::Left)
						board->register_input(direction::left);
					if (event.key.code == sf::Keyboard::Right)
						board->register_input(direction::right);
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (state_ == game_state::menu) {
						sf::Vector2f click((float)event.mouseButton.x, (float)event.mouseButton.y);
						menu->register_click(click);
					}
				}
			}
		}

		window.clear(sf::Color(255, 255, 255));

		switch (state_) {
		case game_state::not_started:
			state_ = game_state::menu;
			menu = new Menu();
			break;
		case game_state::menu:
			state_ = menu->step();
			menu->draw(window);
			if (state_ == game_state::game)
				board = new Board();
			break;
		case game_state::game:
			state_ = board->step(dt.count());
			board->draw(window);
			if (state_ == game_state::menu) {
				std::cerr << "You Lose! Score: " << board->get_points() << std::endl;
				delete board;
				board = nullptr;
			}
			break;
		case game_state::closing:
			delete menu;
			delete board;
			menu = nullptr;
			board = nullptr;
			window.close();
		}

		if (window.isOpen())
			window.display();
	}

	return 0;
}