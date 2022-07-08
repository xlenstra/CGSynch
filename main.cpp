#include <iostream>
#include <chrono>

#include <AlternatingUI/SpiritParser.h>
#include <SynchedUI/SpiritParser.h>
#include "SynchronousGame/SynchronizedGameDatabase.h"
#include "Hackenbush/Hackenbush.h"

int main() {
//	std::cout << "Compiled with C++ version " << __cplusplus << std::endl;
//	std::cout << "Gurobi license key location: " << std::string(tmp ?: "Key not found!") << std::endl;
	std::cout << "-------------------------------- CGSYNCH --------------------------------" << std::endl
	          << std::endl
			  << "A program for analyzing Alternating and Synchronized Combinatorial games." << std::endl
			  << "Written by Xander Lenstra for his Bachelor Thesis at Leiden University.  " << std::endl
			  << "Supervised by Walter Kosters (LIACS) and Mark van den Bergh (MI)." << std::endl
			  << std::endl
			  << std::endl;

	const char* tmp = std::getenv("GRB_LICENSE_FILE");
	if (!tmp) {
		std::cout << "No Gurobi License Key was found. Analyzing Synchronized Positions will not be possible." << std::endl << std::endl;
	}

	char userInput;
	bool running = true;

	while(running) {
		std::cout << "Do you want to analyze [A]lternating or [S]synchronized Combinatorial Games?" << std::endl;
		std::cin >> userInput;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		userInput = tolower(userInput);
		switch (userInput) {
			case 'a':
				alternatingGameUI();
				break;
			case 's':
				synchedGameUI();
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


	return 0;
}
