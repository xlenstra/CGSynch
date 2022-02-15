#include <iostream>
#include "Game/CombinatorialGame.h"
#include "Game/CombinatorialGameDatabase.h"

int main() {
	std::cout << "Compiled with C++ version " << __cplusplus << std::endl << std::endl;

	std::vector<GameId> positiveIntegers;
	positiveIntegers.push_back(cgDatabase.zeroId);
	positiveIntegers.resize(21);
	for (int i = 1; i < 21; ++i) {
		positiveIntegers.at(i) = cgDatabase.createGameId({positiveIntegers[i-1]},{});
	}
	std::vector<GameId> negativeIntegers;
	negativeIntegers.push_back(cgDatabase.zeroId);
	negativeIntegers.resize(21);
	for (int i = 1; i < 21; ++i) {
		negativeIntegers.at(i) = cgDatabase.createGameId({}, {negativeIntegers.at(i-1)});
	}
	CombinatorialGame& gameStar = CREATE_GAME({cgDatabase.zeroId}, {cgDatabase.zeroId});
	std::cout << cgDatabase << std::endl;


	CombinatorialGame& game = CREATE_GAME(
		{positiveIntegers[20]},
		{negativeIntegers[10]}
	);

	CombinatorialGame& game2 = cgDatabase.createGame(
		{game.getId(), positiveIntegers[2]},
		{positiveIntegers[1]}
	);

//	CombinatorialGame& game3 = cgDatabase.createGame(
//		{negativeIntegers[5]},
//		{negativeIntegers[2]}
//	);



//    CombinatorialGame& game = cgDatabase.createGame(
//        {
////	        cgDatabase.zeroId,
////	        cgDatabase.createGameId(
////				{cgDatabase.zeroId},
////				{}
////	        ),
//			cgDatabase.createGameId(
//				{gameStar.getId()},
//				{gameStar.getId()}
//			)
//		},
//        {}
//    );

//	std::cout << game.getWinner() << std::endl;
//    std::cout << game.getDisplayString() << std::endl;
//    std::cout << game.getBirthday() << std::endl;
//    std::cout << cgDatabase << std::endl;
//    CombinatorialGame& game2 = game.getCanonicalForm();
    std::cout << game2.getWinner() << std::endl;
    std::cout << game2.getDisplayString() << std::endl;
    std::cout << game2.getBirthday() << std::endl;
	std::cout << game2.getCanonicalForm().getDisplayString() << std::endl;
    std::cout << cgDatabase << std::endl;
//	CombinatorialGame& game1 = CREATE_GAME({cgDatabase.zeroId}, {});
//	CombinatorialGame& gameM1 = -game1;
//	std::cout << (cgDatabase.getZero() < game1) << std::endl;
//	std::cout << (cgDatabase.getZero() > game1) << std::endl;
//	std::cout << (cgDatabase.getZero() <=> game1 == 0) << std::endl;
//	std::cout << (cgDatabase.getZero() <=> game1 == std::partial_ordering::unordered) << std::endl;
//	std::cout << (cgDatabase.getZero() - game1).getDisplayString() << std::endl;
//	std::cout << (cgDatabase.getZero() - game1).getWinner() << std::endl;


//    CombinatorialGame& game3 = game + game2;
//    std::cout << game3.getWinner() << std::endl;
//    std::cout << game3.getDisplayString() << std::endl;
//    std::cout << game3.getBirthday() << std::endl;
//	std::cout << game3.getCanonicalForm().getDisplayString() << std::endl;
//	std::cout << cgDatabase << std::endl;
//	CombinatorialGame& game4 = game - game;
//	std::cout << "win:" << game4.getWinner() << std::endl;
//	std::cout << "dsp:" << game4.getDisplayString() << std::endl;
//	std::cout << "bd:" << game4.getBirthday() << std::endl;
//	std::cout << "id:" << game4.getId() << std::endl;
//	std::cout << "id:" << cgDatabase.getZero().getId() << std::endl;
//	std::cout << cgDatabase << std::endl;
//	std::cout << "ss:" << (game3 <=> cgDatabase.getZero() == 0) << std::endl;
//	std::cout << "eq:" << (game4 == cgDatabase.getZero()) << std::endl;

	return 0;
}
