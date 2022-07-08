//
// Created by Xander Lenstra on 25-03-22.
//

#include <boost/spirit/home/x3.hpp>
#include "Hackenbush.h"

CreateDatabase(HackenbushPosition, Hackenbush, hackenbushDatabase);

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
			leftOptions.push_back(hackenbushDatabase->getOrInsertGameId(newGame));
		} else {
			rightOptions.push_back(hackenbushDatabase->getOrInsertGameId(newGame));
		}
		position.addEdge(nodeFrom, nodeTo, colour);
	}
}

void Hackenbush::exploreSynched() {
	for (const auto& [nodeFromBlue, nodeToBlue, colourBlue] : position.getAllEdges()) {
		if (colourBlue != PieceColour::BLUE) continue;
		HackenbushPosition positionCopy = position;
		positionCopy.removeEdge(nodeFromBlue, nodeToBlue);

		std::vector<GameId> rightOptionsFromHere;

		for (const auto&[nodeFromRed, nodeToRed, colourRed] : position.getAllEdges()) {
			if (colourRed != PieceColour::RED) continue;
			HackenbushPosition positionCopyCopy = positionCopy;
			positionCopyCopy.removeEdge(nodeFromRed, nodeToRed);
			HackenbushPosition reducedPosition = positionCopyCopy.getSubGraphConnectedToGround(-1, -1);

			rightOptionsFromHere.push_back(hackenbushDatabase->getOrInsertGameId(Hackenbush(reducedPosition)));
		}

		synchedOptions.options.push_back(rightOptionsFromHere);
	}
}

std::unordered_set<NormalGraph> Hackenbush::getTranspositions() const {
	return { position };
}

bool Hackenbush::determineDecidedSynchedValue() {
	std::pair<size_t, size_t> edgeColours = position.getAllEdgeColours();
	if (edgeColours.first * edgeColours.second != 0) return false;
	synchedId = SGDatabase::getInstance().getDecidedGameWithValueId((double) ((long long) edgeColours.first - (long long) edgeColours.second));
	return true;
}

Hackenbush& createHackenbushPosition(const int& nodeSize, const std::string& inputString) {
	std::vector<PieceColour> positionStrip = charsToPieceColours(inputString);
	Hackenbush potentialPosition = Hackenbush(graphFromMatrixString(nodeSize, positionStrip));
	return hackenbushDatabase->getOrInsertGame(potentialPosition);
}
