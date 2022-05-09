//
// Created by ardour on 07-04-22.
//

#include "TreeNodes.h"
#include "CombinatorialGame/CombinatorialGameDatabase.h"
#include "CombinatorialGame/CombinatorialGame.h"

void parser::SpiritParserTreeNodeBinary::setLeftChild(AbstractId newChild) {
	leftChild = newChild;
}

AbstractId parser::SpiritParserTreeNodeAdd::explore(AbstractId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AbstractId result = (cgDatabase.idToGame(parent) + cgDatabase.idToGame(leftChild)).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}

AbstractId parser::SpiritParserTreeNodeSubtract::explore(AbstractId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AbstractId result = (cgDatabase.idToGame(parent) - cgDatabase.idToGame(leftChild)).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}

AbstractId parser::SpiritParserTreeNodeCanonical::explore(AbstractId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AbstractId result = (cgDatabase.idToGame(parent).getCanonicalForm()).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}
