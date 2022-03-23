//
// Created by ardour on 18-02-22.
//

#include "Cherries.h"

#include <utility>
#include <iostream>

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<CherriesPosition, CherriesGame>> GameDatabase<CherriesPosition, CherriesGame>::instance = nullptr;
template<> std::vector<std::shared_ptr<CherriesGame>> GameDatabase<CherriesPosition, CherriesGame>::database = {};
template<> std::unordered_map<CherriesPosition, GameId> GameDatabase<CherriesPosition, CherriesGame>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<GameDatabase<CherriesPosition, CherriesGame>> cherriesDatabase = GameDatabase<CherriesPosition, CherriesGame>::getInstance();

CherriesGame::CherriesGame(CherriesPosition position) : position(std::move(position)) {}

void CherriesGame::explore() {
	for (auto it = position.begin(); it != position.end(); ++it) {
		const auto& unconnectedLine = *it;
		// Copy position
		CherriesPosition copy = position;
		auto elementToRemove = copy.begin();
		auto distance = std::distance(position.begin(), it);
		std::advance(elementToRemove, distance);
		copy.erase(elementToRemove);
		CherriesPosition secondCopy = copy;

		std::deque<PieceColour> replacement = unconnectedLine;
		replacement.pop_front();
		if (!replacement.empty())
			copy.insert(replacement);
		if (unconnectedLine.front() == PieceColour::BLACK) {
			leftOptions.insert(cherriesDatabase->getOrInsertGameId(CherriesGame(copy)));
		} else {
			rightOptions.insert(cherriesDatabase->getOrInsertGameId(CherriesGame(copy)));
		}

		replacement = unconnectedLine;
		replacement.pop_back();
		if (!replacement.empty())
			secondCopy.insert(replacement);
		if (unconnectedLine.back() == PieceColour::BLACK) {
			leftOptions.insert(cherriesDatabase->getOrInsertGameId(CherriesGame(secondCopy)));
		} else {
			rightOptions.insert(cherriesDatabase->getOrInsertGameId(CherriesGame(secondCopy)));
		}
	}
	explored = true;
}

std::unordered_set<CherriesPosition> CherriesGame::getTranspositions() const {
	std::unordered_set<CherriesPosition> transpositions;
	std::unordered_set<size_t> sectionsToReverse;
	size_t depth = 0;
	addTranspositionsRecursively(transpositions, sectionsToReverse, depth);
	return transpositions;
}

void CherriesGame::addTranspositionsRecursively(std::unordered_set<CherriesPosition>& transpositions, std::unordered_set<size_t>& sectionsToReverse, const size_t& depth) const {
	if (depth == position.size()) {
		CherriesPosition transposition {};
		int i = 0;
		for (const auto& component : position) {
			if (sectionsToReverse.contains(i)) {
				std::deque<PieceColour> reversedComponent = component;
				std::reverse(reversedComponent.begin(), reversedComponent.end());
				transposition.insert(reversedComponent);
			} else {
				transposition.insert(component);
			}
			++i;
		}
		transpositions.insert(transposition);
		return;
	}

	addTranspositionsRecursively(transpositions, sectionsToReverse, depth+1);
	sectionsToReverse.insert(depth);
	addTranspositionsRecursively(transpositions, sectionsToReverse, depth+1);
	sectionsToReverse.erase(depth);
}

CherriesPosition CherriesGame::getAnyTransposition() const {
	return position;
}

std::string CherriesGame::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& component : position) {
		for (const auto& character : component) {
			switch (character) {
				case PieceColour::WHITE:
				case PieceColour::RED:
					displayString += "W";
					break;
				case PieceColour::BLACK:
				case PieceColour::BLUE:
					displayString += "B";
					break;
				case PieceColour::NONE:
					displayString += " ";
					break;
			}
		}
		displayString += " ";
	}
	displayString += "\b";
	return displayString;
}

// Source: Mark van den Bergh, accessed via a paper written by Thomas de Mol I received from Walter Kosters.
bool CherriesGame::tryToDetermineAbstractForm() {
	// It turns out the value of each cherries game is an integer.
	// We get it, by summing the values of each segment of the position.
	int value = 0;
	for (const auto& segment : position) {
		// Simple base cases
		const int segmentSize = (int) segment.size();
		if (segmentSize == 0) continue; // ????
		// If no white stones in this segment:
		if (std::find(segment.begin(), segment.end(), PieceColour::WHITE) == segment.end()) {
			value += segmentSize;
			continue;
		// If no black stones in this segment:
		} else if (std::find(segment.begin(), segment.end(), PieceColour::BLACK) == segment.end()) {
			value -= segmentSize;
			continue;
		}
		// value = l(m-1) + r(n-1) + (l+r)/2 + (x+y)/2
		// Where x,y,l,r \in {-1, +1} and m,n > 0
		// For exact values, see the paper.
		PieceColour firstBlockColour = segment[0]; // l
		PieceColour lastBlockColour = segment[segmentSize - 1]; // r
		int firstBlockSize; // m
		int lastBlockSize;  // n
		for (int i = 0; i < segmentSize; ++i) {
			if (segment[i] != firstBlockColour) {
				firstBlockSize = i;
				break;
			}
		}
		for (int i = segmentSize-1; i >= 0; --i) {
			if (segment[i] != lastBlockColour) {
				lastBlockSize = segmentSize-1-i;
				break;
			}
		}
		PieceColour firstLargeBlockColour = PieceColour::NONE; // x
		PieceColour lastLargeBlockColour = PieceColour::NONE; // y
		for (int i = 1; i < segmentSize; ++i) {
			if (segment[i] == segment[i-1]) {
				firstLargeBlockColour = segment[i];
				break;
			}
		}
		for (int i = segmentSize-2; i >= 0; --i) {
			if (segment[i] == segment[i+1]) {
				lastLargeBlockColour = segment[i];
				break;
			}
		}
		value += (
			(int)firstBlockColour * (firstBlockSize - 1)
			+ (int)lastBlockColour * (lastBlockSize - 1)
			+ ((int)firstBlockColour + (int)lastBlockColour)/2
			+ ((int)firstLargeBlockColour + (int)lastLargeBlockColour)/2
		);
	}
	abstractForm = cgDatabase.getInteger(value).getId();
	return true;
}




CherriesGame& createCherriesPosition(const std::string& inputString) {
	std::istringstream input(inputString);
	CherriesPosition position;
	std::deque<PieceColour> component;
	for (const auto& character : inputString) {
		switch (character) {
			case 'W':
			case 'w':
				component.push_back(PieceColour::WHITE);
				break;
			case 'B':
			case 'b':
				component.push_back(PieceColour::BLACK);
				break;
			case ' ':
				if (!component.empty()) {
					position.insert(component);
					component = std::deque<PieceColour>();
				}
				break;
			default:
				break;
		}
	}
	if (!component.empty()) {
		position.insert(component);
	}

	CherriesGame ruleset = CherriesGame(position);
	return cherriesDatabase->getOrInsertGame(ruleset);
}