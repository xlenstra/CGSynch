//
// Created by s1935534 on 31/03/2022.
//

#include "UITreeNodeFunction.h"

#include <utility>

UITreeNodeFunction::UITreeNodeFunction(std::string functionName, const std::vector<AbstractUITreeNode*>& arguments) :
	AbstractUITreeNode(NodeType::Function, arguments),
	functionName(std::move(functionName))
{

}

TreeResultData UITreeNodeFunction::compute() {
	std::vector<TreeResultData> arguments;
	for (const auto& child : children) {
		arguments.push_back(child->compute());
	}
	return m_functionMap[functionName](arguments);
}

int UITreeNodeFunction::getArity() {
	if (functionName == "Shove" || functionName == "Push" || functionName == "Cherries" || functionName == "StackCherries") {
		// These are game generating functions, so they only take an argument of the input for the game
		return 1;
	} else {
		return 0;
	}
}

void UITreeNodeFunction::fillFunctionMap() {
	m_functionMap["Shove"] = Shove;

}
