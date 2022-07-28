#include <iostream>
#include <chrono>

#include <AlternatingUI/SpiritParser.h>
#include <SynchedUI/SpiritParser.h>
#include <boost/algorithm/string/predicate.hpp>
#include "SynchronousGame/SynchronizedGameDatabase.h"
#include "Hackenbush/Hackenbush.h"

int main() {
//	std::cout << "Compiled with C++ version " << __cplusplus << std::endl;
//	std::cout << "Gurobi license key location: " << std::string(tmp ?: "Key not found!") << std::endl;
	std::cout << "-------------------------------- CGSYNCH --------------------------------" << std::endl
	          << std::endl
			  << "A program for analyzing Combinatorial and Synchronized Games." << std::endl
			  << "Written by Xander Lenstra for his Bachelor Thesis at Leiden University.  " << std::endl
			  << "Supervised by Walter Kosters (LIACS) and Mark van den Bergh (MI)." << std::endl
			  << std::endl
			  << std::endl;

	const char* tmp = std::getenv("GRB_LICENSE_FILE");
	if (!tmp) {
		std::cout << "No Gurobi License Key was found. Analyzing Synchronized Positions will not be possible." << std::endl << std::endl;
	}

	std::string userInput;

	while(true) {
		std::cout << "Do you want to analyze Combinatorial or Synchronized Games?" << std::endl;
		std::getline(std::cin, userInput);
		if (boost::iequals("c", userInput) || boost::iequals(userInput, "Combinatorial")) {
			std::cout << "Analyzing Combinatorial Games" << std::endl;
			alternatingGameUI();
		} else if (boost::iequals("s", userInput) || boost::iequals(userInput, "Synchronized") || boost::iequals(userInput, "Synched")) {
			std::cout << "Analyzing Synchronized Games" << std::endl;
			synchedGameUI();
		} else if (boost::iequals("h", userInput) || boost::iequals(userInput, "Help")) {
			std::cout << "Type 'c' or 'Combinatorial' to enter and analyze combinatorial games" << std::endl
			          << "Type 'c' or 'Synched' to enter and analyze synchronized games" << std::endl
			          << "Type 'h' or 'Help' to show this screen" << std::endl
			          << "Type 'q' or 'quit' to quit" << std::endl;
		} else if (boost::iequals("q", userInput) || boost::iequals(userInput, "Quit")) {
			break;
		} else {
			std::cout << "Unknown option '" << userInput << "'" << std::endl << "Type 'help' for help." << std::endl;
		}
	}

	return 0;
}
