//
// Created by ardour on 15-02-22.
//

#ifndef CGSYNCH_2_ABSTRACTRULESET_H
#define CGSYNCH_2_ABSTRACTRULESET_H

#include <concepts>
#include "../AbstractGame/CombinatorialGameDatabase.h"

template<typename T>
concept ComparableType = std::equality_comparable<T>;

template<ComparableType comparable>
class AbstractRuleset {
public:

	GameId abstractForm = -1;

	virtual std::unordered_set<comparable> getComparableObject() = 0;


protected:
	AbstractRuleset() = default;

//	std::unordered_set<>
};

#endif //CGSYNCH_2_ABSTRACTRULESET_H
