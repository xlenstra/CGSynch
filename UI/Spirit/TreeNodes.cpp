//
// Created by ardour on 07-04-22.
//

#include "TreeNodes.h"
#include "CombinatorialGame/CombinatorialGameDatabase.h"
#include "CombinatorialGame/CombinatorialGame.h"

void parser::SpiritParserTreeNodeBinary::setLeftChild(AlternatingId newChild) {
	leftChild = newChild;
}

AlternatingId parser::SpiritParserTreeNodeAdd::explore(AlternatingId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AlternatingId result = (cgDatabase.idToGame(parent) + cgDatabase.idToGame(leftChild)).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}

AlternatingId parser::SpiritParserTreeNodeSubtract::explore(AlternatingId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AlternatingId result = (cgDatabase.idToGame(parent) - cgDatabase.idToGame(leftChild)).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}

AlternatingId parser::SpiritParserTreeNodeCanonical::explore(AlternatingId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AlternatingId result = (cgDatabase.idToGame(parent).getCanonicalForm()).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}
