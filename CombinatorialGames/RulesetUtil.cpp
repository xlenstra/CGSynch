
// Created by s1935534 on 09/03/2022.
//

#include "RulesetUtil.h"

std::ostream& operator<<(std::ostream& os, const PieceColour& stone){
	switch (stone) {
		case PieceColour::BLACK: os << "B"; break;
		case PieceColour::BLUE : os << "B"; break;
		case PieceColour::WHITE: os << "W"; break;
		case PieceColour::RED  : os << "R"; break;
		case PieceColour::NONE : os << " "; break;
	}
return os;
}