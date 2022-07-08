//
// Created by Xander Lenstra on 01/06/2022.
//

#include "GameUtil.h"

std::string winningPlayerGetDisplayString(const WinningPlayer& winningPlayer) {
	switch (winningPlayer) {
		case WinningPlayer::LEFT: return "LEFT";
		case WinningPlayer::RIGHT: return "RIGHT";
		case WinningPlayer::PREVIOUS: return "PREVIOUS";
		case WinningPlayer::NEXT: return "NEXT";
		case WinningPlayer::NONE: return "NONE";
		case WinningPlayer::DRAW: return "DRAW";
	}
}

std::ostream& operator<<(std::ostream& os, const WinningPlayer& winningPlayer) {
	os << winningPlayerGetDisplayString(winningPlayer);
	return os;
}

std::partial_ordering operator<=>(const WinningPlayer& winningPlayer1, const WinningPlayer& winningPlayer2) {
	if (winningPlayer1 == winningPlayer2) return std::partial_ordering::equivalent;
	if (winningPlayer1 == WinningPlayer::LEFT) {
		return std::partial_ordering::greater;
	} else if (winningPlayer1 == WinningPlayer::RIGHT) {
		return std::partial_ordering::less;
	} else if (winningPlayer2 == WinningPlayer::LEFT) {
		return std::partial_ordering::less;
	} else if (winningPlayer2 == WinningPlayer::RIGHT) {
		return std::partial_ordering::greater;
	}
	return std::partial_ordering::unordered;
}
