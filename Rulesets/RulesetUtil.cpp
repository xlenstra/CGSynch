
// Created by Xander Lenstra on 09/03/2022.
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
		case PieceColour::NONE : return ' ';
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

std::vector<PieceColour> charsToPieceColours(const std::string& input) {
	std::vector<PieceColour> returnVal;
	for (const auto& character : input) {
		returnVal.push_back(charToPieceColour(character));
	}
	return returnVal;
}


PieceColour operator-(PieceColour piece) {
	switch (piece) {
		case PieceColour::BLUE:
			return PieceColour::RED;
		case PieceColour::RED:
			return PieceColour::BLUE;
		case PieceColour::NONE:
			return PieceColour::NONE;
	}
}


std::ostream& operator<<(std::ostream& os, const PieceColour& stone){
	os << pieceColourToChar(stone);
	return os;
}