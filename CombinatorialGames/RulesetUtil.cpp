
// Created by s1935534 on 09/03/2022.
//

#include "RulesetUtil.h"

int pieceColourToSign(const PieceColour& piece) {
	switch (piece) {
		case PieceColour::BLUE: return 1;
		case PieceColour::RED: return -1;
		case PieceColour::NONE: return 0;
	}
}


char pieceColourToChar(const PieceColour& piece) {
	switch (piece) {
		case PieceColour::BLUE : return 'B';
		case PieceColour::RED  : return 'R';
		case PieceColour::NONE : return '_';
	}
}

PieceColour charToPieceColour(const char& input) {
	switch (input) {
		case 'B':
		case 'b':
			return PieceColour::BLUE;
		case 'R':
		case 'r':
		case 'W':
		case 'w':
			return PieceColour::RED;
		case ' ':
		case '_':
		case '\t':
			return PieceColour::NONE;
		default:
			throw std::logic_error("Invalid character in input");
	}
}


std::ostream& operator<<(std::ostream& os, const PieceColour& stone){
	os << pieceColourToChar(stone);
	return os;
}