#include <iostream>
#include <Spirit/SpiritParser.h>
#include "AbstractGame/CombinatorialGame.h"
#include "AbstractGame/CombinatorialGameDatabase.h"
#include "CombinatorialGames/RulesetMath.h"
#include "CombinatorialGames/GameDatabase.h"
#include "CombinatorialGames/Cherries/Cherries.h"
#include "CombinatorialGames/Cherries/StackCherries.h"
#include "Push-Shove/Push.h"
#include "Push-Shove/Shove.h"
#include "UI/CombinatorialGameCommandLine.h"

int main() {
	std::cout << "Compiled with C++ version " << __cplusplus << std::endl;
	std::cout << "Written by Xander Lenstra" << std::endl << std::endl;

	char userInput;
	bool running = true;
	CombinatorialGameCommandLine alternatingUI;

	while(running) {
		std::cout << "Do you want to analyze [A]lternating or [S]synchronized Combinatorial Games?" << std::endl;
		std::cin >> userInput;
		userInput = tolower(userInput);
		switch (userInput) {
			case 'a':
				//alternatingUI.main();
				parseStringMain();
				break;
			case 's':
				std::cout << "Not implemented yet. Sorry :(" << std::endl;
				break;
			case 'h':
				std::cout << "Press 'a' for alternating" << std::endl
						  << "Press 's' for synchronized" << std::endl
						  << "Press 'h' for help" << std::endl
						  << "Press 'q' to quit" << std::endl;
				break;
			case 'q':
				running = false;
				break;
			default:
				std::cout << "Unknown option '" << userInput << "'" << std::endl << "Press 'h' for help." << std::endl;
		}
	};

	//CombinatorialGame& gameStar = GET_GAME({ cgDatabase.zeroId }, { cgDatabase.zeroId });


	//StackCherries& cherriesPosition = createStackCherriesPosition("BWWWWWW");
	//std::cout << cherriesPosition.getDisplayString() << std::endl;
	//AbstractId abstractId = getAbstractFormId<StackCherriesPosition>(cherriesPosition);
	//std::cout << ID_TO_GAME(abstractId).getCanonicalForm().getDisplayString() << std::endl;
	//std::cout << cgDatabase << std::endl;

//	Shove& pushPosition = createShovePosition("BB  RR");
//	std::cout << pushPosition.getDisplayString() << std::endl;
//	AbstractId abstractId = getAbstractFormId<PushShovePosition>(pushPosition);
////	Cherries& cherriesPosition = createCherriesPosition("BBWBBW BWWWB WBB WBBWW WWWW");
////	AbstractId abstractId = getAbstractFormId<CherriesPosition>(cherriesPosition);
////	std::cout << cherriesPosition.getDisplayString() << std::endl;
//	std::cout << (int)abstractId << std::endl;
//	std::cout << ID_TO_GAME(abstractId).getCanonicalForm().getDisplayString() << std::endl;
//	std::cout << ID_TO_GAME(abstractId)/*.getCanonicalForm()*/.getDisplayString() << std::endl;
//	std::cout << cgDatabase << std::endl;

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
//	Cherries& leftCherries1 = cherriesDatabase->idToGame(*cherriesPosition.getLeftOptions().begin());
//	leftCherries1.explore();
//	Cherries& leftLeftCherries1 = cherriesDatabase->idToGame(*leftCherries1.getLeftOptions().begin());
//	Cherries& rulesetCherries2 = createCherriesPosition("WW BB");
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
