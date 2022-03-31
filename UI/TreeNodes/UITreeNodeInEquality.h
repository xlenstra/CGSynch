//
// Created by s1935534 on 31/03/2022.
//

#ifndef CGSYNCH_2_UITREENODEINEQUALITY_H
#define CGSYNCH_2_UITREENODEINEQUALITY_H

#include <boost/bimap.hpp>
#include <boost/container/allocator.hpp>

#include "AbstractUITreeNode.h"

enum class ComparisonOperator {
	Less,
	Greater,
	LessEqual,
	GreaterEqual,
	Equal,
	Unequal,
	//Spaceship,
	None,
};

const std::unordered_map<std::string, ComparisonOperator> stringToComparisonTranslation {
	{"<", ComparisonOperator::Less},
	{">", ComparisonOperator::Greater},
	{"<=", ComparisonOperator::LessEqual},
	{">=", ComparisonOperator::GreaterEqual},
	{"==", ComparisonOperator::Equal},
	{"!=", ComparisonOperator::Unequal},
};

const std::unordered_map<ComparisonOperator, std::string> comparisonToStringTranslation {
	{ComparisonOperator::Less, "<"},
	{ComparisonOperator::Greater, ">"},
	{ComparisonOperator::LessEqual, "<="},
	{ComparisonOperator::GreaterEqual, ">="},
	{ComparisonOperator::Equal, "=="},
	{ComparisonOperator::Unequal, "!="},
};


class UITreeNodeInEquality : public AbstractUITreeNode {
public:
	UITreeNodeInEquality(const std::string& inEqualitySymbol, std::vector<AbstractUITreeNode*> children);
	TreeResultData compute() override;
	int getArity() override;
private:
	ComparisonOperator comparisonOperator = ComparisonOperator::None;
	bool checkComparisonOperator(std::partial_ordering result);


};


#endif //CGSYNCH_2_UITREENODEINEQUALITY_H
