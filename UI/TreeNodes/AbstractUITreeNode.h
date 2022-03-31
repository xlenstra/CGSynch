//
// Created by s1935534 on 31/03/2022.
//
// Part of this code is self-plagiarized from the second assignment of Programmeertechnieken,
// which I followed in spring 2019. In this exercise, we had to make an excel sheet in the command line.
// I'll denote what exactly was copied at the code itself.
//

#ifndef CGSYNCH_2_ABSTRACTUITREENODE_H
#define CGSYNCH_2_ABSTRACTUITREENODE_H

#include <utility>
#include <vector>

enum class TreeResultType {
	Bool,
	Int,
	String,
	Game,
};

enum class NodeType {
	InEquality,
	Function,
	plus,
	minus,
	divide, // for dyadic rationals
	unary_minus,
	dot,
	parenthesis,
};

struct TreeResultData {
	void* data;
	TreeResultType type;
};

class AbstractUITreeNode {
public:
	virtual int getArity() = 0;
	virtual TreeResultData compute() = 0;

protected:
	AbstractUITreeNode(NodeType type, std::vector<AbstractUITreeNode*> children) : type(type), children(std::move(children)) {}

	NodeType type;
	std::vector<AbstractUITreeNode*> children;
};

#endif //CGSYNCH_2_ABSTRACTUITREENODE_H
