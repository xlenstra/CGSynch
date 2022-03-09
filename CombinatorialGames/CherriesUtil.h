//
// Created by s1935534 on 09/03/2022.
//

#ifndef CGSYNCH_2_CHERRIESUTIL_H
#define CGSYNCH_2_CHERRIESUTIL_H

#include <boost/functional/hash/hash.hpp>

#include "RulesetUtil.h"

/** Position in a game of cherries.
 * Cannot be unordered as we need the segments to be ordered identically for the hash function to work.
 */
typedef std::multiset<std::deque<StoneColour>> CherriesPosition;
std::ostream& operator<<(std::ostream& os, const CherriesPosition& position);

// Required to make a hashMap from transposed CherriesPositions to actual positions
namespace std {
	template<>
	struct hash<CherriesPosition> {
		size_t operator()(const CherriesPosition& position) const {
			return boost::hash_range(position.begin(), position.end());
		}
	};
}

#endif //CGSYNCH_2_CHERRIESUTIL_H
