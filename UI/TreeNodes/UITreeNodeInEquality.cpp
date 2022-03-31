//
// Created by s1935534 on 31/03/2022.
//

#include <stdexcept>
#include <utility>
#include <CombinatorialGame.h>
#include "UITreeNodeInEquality.h"

UITreeNodeInEquality::UITreeNodeInEquality(const std::string& inEqualitySymbol, std::vector<AbstractUITreeNode*> children) :
	AbstractUITreeNode(NodeType::InEquality, std::move(children))
{
	comparisonOperator = stringToComparisonTranslation.at(inEqualitySymbol);
}

TreeResultData UITreeNodeInEquality::compute() {
	if (children.size() != 2) {
		throw std::logic_error("Invalid number of arguments passed to comparison operator!");
	}
	TreeResultData left = children[0]->compute();
	TreeResultData right = children[1]->compute();
	TreeResultData result{};
	result.type = TreeResultType::Bool;
	if (left.type != right.type) {
		throw std::logic_error("Cannot compare things of unequal type!");

	} else if (left.type == TreeResultType::String && right.type == TreeResultType::String) {
		std::string leftString = * (std::string*) left.data;
		std::string rightString = * (std::string*) right.data;
		auto comparisonResult = leftString == rightString;
		switch (comparisonOperator) {
			case ComparisonOperator::Equal:
				result.data = (void*) new bool(comparisonResult);
				break;
			case ComparisonOperator::Unequal:
				result.data = (void*) new bool(!comparisonResult);
				break;
			default:
				throw std::logic_error("Operator " + comparisonToStringTranslation.at(comparisonOperator) + " cannot be used to compare strings!");
		}
		delete (std::string*) left.data;
		delete (std::string*) right.data;

	} else if (left.type == TreeResultType::Game && right.type == TreeResultType::Game) {
		CombinatorialGame& leftGame = cgDatabase.idToGame(* (AbstractId*) left.data);
		CombinatorialGame& rightGame = cgDatabase.idToGame(* (AbstractId*) right.data);
		auto comparisonResult = leftGame <=> rightGame;
		result.data = (void*) new bool(checkComparisonOperator(comparisonResult));

		delete (AbstractId*) left.data;
		delete (AbstractId*) right.data;

	} else if (left.type == TreeResultType::Int && right.type == TreeResultType::Int) {
		int leftInt = *(int*) left.data;
		int rightInt = *(int*) right.data;
		auto comparisonResult = leftInt <=> rightInt;
		result.data = (void*) new bool(checkComparisonOperator(comparisonResult));

		delete (int*) left.data;
		delete (int*) right.data;

	} else {
		throw std::logic_error("Invalid comparison state!");
	}
	return result;
}

int UITreeNodeInEquality::getArity() {
	return 2;
}

bool UITreeNodeInEquality::checkComparisonOperator(std::partial_ordering result) {
	switch (comparisonOperator) {

		case ComparisonOperator::Less:
			return result < 0;
		case ComparisonOperator::Greater:
			return result > 0;
		case ComparisonOperator::LessEqual:
			return result <= 0;
		case ComparisonOperator::GreaterEqual:
			return result >= 0;
		case ComparisonOperator::Equal:
			return result == 0;
		case ComparisonOperator::Unequal:
			return result != 0;
		case ComparisonOperator::None:
		default:
			throw std::logic_error("Invalid state in comparison checking algorithm");
	}
}
