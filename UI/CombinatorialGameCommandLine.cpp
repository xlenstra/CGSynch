//
// Created by s1935534 on 31/03/2022.
//

#include <string>
#include <iostream>
#include <TreeNodes/TypeCheckFunctions.h>
#include <TreeNodes/UITreeNodeInEquality.h>
#include "CombinatorialGameCommandLine.h"

//UICombinatorialGame& UICombinatorialGame::getInstance() {
//	static UICombinatorialGame instance;
//	return instance;
//}

std::string CombinatorialGameCommandLine::readInput() {
	std::string input;
	while (input.empty()) {
		std::getline(std::cin, input);
		std::cout << "input: " << input << std::endl;
	}
	return input;
}

void CombinatorialGameCommandLine::main() {
	while (true) {
		std::string input = readInput();
		if (input == "quit" || input == "q") return;
	}
}

// Based on a PT2 algorithm
AbstractUITreeNode* CombinatorialGameCommandLine::buildTree(std::string input) {
	// If the input starts and ends with a ", remove it and say it's a string.
	//if (input[0] == '"' && input[input.size()-1] == '"') return STRING;
	// If the input is literally a -, it will be wrongly determined to be an
	// int, so remove it right here.
	//if (input.compare("-") == 0) return STRING;
	if (input[0] == '(' && input[input.size()-1] == ')') {
		return buildTree(input.substr(1, input.size()-2));
	}
	int parenthesisAmount = 0;
	for (size_t i = 0; i < input.size(); ++i) {
		if (input[i] == '(') {
			++parenthesisAmount;
			continue;
		} else if (input [i] == ')') {
			--parenthesisAmount;
			continue;
		}

		if (parenthesisAmount == 0 && isEqualitySymbol(input[i])) {
			return createEqualityNode(input, i);
		}
	}
}

AbstractUITreeNode* CombinatorialGameCommandLine::createEqualityNode(std::string input, const size_t& i) {
	if (i == 0) throw std::logic_error("Invalid position for the " + std::string(input[i], 1) + " symbol!");
	if (input[i] == '=' && input[i-1] == '!') {
		AbstractUITreeNode* leftChild = buildTree(input.substr(0, i-1));
		AbstractUITreeNode* rightChild = buildTree(input.substr(i+1, input.size()-(i+1)-1));
		return new UITreeNodeInEquality("!=", { leftChild, rightChild } );
	}
}
