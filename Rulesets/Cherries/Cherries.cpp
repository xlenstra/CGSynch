//
// Created by ardour on 18-02-22.
//

#include "Cherries.h"
#include "CombinatorialGame/CombinatorialGame.h"

#include <utility>
#include <iostream>

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<CherriesPosition, Cherries>> GameDatabase<CherriesPosition, Cherries>::instance = nullptr;
template<> std::vector<std::shared_ptr<Cherries>> GameDatabase<CherriesPosition, Cherries>::database = {};
template<> std::unordered_map<CherriesPosition, GameId> GameDatabase<CherriesPosition, Cherries>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<GameDatabase<CherriesPosition, Cherries>> cherriesDatabase = GameDatabase<CherriesPosition, Cherries>::getInstance();

Cherries::Cherries(CherriesPosition position) : position(std::move(position)) {}

void Cherries::exploreAlternating() {
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
		if (unconnectedLine.front() == PieceColour::BLUE) {
			leftOptions.insert(cherriesDatabase->getOrInsertGameId(Cherries(copy)));
		} else {
			rightOptions.insert(cherriesDatabase->getOrInsertGameId(Cherries(copy)));
		}

		replacement = unconnectedLine;
		replacement.pop_back();
		if (!replacement.empty())
			secondCopy.insert(replacement);
		if (unconnectedLine.back() == PieceColour::BLUE) {
			leftOptions.insert(cherriesDatabase->getOrInsertGameId(Cherries(secondCopy)));
		} else {
			rightOptions.insert(cherriesDatabase->getOrInsertGameId(Cherries(secondCopy)));
		}
	}
	alternatingExplored = true;
}


// This function is completely unreadable spaghetti. I hope it works
void Cherries::exploreSynched() {
	Matrix<GameId> positions;

	int positionElementCount = 0;
	size_t blueMovesFound = 0;
	// First find a move for left
	for (const auto& blueUnconnectedLine : position) {
		if (blueUnconnectedLine.front() != PieceColour::BLUE && blueUnconnectedLine.back() != PieceColour::BLUE) {
			++positionElementCount;
			continue;
		}
		++blueMovesFound;
		if (blueUnconnectedLine.front() == PieceColour::BLUE && blueUnconnectedLine.back() == PieceColour::BLUE && blueUnconnectedLine.size() > 1) {
			++blueMovesFound;
		}
		// Copy position
		CherriesPosition positionWithoutBlue = position;
		auto elementToRemove = positionWithoutBlue.begin();
		std::advance(elementToRemove, positionElementCount);
		positionWithoutBlue.erase(elementToRemove);

		std::vector<GameId> redOptionsForBlueFront;
		std::vector<GameId> redOptionsForBlueBack;

		// Next, find a move for right in the original position
		for (const auto& redUnconnectedLine : position) {
			if (redUnconnectedLine.front() != PieceColour::RED && redUnconnectedLine.back() != PieceColour::RED) continue;
			// Then find where this section is stored in the position where red already played
			size_t elementCount = 0;
			CherriesPosition positionWithoutRedAndBlue = positionWithoutBlue;
			for (const auto& redInBlueIt : positionWithoutRedAndBlue) {
				if (redInBlueIt == redUnconnectedLine) break;
				++elementCount;
			}
			// If we don't play on the same segment
			if (elementCount != positionWithoutRedAndBlue.size()) {
				// Remove the segment we played on as well
				elementToRemove = positionWithoutRedAndBlue.begin();
				std::advance(elementToRemove, elementCount);
				positionWithoutRedAndBlue.erase(elementToRemove);

				// Then re-add both segments after removing the stones we played on
				if (blueUnconnectedLine.front() == PieceColour::BLUE) {
					std::deque<PieceColour> blueReplacement = blueUnconnectedLine;
					blueReplacement.pop_front();

					if (redUnconnectedLine.front() == PieceColour::RED) {
						CherriesPosition positionWithRemovedParts = positionWithoutRedAndBlue;
						std::deque<PieceColour> redReplacement = redUnconnectedLine;
						redReplacement.pop_front();

						if (!redReplacement.empty()) positionWithRemovedParts.insert(redReplacement);
						if (!blueReplacement.empty()) positionWithRemovedParts.insert(blueReplacement);
						redOptionsForBlueFront.push_back(cherriesDatabase->getOrInsertGameId(Cherries(positionWithRemovedParts)));
					}
					if (redUnconnectedLine.back() == PieceColour::RED && redUnconnectedLine.size() > 1) {
						CherriesPosition positionWithRemovedParts = positionWithoutRedAndBlue;
						std::deque<PieceColour> redReplacement = redUnconnectedLine;
						redReplacement.pop_back();

						if (!redReplacement.empty()) positionWithRemovedParts.insert(redReplacement);
						if (!blueReplacement.empty()) positionWithRemovedParts.insert(blueReplacement);
						redOptionsForBlueFront.push_back(cherriesDatabase->getOrInsertGameId(Cherries(positionWithRemovedParts)));
					}
				}
				if (blueUnconnectedLine.back() == PieceColour::BLUE && blueUnconnectedLine.size() > 1) {
					std::deque<PieceColour> blueReplacement = blueUnconnectedLine;
					blueReplacement.pop_back();

					if (redUnconnectedLine.front() == PieceColour::RED) {
						CherriesPosition positionWithRemovedParts = positionWithoutRedAndBlue;
						std::deque<PieceColour> redReplacement = redUnconnectedLine;
						redReplacement.pop_front();

						if (!redReplacement.empty()) positionWithRemovedParts.insert(redReplacement);
						if (!blueReplacement.empty()) positionWithRemovedParts.insert(blueReplacement);
						redOptionsForBlueBack.push_back(cherriesDatabase->getOrInsertGameId(Cherries(positionWithRemovedParts)));
					}
					if (redUnconnectedLine.back() == PieceColour::RED && redUnconnectedLine.size() > 1) {
						CherriesPosition positionWithRemovedParts = positionWithoutRedAndBlue;
						std::deque<PieceColour> redReplacement = redUnconnectedLine;
						redReplacement.pop_back();

						if (!redReplacement.empty()) positionWithRemovedParts.insert(redReplacement);
						if (!blueReplacement.empty()) positionWithRemovedParts.insert(blueReplacement);
						redOptionsForBlueBack.push_back(cherriesDatabase->getOrInsertGameId(Cherries(positionWithRemovedParts)));
					}
				}
			} else {
				// We played on the same segment, so we remove both the start and end of this segment
				CherriesPosition positionWithRemovedParts = positionWithoutRedAndBlue;
				std::deque<PieceColour> replacementSegment = blueUnconnectedLine;
				replacementSegment.pop_front();
				replacementSegment.pop_back();
				if (!replacementSegment.empty()) positionWithRemovedParts.insert(replacementSegment);
				if (blueUnconnectedLine.front() == PieceColour::BLUE)
					redOptionsForBlueFront.push_back(cherriesDatabase->getOrInsertGameId(Cherries(positionWithRemovedParts)));
				else if (blueUnconnectedLine.size() > 1)
					redOptionsForBlueBack.push_back(cherriesDatabase->getOrInsertGameId(Cherries(positionWithRemovedParts)));
			}
		}

		if (blueUnconnectedLine.front() == PieceColour::BLUE)
			positions.push_back(redOptionsForBlueFront);
		if (blueUnconnectedLine.back() == PieceColour::BLUE && blueUnconnectedLine.size() > 1)
			positions.push_back(redOptionsForBlueBack);
		++positionElementCount;
	}

	synchedOptions.options = positions;
	synchedOptions.leftMoveCount = blueMovesFound;
	if (blueMovesFound > 0) {
		synchedOptions.rightMoveCount = positions.getWidth();
	} else {
		synchedOptions.rightMoveCount = 0;
		for (const auto& unconnectedLine : position) {
			if (unconnectedLine.size() == 1) {
				synchedOptions.rightMoveCount += 1;
			} else {
				synchedOptions.rightMoveCount += 2;
			}
		}
	}

	synchedExplored = true;
}

std::unordered_set<CherriesPosition> Cherries::getTranspositions() const {
	std::unordered_set<CherriesPosition> transpositions;
	std::unordered_set<size_t> sectionsToReverse;
	size_t depth = 0;
	addTranspositionsRecursively(transpositions, sectionsToReverse, depth);
	return transpositions;
}

void Cherries::addTranspositionsRecursively(std::unordered_set<CherriesPosition>& transpositions, std::unordered_set<size_t>& sectionsToReverse, const size_t& depth) const {
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

CherriesPosition Cherries::getAnyTransposition() const {
	return position;
}

std::string Cherries::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& component : position) {
		for (const auto& character : component) {
			displayString += pieceColourToChar(character);
		}
		displayString += " ";
	}
	displayString += "\b";
	return displayString;
}

// Source: Mark van den Bergh, accessed via a paper written by Thomas de Mol I received from Walter Kosters.
bool Cherries::tryToDetermineAlternatingId() {
	// It turns out the value of each cherries game is an integer.
	// We get it, by summing the values of each segment of the position.
	int value = 0;
	for (const auto& segment : position) {
		// Simple base cases
		const int segmentSize = (int) segment.size();
		if (segmentSize == 0) continue; // ????
		// If no white stones in this segment:
		if (std::find(segment.begin(), segment.end(), PieceColour::RED) == segment.end()) {
			value += segmentSize;
			continue;
		// If no black stones in this segment:
		} else if (std::find(segment.begin(), segment.end(), PieceColour::BLUE) == segment.end()) {
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
	alternatingId = CGDatabase::getInstance().getInteger(value).getId();
	return true;
}


Cherries& createCherriesPosition(const std::string& inputString) {
	std::istringstream input(inputString);
	CherriesPosition position;
	std::deque<PieceColour> component;
	for (const auto& character : inputString) {
		PieceColour colourToAdd = charToPieceColour(character);
		if (colourToAdd == PieceColour::NONE) {
			if (!component.empty()) {
				position.insert(component);
				component = std::deque<PieceColour>();
			}
		} else {
			component.push_back(colourToAdd);
		}
	}
	if (!component.empty()) {
		position.insert(component);
	}

	Cherries ruleset = Cherries(position);
	return cherriesDatabase->getOrInsertGame(ruleset);
}