//
// Created by Xander Lenstra on 09/03/2022.
//

#include "PushShoveUtil.h"

std::ostream& operator<<(std::ostream& os, const PushShovePosition& position) {
	for (const auto& stone : position)
		os << stone;
	return os;
}