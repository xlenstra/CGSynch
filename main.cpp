#include <iostream>
#include <chrono>

#include <Spirit/SpiritParser.h>
#include <Hackenbush/NormalGraph.h>
#include <SynchronousGame/GurobiSolver.h>
#include <SynchronousGame/SynchronizedGameDatabase.h>
#include <SynchronousGame/SynchronousGame.h>

#include "CombinatorialGame/CombinatorialGame.h"
#include "CombinatorialGame/CombinatorialGameDatabase.h"
#include "Math/RulesetMath.h"
#include "CombinatorialGames/GameDatabase.h"
#include "CombinatorialGames/Cherries/Cherries.h"
#include "CombinatorialGames/Cherries/StackCherries.h"
#include "Push-Shove/Push.h"
#include "Push-Shove/Shove.h"
#include "Hackenbush/HackenbushUtil.h"
#include "Hackenbush/Hackenbush.h"

int main() {
	std::cout << "Compiled with C++ version " << __cplusplus << std::endl;
	std::cout << "Written by Xander Lenstra" << std::endl << std::endl;

	char userInput;
	bool running = true;

	while(running) {
		std::cout << "Do you want to analyze [A]lternating or [S]synchronized Combinatorial Games?" << std::endl;
		std::cin >> userInput;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		userInput = tolower(userInput);
		switch (userInput) {
			case 'a':
				parseStringMain();
				break;
			case 's':
				std::cout << "Not implemented yet. Sorry :(" << std::endl;
				break;
			case 'g': {
				std::cout << "Graph test !" << std::endl;
				NormalGraph graph = graphFromMatrixString(2, charsToPieceColours("_RR_"));
				std::cout << graph.getDisplayString() << std::endl;
				break;
			}
			case 'h':
				std::cout << "Press 'a' for alternating" << std::endl
						  << "Press 's' for synchronized" << std::endl
						  << "Press 'h' for help" << std::endl
						  << "Press 'q' to quit" << std::endl;
				break;
			case 'q':
				running = false;
				break;
			case 't': {
				std::cout << "Starting test!" << std::endl;
				auto startTime = std::chrono::steady_clock::now();
				std::vector<std::vector<double>> matrix = {{0,1},{1,2}};
				auto game = SGDatabase::getInstance().idToGame(0);
				std::cout << "Value = " << game.getValue() << std::endl;
				auto endTime = std::chrono::steady_clock::now();
				auto takenTime = endTime - startTime;
				std::cout << "Caculation time: " << std::chrono::duration_cast<std::chrono::microseconds>(takenTime).count() << " microseconds" << std::endl;
				running = false;
				break;
			}
			default:
				std::cout << "Unknown option '" << userInput << "'" << std::endl << "Press 'h' for help." << std::endl;
		}
	};

	std::cout << GameDatabase<HackenbushPosition, Hackenbush>::getInstance()->size() << std::endl;
//	for (GameId gameId = 0; gameId < 15 && gameId < GameDatabase<HackenbushPosition, Hackenbush>::getInstance()->size(); ++gameId) {
//		auto game = GameDatabase<HackenbushPosition, Hackenbush>::getInstance()->idToGame(gameId);
//		std::cout << gameId << ":" << game.getDisplayString() << std::endl << std::hash<NormalGraph>()(game.getAnyTransposition())
//		<< std::endl;
//	}

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
