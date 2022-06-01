//
// Created by s1935534 on 01/06/2022.
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