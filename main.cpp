#include <iostream>
#include "AbstractGame/CombinatorialGame.h"
#include "AbstractGame/CombinatorialGameDatabase.h"
#include "CombinatorialGames/RulesetMath.h"
#include "CombinatorialGames/GameDatabase.h"
#include "CombinatorialGames/CherriesGame.h"

int main() {
	std::cout << "Compiled with C++ version " << __cplusplus << std::endl << std::endl;

	std::vector<AbstractId> positiveIntegers;
	positiveIntegers.push_back(cgDatabase.zeroId);
	positiveIntegers.resize(21);
	for (int i = 1; i < 21; ++i) {
		positiveIntegers.at(i) = cgDatabase.getGameId({positiveIntegers[i - 1]}, {});
	}
	std::vector<AbstractId> negativeIntegers;
	negativeIntegers.push_back(cgDatabase.zeroId);
	negativeIntegers.resize(21);
	for (int i = 1; i < 21; ++i) {
		negativeIntegers.at(i) = cgDatabase.getGameId({}, {negativeIntegers.at(i - 1)});
	}
	CombinatorialGame& gameStar = GET_GAME({ cgDatabase.zeroId }, { cgDatabase.zeroId });
	std::cout << cgDatabase << std::endl;

//	CombinatorialGame& game2 = CREATE_GAME({negativeIntegers[1]},{positiveIntegers[1]});


//	CherriesGame& rulesetCherries1 = createCherriesPosition("BB WW");
//	rulesetCherries1.explore();
//	CherriesGame& leftCherries1 = cherriesDatabase->idToGame(*rulesetCherries1.getLeftOptions().begin());
//	leftCherries1.explore();
//	CherriesGame& leftLeftCherries1 = cherriesDatabase->idToGame(*leftCherries1.getLeftOptions().begin());
//	CherriesGame& rulesetCherries2 = createCherriesPosition("WW BB");
//	std::cout << *cherriesDatabase << std::endl;
//	std::cout << leftLeftCherries1.getDisplayString() << std::endl;
//	AbstractId abstractId = getAbstractFormId<CherriesPosition>(rulesetCherries1);
//	std::cout << abstractId << std::endl;
//	std::cout << GET_GAME(abstractId).getCanonicalForm().getDisplayString() << std::endl;
//	std::cout << cgDatabase << std::endl;

//	RulesetDatabase<std::string, MathRuleset> positionDatabase;


//	CombinatorialGame& game = CREATE_GAME(
//		{positiveIntegers[20]},
//		{negativeIntegers[10]}
//	);
//
//	CombinatorialGame& game2 = cgDatabase.getOrInsertGame(
//		{game.getId(), positiveIntegers[2]},
//		{positiveIntegers[1]}
//	);
//
//
//
//    std::cout << game2.getWinner() << std::endl;
//    std::cout << game2.getDisplayString() << std::endl;
//    std::cout << game2.getBirthday() << std::endl;
//	std::cout << game2.getCanonicalForm().getDisplayString() << std::endl;
//    std::cout << cgDatabase << std::endl;

	return 0;
}
