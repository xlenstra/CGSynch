//
// Created by ardour on 25-03-22.
//

#include "Hackenbush.h"

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<HackenbushPosition, Hackenbush>> GameDatabase<HackenbushPosition, Hackenbush>::instance = nullptr;
template<> std::vector<std::shared_ptr<Hackenbush>> GameDatabase<HackenbushPosition, Hackenbush>::database = {};
template<> std::unordered_map<HackenbushPosition, GameId> GameDatabase<HackenbushPosition, Hackenbush>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<GameDatabase<HackenbushPosition, Hackenbush>> hackenbushDatabase = GameDatabase<HackenbushPosition, Hackenbush>::getInstance();

Hackenbush::Hackenbush(const HackenbushPosition& position) : position(position) {}

std::string Hackenbush::getDisplayString() {
	return position.getDisplayString();
}

NormalGraph Hackenbush::getAnyTransposition() const {
	return position;
}

void Hackenbush::explore() {
	for (const auto& [nodeFrom, nodeTo, colour] : position.getAllEdges()) {
		position.removeEdge(nodeFrom, nodeTo);
		Hackenbush newGame = Hackenbush(position.getSubGraphConnectedToGround());
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
