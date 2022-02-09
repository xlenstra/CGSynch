#include <iostream>
#include "Game/CombinatorialGame.h"
#include "Game/CombinatorialGameDatabase.h"

int main() {
    CombinatorialGame& game = cgDatabase.createGame({0}, {cgDatabase.createGameId({0},{0})});
	std::cout << game.getWinner() << std::endl;
    std::cout << game.getDisplayString() << std::endl;
    std::cout << game.getBirthday() << std::endl;
	cgDatabase;
	return 0;
}
