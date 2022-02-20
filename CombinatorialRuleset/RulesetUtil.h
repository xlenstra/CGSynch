//
// Created by ardour on 18-02-22.
//

#ifndef CGSYNCH_2_RULESETUTIL_H
#define CGSYNCH_2_RULESETUTIL_H

#include <concepts>
#include <cstddef>

typedef size_t RulesetId;

template<typename T>
concept ComparableType = std::equality_comparable<T>;

template<ComparableType U>
class AbstractRuleset;

template<typename T, typename U>
concept isRuleset = std::is_base_of<AbstractRuleset<U>,T>::value;

#endif //CGSYNCH_2_RULESETUTIL_H
