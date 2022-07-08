//
// Created by Xander Lenstra on 18-02-22.
//

#include "Cherries.h"
#include "CombinatorialGame/CombinatorialGame.h"

#include <utility>
#include <iostream>

CreateDatabase(CherriesPosition, Cherries, cherriesDatabase);

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
			leftOptions.push_back(cherriesDatabase->getOrInsertGameId(Cherries(copy)));
		} else {
			rightOptions.push_back(cherriesDatabase->getOrInsertGameId(Cherries(copy)));
		}

		replacement = unconnectedLine;
		replacement.pop_back();
		if (!replacement.empty())
			secondCopy.insert(replacement);
		if (unconnectedLine.back() == PieceColour::BLUE) {
			leftOptions.push_back(cherriesDatabase->getOrInsertGameId(Cherries(secondCopy)));
		} else {
			rightOptions.push_back(cherriesDatabase->getOrInsertGameId(Cherries(secondCopy)));
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
		int i = segmentSize-1;
		for (; i >= 0; --i) {
			if (segment[i] != lastBlockColour) {
				lastBlockSize = segmentSize-1-i;
				break;
			}
		}
		int lastPosNotInLastBlock = i;
		PieceColour firstLargeBlockColour = PieceColour::NONE; // x
		PieceColour lastLargeBlockColour = PieceColour::NONE; // y
		for (int j = firstBlockSize; j <= lastPosNotInLastBlock; ++j) {
			if (segment[j] == segment[j - 1]) {
				firstLargeBlockColour = segment[j];
				break;
			}
		}
		for (int j = lastPosNotInLastBlock; j >= firstBlockSize; --j) {
			if (segment[j] == segment[j + 1]) {
				lastLargeBlockColour = segment[j];
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

bool Cherries::determineDecidedSynchedValue() {
	long long whiteStoneCount = 0;
	long long blackStoneCount = 0;
	long long whiteEdgeCount = 0;
	long long blackEdgeCount = 0;
	for (const auto& component : position) {
		for (const auto& piece : component) {
			switch (piece) {
				case PieceColour::BLUE:
					++blackStoneCount;
					break;
				case PieceColour::RED:
					++whiteStoneCount;
					break;
				case PieceColour::NONE:
					break;
			}
		}
		if (component[0] == PieceColour::BLUE) {
			++blackEdgeCount;
		} else {
			++whiteEdgeCount;
		}
		if (component.size() > 1) {
			if (component[component.size()-1] == PieceColour::BLUE) {
				++blackEdgeCount;
			} else {
				++whiteEdgeCount;
			}
		}
		if (blackEdgeCount * whiteEdgeCount != 0)
			return false;
	}
	if (blackStoneCount * whiteStoneCount != 0) {
		if (!synchedGamesParser::ignoreNonSeparable && !synchedGamesParser::rulesetsForWhichUndecidableErrorWasShown.contains("Cherries")) {
			std::cout << "A decided Cherries position was analyzed that had an undecided child position." << std::endl
			          << "In general, this is not allowed. However, we give a value to it anyway in case you want it." << std::endl
					  << "This warning will only be shown once per session for this game."
					  << "Type 'ignoreNonSeparable' to disable this warning for all games." << std::endl << std::endl << std::endl;
			synchedGamesParser::rulesetsForWhichUndecidableErrorWasShown.insert("Cherries");
		}
		synchedId = SGDatabase::getInstance().getDecidedGameWithValueId(blackEdgeCount - whiteEdgeCount);
		return true;
	}
	synchedId = SGDatabase::getInstance().getDecidedGameWithValueId(blackStoneCount - whiteStoneCount);
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