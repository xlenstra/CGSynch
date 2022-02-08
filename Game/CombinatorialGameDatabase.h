//
// Created by ardour on 08-02-22.
//

#ifndef CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
#define CGSYNCH_2_COMBINATORIALGAMEDATABASE_H


#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "CombinatorialGame.h"
#include <boost/container_hash/hash.hpp>

class CGDatabase {
public:
	CGDatabase() = default;
	~CGDatabase();


	CombinatorialGame* getGameWithSets(const std::unordered_set<CombinatorialGame*>&, const std::unordered_set<CombinatorialGame*>&);
	// Gets the combinatorial game by the left and right options of an existing game
	CombinatorialGame* getGame(CombinatorialGame*);
private:

	typedef std::pair<
		std::unordered_set<CombinatorialGame*>,
		std::unordered_set<CombinatorialGame*>
	> OptionSetPair;

	std::unordered_map<
		OptionSetPair,
		CombinatorialGame*
	> database;
};

//class pairHash {
//	size_t operator()(std::pair<std::unordered_set<CombinatorialGame*>, std::unordered_set<CombinatorialGame*>> pair) const {
//		return pair.first.hash_function() ^ std::hash(pair.second);
//	}
//};

CGDatabase cgDatabase;



#endif //CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
