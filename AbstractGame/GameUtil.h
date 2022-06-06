//
// Created by s1935534 on 19/05/2022.
//

#ifndef CGSYNCH_GAMEUTIL_H
#define CGSYNCH_GAMEUTIL_H

#include <string>

/** Player that wins a game */
enum class WinningPlayer {
	LEFT,
	PREVIOUS,
	NEXT,
	RIGHT,
	DRAW,
	NONE = -1,
};
std::string winningPlayerGetDisplayString(const WinningPlayer& winningPlayer);

std::ostream& operator<<(std::ostream &os, const WinningPlayer& winningPlayer);

std::partial_ordering operator<=>(const WinningPlayer& winningPlayer1, const WinningPlayer& winningPlayer2);



#endif //CGSYNCH_GAMEUTIL_H
