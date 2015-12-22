#pragma once

enum class direction : char {
	up, down, left, right
};

enum class game_state : char {
	not_started, menu, game, closing
};

enum class menu_state : char {
	main, help
};

enum class menu_message : char {
	new_game, help, exit, back
};