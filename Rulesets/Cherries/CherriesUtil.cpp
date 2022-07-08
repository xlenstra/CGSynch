//
// Created by Xander Lenstra on 09/03/2022.
//

#include "CherriesUtil.h"


std::ostream& operator<<(std::ostream& os, const CherriesPosition& position) {
	for (const auto& unconnectedLine : position) {
		for (const auto& stone : unconnectedLine) {
			std::cout << stone;
		}
		os << " ";
	}
	return os;
}