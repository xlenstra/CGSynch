//
// Created by s1935534 on 31/03/2022.
//

#ifndef CGSYNCH_2_COMBINATORIALGAMECOMMANDLINE_H
#define CGSYNCH_2_COMBINATORIALGAMECOMMANDLINE_H

#include <TreeNodes/AbstractUITreeNode.h>

class CombinatorialGameCommandLine {
public:
	CombinatorialGameCommandLine() {}
	//UICombinatorialGame(const UICombinatorialGame& other) = delete;
	//void operator=(const UICombinatorialGame& other) = delete;

	void main();

private:
	std::string readInput();
	AbstractUITreeNode* buildTree(std::string input);

	AbstractUITreeNode* createEqualityNode(std::string input, const size_t& i);
};

#endif //CGSYNCH_2_COMBINATORIALGAMECOMMANDLINE_H
