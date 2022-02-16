//
// Created by ardour on 15-02-22.
//

#ifndef CGSYNCH_2_RULESET_H
#define CGSYNCH_2_RULESET_H

#include <concepts>
#include "../AbstractGame/CombinatorialGameDatabase.h"

template<typename T>
concept ComparableType = std::equality_comparable<T>;

template<typename ComparableType>
class Ruleset {
public:

	GameId abstractForm = -1;

	virtual std::unordered_set<ComparableType> getComparableObject() = 0;


protected:
	Ruleset() = default;

//	std::unordered_set<>
};

#endif //CGSYNCH_2_RULESET_H
