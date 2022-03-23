//
// Created by s1935534 on 09/03/2022.
//

#ifndef CGSYNCH_2_PUSHSHOVEUTIL_H
#define CGSYNCH_2_PUSHSHOVEUTIL_H

#include <list>
#include <boost/functional/hash.hpp>

#include "RulesetUtil.h"

typedef std::vector<PieceColour> PushShovePosition;
std::ostream& operator<<(std::ostream& os, const PushShovePosition& position);

// Required to make a hashMap from transposed PushShovePositions to actual positions
namespace std {
	template<>
	struct hash<PushShovePosition> {
		size_t operator()(const PushShovePosition& position) const {
			return boost::hash_range(position.begin(), position.end());
		}
	};
}

#endif //CGSYNCH_2_PUSHSHOVEUTIL_H
