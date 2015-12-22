#include "game.h"

int main() {
	Game* game_object = Game::get_game_object();
	return game_object->run();
}
