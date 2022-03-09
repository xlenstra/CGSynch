#include <iostream>
#include "AbstractGame/CombinatorialGame.h"
#include "AbstractGame/CombinatorialGameDatabase.h"
#include "CombinatorialGames/RulesetMath.h"
#include "CombinatorialGames/GameDatabase.h"
#include "CombinatorialGames/Cherries.h"
#include "CombinatorialGames/StackCherries.h"
#include "CombinatorialGames/Push.h"

int main() {
	std::cout << "Compiled with C++ version " << __cplusplus << std::endl << std::endl;

	//CombinatorialGame& gameStar = GET_GAME({ cgDatabase.zeroId }, { cgDatabase.zeroId });


	//StackCherriesGame& cherriesPosition = createStackCherriesPosition("BWWWWWW");
	//std::cout << cherriesPosition.getDisplayString() << std::endl;
	//AbstractId abstractId = getAbstractFormId<StackCherriesPosition>(cherriesPosition);
	//std::cout << ID_TO_GAME(abstractId).getCanonicalForm().getDisplayString() << std::endl;
	//std::cout << cgDatabase << std::endl;

	Push& pushPosition = createPushPosition("RRB");
	std::cout << pushPosition.getDisplayString() << std::endl;
	AbstractId abstractId = getAbstractFormId<PushShovePosition>(pushPosition);
	std::cout << (int)abstractId << std::endl;
	std::cout << ID_TO_GAME(abstractId)/*.getCanonicalForm()*/.getDisplayString() << std::endl;
	std::cout << ID_TO_GAME(abstractId).getCanonicalForm().getDisplayString() << std::endl;
	std::cout << cgDatabase << std::endl;

    //CombinatorialGame& game2 = cgDatabase.getDyadicRational(513,1024);
	//for (int denominator = 1; denominator < 2048; denominator *= 2) {
	//	std::cout << "Checking denominator " << denominator << "..." << std::endl;
	//	for (int numerator = 1; numerator < denominator; numerator += 2) {
	//		CombinatorialGame game = cgDatabase.getDyadicRational(numerator, denominator);
	//		std::string displayString = game.getDisplayString();
	//		if (displayString != std::to_string(numerator) + "/" + std::to_string(denominator)) {
	//			std::cout << "ERROR! Dyadic rational " << numerator << "/" << denominator << " has " << displayString << "as displayString!" << std::endl;
	//		}
	//	}
	//}

//	CombinatorialGame& game2 = CREATE_GAME({negativeIntegers[1]},{positiveIntegers[1]});



//	cherriesPosition.explore();
//	CherriesGame& leftCherries1 = cherriesDatabase->idToGame(*cherriesPosition.getLeftOptions().begin());
//	leftCherries1.explore();
//	CherriesGame& leftLeftCherries1 = cherriesDatabase->idToGame(*leftCherries1.getLeftOptions().begin());
//	CherriesGame& rulesetCherries2 = createCherriesPosition("WW BB");
//	std::cout << *cherriesDatabase << std::endl;
//	std::cout << abstractId << std::endl;

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
//	std::cout << game2.getBirthday() << std::endl;
//	std::cout << game2.getDisplayString() << std::endl;
//    std::cout << game2.getCanonicalForm().getDisplayString() << std::endl;
//    std::cout << cgDatabase << std::endl;

	return 0;
}
