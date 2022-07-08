//
// Created by Xander Lenstra on 21/02/2022.
//

#ifndef CGSYNCH_2_COMBINATORIALGAMEUTIL_H
#define CGSYNCH_2_COMBINATORIALGAMEUTIL_H

#include <boost/container_hash/hash.hpp>

/** Id of a game in the database */
typedef size_t AlternatingId;

// Forward declarations
class CombinatorialGame;
class CGDatabase;

typedef std::pair<std::set<AlternatingId>,std::set<AlternatingId>> leftRightOptionPair;

namespace std {
	template<>
	struct hash<leftRightOptionPair> {
		size_t operator()(const std::pair<std::set<AlternatingId>,std::set<AlternatingId>>& pair) const {
			size_t hash = boost::hash<std::set<AlternatingId>>()(pair.first);
			boost::hash_combine(hash, boost::hash<std::set<AlternatingId>>()(pair.second));
			return hash;
		}
	};
}

#endif //CGSYNCH_2_COMBINATORIALGAMEUTIL_H
