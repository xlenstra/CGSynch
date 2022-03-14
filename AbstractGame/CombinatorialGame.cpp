//
// Created by ardour on 03-02-22.
//

#include <unordered_set>
#include <utility>
#include <iostream>
#include "CombinatorialGame.h"

//CGCacheBlock::CGCacheBlock(
//	std::string displayString,
//	WinningPlayer cachedWinner,
//	AbstractId canonicalFormId,
//	AbstractId negativeFormId,
//	const std::optional<bool>& cachedIsInteger,
//	const std::optional<bool>& isNumber
//) :
//	displayString(std::move(displayString)), cachedWinner(cachedWinner), canonicalFormId(canonicalFormId),
//	negativeFormId(negativeFormId), cachedIsInteger(cachedIsInteger), isNumber(isNumber) {}


std::ostream& operator<<(std::ostream& os, WinningPlayer winningPlayer) {
	static const std::string LEFT = "LEFT";
	static const std::string RIGHT = "RIGHT";
	static const std::string PREVIOUS = "PREVIOUS";
	static const std::string NEXT = "NEXT";
	static const std::string NONE = "NONE";
	switch (winningPlayer) {
		case WinningPlayer::LEFT: os << LEFT; break;
		case WinningPlayer::RIGHT: os << RIGHT; break;
		case WinningPlayer::PREVIOUS: os << PREVIOUS; break;
		case WinningPlayer::NEXT: os << NEXT; break;
		case WinningPlayer::NONE: os << NONE; break;
	}
	return os;
}


CombinatorialGame::CombinatorialGame(
	std::unordered_set<AbstractId> leftOptions,
	std::unordered_set<AbstractId> rightOptions,
	AbstractId id
) : leftOptions(std::move(leftOptions)), rightOptions(std::move(rightOptions)), id(id) {}


CombinatorialGame::CombinatorialGame(const CombinatorialGame& other) :
	leftOptions(other.leftOptions),
	rightOptions(other.rightOptions),
	id(other.id)
{
	setCache(other.cacheBlock);
}


WinningPlayer CombinatorialGame::getWinner() const {
	if (cacheBlock.cachedWinner != WinningPlayer::NONE) return cacheBlock.cachedWinner;
	WinningPlayer winner;
	if (rightOptions.empty() && leftOptions.empty()) {
		winner = WinningPlayer::PREVIOUS;
	} else {
		bool leftHasWinningMove = false;
		bool rightHasWinningMove = false;
		for (const auto& gameId : leftOptions) {
			WinningPlayer leftWinner = ID_TO_GAME(gameId).getWinner();
			if (leftWinner == WinningPlayer::LEFT || leftWinner == WinningPlayer::PREVIOUS) {
				leftHasWinningMove = true;
				break;
			}
		}
		for (const auto& gameId : rightOptions) {
			WinningPlayer rightWinner = ID_TO_GAME(gameId).getWinner();
			if (rightWinner == WinningPlayer::RIGHT || rightWinner == WinningPlayer::PREVIOUS) {
				rightHasWinningMove = true;
				break;
			}
		}
		if (leftHasWinningMove && rightHasWinningMove) winner = WinningPlayer::NEXT;
		else if (leftHasWinningMove) winner = WinningPlayer::LEFT;
		else if (rightHasWinningMove) winner = WinningPlayer::RIGHT;
		else winner = WinningPlayer::PREVIOUS;
	}
	cacheBlock.cachedWinner = winner;
	return winner;
}

CombinatorialGame& CombinatorialGame::operator-() const {
	if (id == cgDatabase.zeroId) return cgDatabase.getZeroGame();
	if (cacheBlock.negativeFormId != -1ul) return ID_TO_GAME(cacheBlock.negativeFormId);
	std::unordered_set<AbstractId> newRightOptions {};
	newRightOptions.reserve(leftOptions.size());
	for (const auto& leftOption : leftOptions) {
		newRightOptions.insert((-ID_TO_GAME(leftOption)).getId());
	}
	std::unordered_set<AbstractId> newLeftOptions;
	newLeftOptions.reserve(rightOptions.size());
	for (const auto& rightOption: rightOptions) {
		newLeftOptions.insert((-ID_TO_GAME(rightOption)).getId());
	}
	cacheBlock.negativeFormId = cgDatabase.getGameId(newLeftOptions, newRightOptions);
	return ID_TO_GAME(cacheBlock.negativeFormId);
}

CombinatorialGame& CombinatorialGame::operator+(const CombinatorialGame& other) const {
	if (id == cgDatabase.zeroId) return cgDatabase.idToGame(other.getId());
	if (other.getId() == cgDatabase.zeroId) return cgDatabase.idToGame(id);
	if (other.getId() == cacheBlock.negativeFormId) return cgDatabase.getZeroGame();
	if (cacheBlock.additionCache.contains(other.getId()))
		return ID_TO_GAME(cacheBlock.additionCache[other.getId()]);
	std::unordered_set<AbstractId> newLeftOptions {};
	std::unordered_set<AbstractId> newRightOptions {};
	newLeftOptions.reserve(leftOptions.size() + other.leftOptions.size());
	newRightOptions.reserve(rightOptions.size() + other.rightOptions.size());
	for (const auto& leftOption : leftOptions) {
		newLeftOptions.insert((ID_TO_GAME(leftOption) + other).getId());
	}
	for (const auto& leftOption : other.leftOptions) {
		newLeftOptions.insert((*this + ID_TO_GAME(leftOption)).getId());
	}
	for (const auto& rightOption: rightOptions) {
		newRightOptions.insert((ID_TO_GAME(rightOption) + other).getId());
	}
	for (const auto& rightOption: other.rightOptions) {
		newRightOptions.insert((*this + ID_TO_GAME(rightOption)).getId());
	}
	CombinatorialGame& newGame = GET_GAME(newLeftOptions, newRightOptions);
	cacheBlock.additionCache[other.getId()] = newGame.getId();
	return newGame;
}

CombinatorialGame& CombinatorialGame::operator-(const CombinatorialGame& other) const {
	if (other.getId() == cgDatabase.zeroId) return cgDatabase.idToGame(id);
	if (id == cgDatabase.zeroId) return -other;
	if (id == other.getId()) return cgDatabase.getZeroGame();
	if (cacheBlock.subtractionCache.contains(other.getId()))
		return ID_TO_GAME(cacheBlock.subtractionCache[other.getId()]);
	std::unordered_set<AbstractId> newLeftOptions {};
	std::unordered_set<AbstractId> newRightOptions {};
	newLeftOptions.reserve(leftOptions.size() + other.leftOptions.size());
	newRightOptions.reserve(rightOptions.size() + other.rightOptions.size());
	for (const auto& leftOption : leftOptions) {
		newLeftOptions.insert((ID_TO_GAME(leftOption) - other).getId());
	}
	for (const auto& rightOption: other.rightOptions) {
		newLeftOptions.insert((*this - ID_TO_GAME(rightOption)).getId());
	}
	for (const auto& rightOption: rightOptions) {
		newRightOptions.insert((ID_TO_GAME(rightOption) - other).getId());
	}
	for (const auto& leftOption : other.leftOptions) {
		newRightOptions.insert((*this - ID_TO_GAME(leftOption)).getId());
	}
	CombinatorialGame& newGame = GET_GAME(newLeftOptions, newRightOptions);
	cacheBlock.subtractionCache[other.getId()] = newGame.getId();
	return newGame;
}

bool CombinatorialGame::operator==(const CombinatorialGame& other) const {
	return id == other.id; // ID's imply isomorphism by how the database is structured
}


std::partial_ordering CombinatorialGame::operator<=>(const CombinatorialGame& other) const {
	CombinatorialGame& leftGame = this->getSimplestAlreadyCalculatedForm();
	CombinatorialGame& rightGame = other.getSimplestAlreadyCalculatedForm();
	if (leftGame.cacheBlock.compareCache.contains(rightGame.getId()))
		return leftGame.cacheBlock.compareCache.at(rightGame.getId());
	// If we already know that this game is a number without doing additional calculations,
	// just compare the numbers.
	if (leftGame.cacheBlock.isNumber && leftGame.isNumber() && rightGame.cacheBlock.isNumber && rightGame.isNumber())
		return leftGame.getNumberValue() <=> rightGame.getNumberValue();
	AbstractId differenceGame;
	bool swappedGames = false;
	if (leftGame.getBirthday() < rightGame.getBirthday())
		differenceGame = (leftGame - rightGame).getId();
	else {
		differenceGame = (rightGame - leftGame).getId();
		swappedGames = true;
	}

	std::partial_ordering ordering = std::partial_ordering::unordered;

	switch (ID_TO_GAME(differenceGame).getWinner()) {
		case WinningPlayer::LEFT:
			if (swappedGames) {
				ordering = std::partial_ordering::less;
			} else {
				ordering = std::partial_ordering::greater;
			}
			break;
		case WinningPlayer::PREVIOUS:
			ordering = std::partial_ordering::equivalent;
			break;
		case WinningPlayer::NEXT:
			ordering = std::partial_ordering::unordered;
			break;
		case WinningPlayer::RIGHT:
			if (swappedGames) {
				ordering = std::partial_ordering::greater;
			} else {
				return std::partial_ordering::less;
			}
			break;
		case WinningPlayer::NONE:
		default:
			throw(std::domain_error("Invalid game: " + (ID_TO_GAME(differenceGame)).getDisplayString()));
	}

	cacheBlock.compareCache.insert({ other.getId(), ordering });
	cacheBlock.compareCache.insert({ rightGame.getId(), ordering });
	leftGame.cacheBlock.compareCache.insert({ other.getId(), ordering });
	leftGame.cacheBlock.compareCache.insert({ rightGame.getId(),  ordering});
	std::partial_ordering reversedOrdering = ordering;
	if (ordering == std::partial_ordering::less) {
		reversedOrdering = std::partial_ordering::greater;
	} else if (ordering == std::partial_ordering::greater) {
		reversedOrdering = std::partial_ordering::less;
	}
	other.cacheBlock.compareCache.insert({ id, reversedOrdering });
	other.cacheBlock.compareCache.insert({ leftGame.id, reversedOrdering });
	rightGame.cacheBlock.compareCache.insert({ id, reversedOrdering });
	rightGame.cacheBlock.compareCache.insert({ leftGame.id, reversedOrdering });
	return ordering;
}

void CombinatorialGame::setCache(const CGCacheBlock& other) {
	cacheBlock = other;
}

std::string CombinatorialGame::getDisplayString() const {
	if (id == cgDatabase.zeroId) {
		cacheBlock.displayString = "0";
		return cacheBlock.displayString;
	}
	if (isCanonicalNumber()) {

		DyadicRational numberValue = *getNumberValue();
		if (numberValue.denominator == 1) {
			cacheBlock.displayString = std::to_string(numberValue.numerator);
		} else {
			cacheBlock.displayString = std::to_string(numberValue.numerator) + "/" + std::to_string(numberValue.denominator);
		}
		return cacheBlock.displayString;
	}
	if (!cacheBlock.displayString.empty()) return cacheBlock.displayString;
	cacheBlock.displayString = "{";
	for (const auto &leftId: leftOptions) {
		cacheBlock.displayString += ID_TO_GAME(leftId).getDisplayString() + ",";
	}
	if (!leftOptions.empty()) cacheBlock.displayString += "\b"; // remove trailing ,
	cacheBlock.displayString += "|";
	for (const auto &rightId: rightOptions) {
		cacheBlock.displayString += ID_TO_GAME(rightId).getDisplayString() + ",";
	}
	if (!rightOptions.empty()) cacheBlock.displayString += "\b"; // remove trailing ,
	cacheBlock.displayString += "}";
	return cacheBlock.displayString;
}

size_t CombinatorialGame::getBirthday() const {
	if (leftOptions.empty() && rightOptions.empty()) return 0;
	if (cacheBlock.birthDay) return *cacheBlock.birthDay;
	size_t maxFound = 0;
	for (const auto& leftId : leftOptions) {
		maxFound = std::max(maxFound, ID_TO_GAME(leftId).getBirthday());
	}
	for (const auto& rightId: rightOptions) {
		maxFound = std::max(maxFound, ID_TO_GAME(rightId).getBirthday());
	}
	cacheBlock.birthDay = maxFound + 1;
	return *cacheBlock.birthDay;
}

CombinatorialGame& CombinatorialGame::getCanonicalForm() const {
	if (cacheBlock.canonicalFormId != AbstractId(-1)) return ID_TO_GAME(cacheBlock.canonicalFormId);
	if (getWinner() == WinningPlayer::PREVIOUS) return ID_TO_GAME(0);

	std::unordered_set<AbstractId> newLeftOptions {};
	std::unordered_set<AbstractId> newRightOptions {};
	// First we convert all our options to canonical form
	for (const auto& leftId : leftOptions) {
		newLeftOptions.insert(ID_TO_GAME(leftId).getCanonicalForm().getId());
	}
	for (const auto& rightId : rightOptions) {
		newRightOptions.insert(ID_TO_GAME(rightId).getCanonicalForm().getId());
	}

	// Then we delete any dominated positions
	std::unordered_set<AbstractId> undominatedLeftOptions {};
	std::unordered_set<AbstractId> undominatedRightOptions {};
	for (const auto& leftId : newLeftOptions) {
		bool shouldAdd = true;
		for (const auto& otherLeftId : newLeftOptions) {
			if (leftId != otherLeftId && ID_TO_GAME(leftId) < ID_TO_GAME(otherLeftId)) {
				shouldAdd = false;
				break;
			}
		}
		for (const auto& alreadyAddedLeftId : undominatedLeftOptions) {
			if (leftId != alreadyAddedLeftId && ID_TO_GAME(alreadyAddedLeftId) <=> ID_TO_GAME(leftId) == 0) {
				shouldAdd = false;
				break;
			}
		}
		if (shouldAdd)
			undominatedLeftOptions.insert(leftId);
	}
	for (const auto& rightId : newRightOptions) {
		bool shouldAdd = true;
		for (const auto& otherRightId : newRightOptions) {
			if (rightId != otherRightId && ID_TO_GAME(rightId) > ID_TO_GAME(otherRightId)) {
				shouldAdd = false;
				break;
			}
		}
		for (const auto& alreadyAddedRightId : undominatedRightOptions) {
			if (rightId != alreadyAddedRightId && ID_TO_GAME(alreadyAddedRightId) <=> ID_TO_GAME(rightId) == 0) {
				shouldAdd = false;
				break;
			}
		}
		if (shouldAdd)
			undominatedRightOptions.insert(rightId);
	}

	// Check for simplest number theorem
	if (undominatedLeftOptions.size() == 1 && undominatedRightOptions.size() == 1 && ID_TO_GAME(*undominatedLeftOptions.begin()).isNumber() && ID_TO_GAME(*undominatedRightOptions.begin()).isNumber()) {
		std::optional<DyadicRational> number =
			getSimplestNumber(
				*ID_TO_GAME(*undominatedLeftOptions.begin()).getNumberValue(),
				*ID_TO_GAME(*undominatedRightOptions.begin()).getNumberValue()
			);
		if (number) {
			CombinatorialGame& newGame = cgDatabase.getDyadicRational(number->numerator, number->denominator);
			cacheBlock.canonicalFormId = newGame.getId();
			newGame.cacheBlock.canonicalFormId = newGame.getId();
			return newGame;
		}
	}


	// Lastly, we do reversibility
	std::unordered_set<AbstractId> finalLeftOptions {};
	std::unordered_set<AbstractId> finalRightOptions {};
	bool anyChangesWithReversibility = false;
	for (const auto& leftId : undominatedLeftOptions) {
		CombinatorialGame& leftOption = ID_TO_GAME(leftId);
		bool shouldAddLeftId = true;
		for (const auto& rightId : leftOption.getRightOptions()) {
			CombinatorialGame& leftRightOption = ID_TO_GAME(rightId);
			if (leftRightOption <= *this) {
				anyChangesWithReversibility = true;
				shouldAddLeftId = false;
				finalLeftOptions.insert(leftRightOption.leftOptions.begin(), leftRightOption.leftOptions.end());
				break;
			}
		}
		if (shouldAddLeftId)
			finalLeftOptions.insert(leftId);
	}
	for (const auto rightId : undominatedRightOptions) {
		CombinatorialGame& rightOption = ID_TO_GAME(rightId);
		bool shouldAddRightId = true;
		for (const auto& leftId : rightOption.getLeftOptions()) {
			CombinatorialGame& rightLeftOption = ID_TO_GAME(leftId);
			if (rightLeftOption >= *this) {
				anyChangesWithReversibility = true;
				shouldAddRightId = false;
				finalRightOptions.insert(rightLeftOption.rightOptions.begin(), rightLeftOption.rightOptions.end());
				break;
			}
		}
		if (shouldAddRightId)
			finalRightOptions.insert(rightId);
	}

	if (anyChangesWithReversibility) {
		// Whelp, more options were added, and so presumably more options to remove by domination or reversibility.
		// Time to start over!
		CombinatorialGame& newGame = GET_GAME(finalLeftOptions, finalRightOptions).getCanonicalForm();
		cacheBlock.canonicalFormId = newGame.getId();
		newGame.cacheBlock.canonicalFormId = newGame.getId();
		return newGame;
	}

	CombinatorialGame& newGame = GET_GAME(finalLeftOptions, finalRightOptions);
	cacheBlock.canonicalFormId = newGame.getId();
	newGame.cacheBlock.canonicalFormId = newGame.getId();
	return newGame;
}

CombinatorialGame& CombinatorialGame::getSimplestAlreadyCalculatedForm() const {
	if (cacheBlock.canonicalFormId != AbstractId(-1))
		return ID_TO_GAME(cacheBlock.canonicalFormId);
	return ID_TO_GAME(id);
}

bool CombinatorialGame::isCanonicalInteger() const {
	if (id == cgDatabase.zeroId) return true;
//	if (cgDatabase.getSavedIntegers().contains(id)) return true;
	if (cacheBlock.isInteger) return cacheBlock.isInteger.value();
	cacheBlock.isInteger = false;
	if (rightOptions.empty() && leftOptions.size() == 1) {
		cacheBlock.isInteger = ID_TO_GAME(*leftOptions.begin()).isCanonicalInteger();
	} else if (leftOptions.empty() && rightOptions.size() == 1) {
		cacheBlock.isInteger = ID_TO_GAME(*rightOptions.begin()).isCanonicalInteger();
	}
	return cacheBlock.isInteger.value();
}

bool CombinatorialGame::_isNumber() const {
	if (isCanonicalInteger()) return true;
	if (!isInCanonicalForm()) return getCanonicalForm().isNumber();
	// At this point we know we are in canonical form, and if we are a number, we're a rational
	if (leftOptions.size() != 1) return false;
	if (rightOptions.size() != 1) return false;
	CombinatorialGame& leftGame = cgDatabase.idToGame(*leftOptions.begin());
	if (!leftGame.isNumber()) return false;
	CombinatorialGame& rightGame = cgDatabase.idToGame(*rightOptions.begin());
	if (!rightGame.isNumber()) return false;
	return leftGame < rightGame; // Simplest number theorem
}

bool CombinatorialGame::isInCanonicalForm() const {
	if (cacheBlock.canonicalFormId == -1ul)
		getCanonicalForm();
	return id == cacheBlock.canonicalFormId;
}

bool CombinatorialGame::isNumber() const {
	if (!cacheBlock.isNumber)
		cacheBlock.isNumber = _isNumber();
	return cacheBlock.isNumber.value();
}

std::optional<DyadicRational> CombinatorialGame::getNumberValue() const {
	if (cacheBlock.numberValue) return cacheBlock.numberValue;
	if (!isInCanonicalForm()) {
		cacheBlock.numberValue = getCanonicalForm().getNumberValue();
		return cacheBlock.numberValue;
	} else if (!isNumber()) {
		return {};
	} else if (isCanonicalInteger()) {
		if (*this == cgDatabase.getZeroGame()) {
			cacheBlock.numberValue = DyadicRational(0, 1);
		} else if (leftOptions.empty()) {
			cacheBlock.numberValue = DyadicRational((int)-getBirthday(), 1);
		} else {
			cacheBlock.numberValue = DyadicRational((int) getBirthday(), 1);
		}
	} else {
		// If we're not a canonical number, but we are in canonical form, that must mean we're a dyadic rational.
		// Calculate our value using the simples number theorem.
		auto& leftGame = ID_TO_GAME(*leftOptions.begin());
		auto& rightGame = ID_TO_GAME(*rightOptions.begin());
		auto leftNumber = *leftGame.getNumberValue();
		auto rightNumber = *rightGame.getNumberValue();
		cacheBlock.numberValue = getSimplestNumber(leftNumber, rightNumber);
	}
	return cacheBlock.numberValue;
}

bool CombinatorialGame::isCanonicalNumber() const {
	return isInCanonicalForm() && isNumber(); // isNumber() calls isInCanonicalForm(), so checking that first is faster
}

//void CombinatorialGame::copyCacheToCanonicalForm() const {
//	if (cacheBlock.canonicalFormId == -1ul)
//		throw(std::underflow_error("Copying cache to not yet determined canonical form!"));
//	CombinatorialGame& canonicalForm = ID_TO_GAME(cacheBlock.canonicalFormId);
//
//}
