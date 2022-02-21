#include <iostream>
#include "AbstractGame/CombinatorialGame.h"
#include "AbstractGame/CombinatorialGameDatabase.h"
#include "CombinatorialRuleset/RulesetMath.h"
#include "CombinatorialRuleset/RulesetDatabase.h"
#include "CombinatorialRuleset/RulesetCherries.h"

int main() {
	std::cout << "Compiled with C++ version " << __cplusplus << std::endl << std::endl;

	std::vector<GameId> positiveIntegers;
	positiveIntegers.push_back(cgDatabase.zeroId);
	positiveIntegers.resize(21);
	for (int i = 1; i < 21; ++i) {
		positiveIntegers.at(i) = cgDatabase.getGameId({positiveIntegers[i - 1]}, {});
	}
	std::vector<GameId> negativeIntegers;
	negativeIntegers.push_back(cgDatabase.zeroId);
	negativeIntegers.resize(21);
	for (int i = 1; i < 21; ++i) {
		negativeIntegers.at(i) = cgDatabase.getGameId({}, {negativeIntegers.at(i - 1)});
	}
	CombinatorialGame& gameStar = GET_GAME({ cgDatabase.zeroId }, { cgDatabase.zeroId });
	std::cout << cgDatabase << std::endl;

//	CombinatorialGame& game2 = CREATE_GAME({negativeIntegers[1]},{positiveIntegers[1]});


//	RulesetCherries& rulesetCherries1 = createCherriesPosition("BB WW");
//	rulesetCherries1.explore();
//	RulesetCherries& leftCherries1 = cherriesDatabase->getGame(*rulesetCherries1.getLeftOptions().begin());
//	leftCherries1.explore();
//	RulesetCherries& leftLeftCherries1 = cherriesDatabase->getGame(*leftCherries1.getLeftOptions().begin());
//	RulesetCherries& rulesetCherries2 = createCherriesPosition("WW BB");
//	std::cout << *cherriesDatabase << std::endl;
//	std::cout << leftLeftCherries1.getDisplayString() << std::endl;
//	GameId abstractId = getAbstractFormId<CherriesPosition>(rulesetCherries1);
//	std::cout << abstractId << std::endl;
//	std::cout << GET_GAME(abstractId).getCanonicalForm().getDisplayString() << std::endl;
//	std::cout << cgDatabase << std::endl;

//	RulesetDatabase<std::string, MathRuleset> positionDatabase;


//	CombinatorialGame& game = CREATE_GAME(
//		{positiveIntegers[20]},
//		{negativeIntegers[10]}
//	);
//
//	CombinatorialGame& game2 = cgDatabase.createGame(
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
