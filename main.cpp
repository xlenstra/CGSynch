#include <iostream>
#include "Game/CombinatorialGame.h"
#include "Game/CombinatorialGameDatabase.h"

int main() {
    CombinatorialGame& game = GAME(
        { cgDatabase.zeroId },
        { }
    );
	std::cout << game.getWinner() << std::endl;
    std::cout << game.getDisplayString() << std::endl;
    std::cout << game.getBirthday() << std::endl;
    std::cout << cgDatabase << std::endl;
    CombinatorialGame& game2 = GET_GAME(-game);
    std::cout << game2.getWinner() << std::endl;
    std::cout << game2.getDisplayString() << std::endl;
    std::cout << game2.getBirthday() << std::endl;
    std::cout << cgDatabase << std::endl;
    CombinatorialGame& game3 = GET_GAME(game + game2);
    std::cout << game3.getWinner() << std::endl;
    std::cout << game3.getDisplayString() << std::endl;
    std::cout << game3.getBirthday() << std::endl;
    std::cout << cgDatabase << std::endl;
	return 0;
}
