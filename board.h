#pragma once

#include "types.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <random>

class Board {
private:
	int points_;
	direction next_step_direction_;
	direction forbidden_direction_;
	std::list<sf::Sprite> fruits_;
	std::list<sf::Sprite> walls_;
	std::vector<sf::Sprite> snake_;
	std::vector<sf::Texture> textures_fruits_;
	sf::Texture texture_background_;
	sf::Sprite background_;
	sf::Texture texture_snake_body_;
	sf::Texture texture_snake_head_;
	sf::Texture texture_wall_;
	std::mt19937 random_;
	double snake_speed_;

	const size_t max_fruit_count_ = 5;
	const double fruit_spawn_speed_ = 1.5;
	const int points_per_fruit_ = 1;

public:
	Board();
	~Board();

	game_state step(const double);
	void draw(sf::RenderWindow&);
	void register_input(direction);
	int get_points() const;
};