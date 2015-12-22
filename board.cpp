#include "board.h"

#include <cmath>

constexpr float chunk_size = 20.f;

Board::Board() {
	random_.seed((unsigned int)this);

	textures_fruits_.resize(5);
	textures_fruits_[0].loadFromFile("graphics/fruit1.png");
	textures_fruits_[1].loadFromFile("graphics/fruit2.png");
	textures_fruits_[2].loadFromFile("graphics/fruit3.png");
	textures_fruits_[3].loadFromFile("graphics/fruit4.png");
	textures_fruits_[4].loadFromFile("graphics/fruit5.png");

	texture_background_.loadFromFile("graphics/background.png");
	background_.setTexture(texture_background_);

	texture_snake_body_.loadFromFile("graphics/snake_body.png");
	texture_snake_head_.loadFromFile("graphics/snake_head.png");

	texture_wall_.loadFromFile("graphics/wall.png");
	sf::Sprite wall(texture_wall_);
	for (int i = 0; i < 800 / (int)chunk_size; ++i) {
		wall.setPosition(sf::Vector2f(i * chunk_size, 0));
		walls_.push_back(wall);
		wall.setPosition(sf::Vector2f(i * chunk_size, 600 - chunk_size));
		walls_.push_back(wall);
	}
	for (int i = 0; i < 600 / (int)chunk_size; ++i) {
		wall.setPosition(sf::Vector2f(0, i * chunk_size));
		walls_.push_back(wall);
		wall.setPosition(sf::Vector2f(800 - chunk_size, i * chunk_size));
		walls_.push_back(wall);
	}

	sf::Sprite snake_segment(texture_snake_head_);
	snake_segment.setPosition(sf::Vector2f(200.f, 280.f));
	snake_.push_back(snake_segment);
	snake_segment.setTexture(texture_snake_body_);
	snake_segment.setPosition(sf::Vector2f(180.f, 280.f));
	snake_.push_back(snake_segment);
	snake_segment.setPosition(sf::Vector2f(160.f, 280.f));
	snake_.push_back(snake_segment);

	points_ = 0;
	next_step_direction_ = direction::right;
	forbidden_direction_ = direction::left;
}

Board::~Board() {
}

game_state Board::step(const double dt) {
	static double movement_counter = 0.0;
	static double spawn_counter = 0.0;
	movement_counter += dt;
	spawn_counter += dt;

	snake_speed_ = 15.0 + 5.0 * log(snake_.size());

	if (movement_counter * snake_speed_ >= 1.0) {
		movement_counter -= 1.0 / snake_speed_;

		sf::Vector2f next_field = snake_[0].getPosition();
		switch (next_step_direction_) {
		case direction::up:
			next_field.y -= chunk_size;
			forbidden_direction_ = direction::down;
			break;
		case direction::down:
			next_field.y += chunk_size;
			forbidden_direction_ = direction::up;
			break;
		case direction::left:
			next_field.x -= chunk_size;
			forbidden_direction_ = direction::right;
			break;
		case direction::right:
			next_field.x += chunk_size;
			forbidden_direction_ = direction::left;
			break;
		}

		for (auto& wall : walls_)
			if (wall.getPosition() == next_field)
				return game_state::menu;

		bool eating = false;
		for (auto it = fruits_.begin(); it != fruits_.end(); ++it)
			if (it->getPosition() == next_field) {
				eating = true;
				fruits_.erase(it);
				snake_.push_back(sf::Sprite(texture_snake_body_));
				points_ += points_per_fruit_;
				break;
			}

		for (int i = snake_.size() - 1; i > 0; --i)
			if (snake_[i].getPosition() == next_field)
				return game_state::menu;

		for (int i = snake_.size() - 1; i > 0; --i)
			snake_[i].setPosition(snake_[i - 1].getPosition());
		snake_[0].setPosition(next_field);
	}

	if (spawn_counter * fruit_spawn_speed_ >= 1.0) {
		spawn_counter -= 1.0 / fruit_spawn_speed_;

		if (fruits_.size() < max_fruit_count_) {
			float x = (random_() % (800 / (int)chunk_size)) * chunk_size;
			float y = (random_() % (600 / (int)chunk_size)) * chunk_size;

			sf::Sprite new_fruit;
			new_fruit.setTexture(textures_fruits_[random_() % 5]);
			new_fruit.setPosition(sf::Vector2f(x, y));

			fruits_.push_back(new_fruit);
		}
	}

	return game_state::game;
}

void Board::draw(sf::RenderWindow& window) {
	window.draw(background_);

	for (auto& fruit : fruits_)
		window.draw(fruit);

	for (auto& segment : snake_)
		window.draw(segment);

	for (auto& wall : walls_)
		window.draw(wall);
}

void Board::register_input(direction input) {
	if (input != forbidden_direction_)
		next_step_direction_ = input;
}

int Board::get_points() const {
	return points_;
}