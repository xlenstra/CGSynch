//
// Created by ardour on 15-02-22.
//

#include <regex>

#include "RulesetMath.h"




MathRuleset::MathRuleset(MathPosition inputString) : position(inputString) {

}

void MathRuleset::explore() {
	if (std::regex_match(position, std::basic_regex("-?[0-9]+"))) {
		abstractForm = CGDatabase::getInstance().getInteger(std::stoi(position)).getId();
		return;
	}
	if (std::regex_match(position, std::basic_regex("-?[0-9]+/[0-9]+"))) {
		int numerator = std::stoi(position.substr(0, position.find_first_of('/')));
		int denominator = std::stoi(position.substr(position.find_first_of('/'), std::string::npos));
		abstractForm = CGDatabase::getInstance().getDyadicRational(numerator, denominator).getId();
		return;
	}

}

std::string MathRuleset::getAnyTransposition() const {
	return std::string();
}

std::unordered_set<std::string> MathRuleset::getTranspositions() const {
	return std::unordered_set<std::string>();
}

std::string MathRuleset::getDisplayString() {
	return std::string();
}

bool MathRuleset::tryToDetermineAbstractForm() {
	return AbstractGame::tryToDetermineAbstractForm();
}

//void MathRuleset::setOptions(std::pair<std::vector<MathPosition>, std::vector<MathPosition>>) {
//
//}

