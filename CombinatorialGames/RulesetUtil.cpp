
// Created by s1935534 on 09/03/2022.
//

#include "RulesetUtil.h"

std::ostream& operator<<(std::ostream& os, const StoneColour& stone){
	switch (stone) {
		case StoneColour::BLACK: os << "B"; break;
		case StoneColour::BLUE : os << "B"; break;
		case StoneColour::WHITE: os << "W"; break;
		case StoneColour::RED  : os << "R"; break;
		case StoneColour::NONE : os << " "; break;
	}
return os;
}