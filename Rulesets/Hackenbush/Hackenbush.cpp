//
// Created by ardour on 25-03-22.
//

#include <boost/spirit/home/x3.hpp>
#include "Hackenbush.h"

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<HackenbushPosition, Hackenbush>> GameDatabase<HackenbushPosition, Hackenbush>::instance = nullptr;
template<> std::vector<std::shared_ptr<Hackenbush>> GameDatabase<HackenbushPosition, Hackenbush>::database = {};
template<> std::unordered_map<HackenbushPosition, GameId> GameDatabase<HackenbushPosition, Hackenbush>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
const std::shared_ptr<GameDatabase<HackenbushPosition, Hackenbush>> hackenbushDatabase = GameDatabase<HackenbushPosition, Hackenbush>::getInstance();

Hackenbush::Hackenbush(const HackenbushPosition& position) : position(position) {}

std::string Hackenbush::getDisplayString() {
	return position.getDisplayString();
}

NormalGraph Hackenbush::getAnyTransposition() const {
	return position;
}

void Hackenbush::exploreAlternating() {
	for (const auto& [nodeFrom, nodeTo, colour] : position.getAllEdges()) {
		position.removeEdge(nodeFrom, nodeTo);
		Hackenbush newGame = Hackenbush(position.getSubGraphConnectedToGround(nodeFrom, nodeTo));
		if (colour == PieceColour::BLUE) {
			leftOptions.insert(hackenbushDatabase->getOrInsertGameId(newGame));
		} else {
			rightOptions.insert(hackenbushDatabase->getOrInsertGameId(newGame));
		}
		position.addEdge(nodeFrom, nodeTo, colour);
	}
}

std::unordered_set<NormalGraph> Hackenbush::getTranspositions() const {
	return { position };
}

Hackenbush& createHackenbushPosition(const int& nodeSize, const std::string& inputString) {
	std::vector<PieceColour> positionStrip = charsToPieceColours(inputString);
	Hackenbush potentialPosition = Hackenbush(graphFromMatrixString(nodeSize, positionStrip));
	return hackenbushDatabase->getOrInsertGame(potentialPosition);
}
