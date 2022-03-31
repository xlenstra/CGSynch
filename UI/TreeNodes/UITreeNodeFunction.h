//
// Created by s1935534 on 31/03/2022.
//

#ifndef CGSYNCH_2_UITREENODEFUNCTION_H
#define CGSYNCH_2_UITREENODEFUNCTION_H

#include <string>
#include <vector>
#include <unordered_map>
#include "AbstractUITreeNode.h"
#include "UITreeNodeFunctions.h"

class UITreeNodeFunction : public AbstractUITreeNode {
public:
	UITreeNodeFunction(std::string functionName, const std::vector<AbstractUITreeNode*>& arguments);
	int getArity() override;
	TreeResultData compute() override;
private:
	void fillFunctionMap();

	std::string functionName;

	// The usage of this map and this type were taken from PT2
	typedef TreeResultData (*FnPtr)(const std::vector<TreeResultData>&);
	static std::unordered_map<std::string, FnPtr> m_functionMap;

};

#endif //CGSYNCH_2_UITREENODEFUNCTION_H
