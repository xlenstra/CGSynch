//
// Created by s1935534 on 09/03/2022.
//

#include "Push.h"

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<PushShovePosition, Push>> GameDatabase<PushShovePosition, Push>::instance = nullptr;
template<> std::vector<std::shared_ptr<Push>> GameDatabase<PushShovePosition, Push>::database = {};
template<> std::unordered_map<PushShovePosition, GameId> GameDatabase<PushShovePosition, Push>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<GameDatabase<PushShovePosition, Push>> cherriesDatabase = GameDatabase<PushShovePosition, Push>::getInstance();


Push::Push(PushShovePosition position) : position(position) {}

std::string Push::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& square : position) {
		switch (square) {
			case StoneColour::RED:
				displayString += "R";
				break;
			case StoneColour::BLUE:
				displayString += "B";
				break;
			case StoneColour::NONE:
				displayString += " ";
				break;
		}
		displayString += " ";
	}
	displayString += "\b";
	return displayString;
}

PushShovePosition Push::getAnyTransposition() const {
	return position;
}

std::unordered_set<PushShovePosition> Push::getTranspositions() const {
	return { position };
}

void Push::explore() {
	//size_t
}

