//
// Created by ardour on 07-04-22.
//

#include "TreeNodes.h"
#include "CombinatorialGame/CombinatorialGameDatabase.h"
#include "CombinatorialGame/CombinatorialGame.h"

void alternatingGamesParser::SpiritParserTreeNodeBinary::setLeftChild(AlternatingId newChild) {
	leftChild = newChild;
}

AlternatingId alternatingGamesParser::SpiritParserTreeNodeAdd::explore(AlternatingId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AlternatingId result = (cgDatabase.idToGame(parent) + cgDatabase.idToGame(leftChild)).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}

AlternatingId alternatingGamesParser::SpiritParserTreeNodeSubtract::explore(AlternatingId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AlternatingId result = (cgDatabase.idToGame(parent) - cgDatabase.idToGame(leftChild)).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}

AlternatingId alternatingGamesParser::SpiritParserTreeNodeCanonical::explore(AlternatingId parent) {
	CGDatabase& cgDatabase = CGDatabase::getInstance();
	AlternatingId result = (cgDatabase.idToGame(parent).getCanonicalForm()).getId();
	if (rightChild)
		return rightChild->explore(result);
	return result;
}
