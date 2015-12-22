#pragma once

#include "types.h"
#include <SFML/Graphics.hpp>

class Menu;
class Board;

class Game {
private:
	static Game* singleton;

	game_state state_;
	Menu* menu_;
	Board* board_;

	Game();
	Game& operator=(const Game&) = delete;

public:
	static Game* get_game_object();
	~Game();

	int run();
};
