//
// Created by s1935534 on 31/03/2022.
//

#ifndef CGSYNCH_2_UITREENODEFUNCTIONS_H
#define CGSYNCH_2_UITREENODEFUNCTIONS_H

#include <vector>
#include <stdexcept>
#include <CombinatorialGameUtil.h>
#include <AbstractGame.h>
#include <Push-Shove/Shove.h>

#include "AbstractUITreeNode.h"

TreeResultData Shove(const std::vector<TreeResultData>& arguments) {
	if (arguments.size() != 1)
		throw std::logic_error("Invalid amounts of arguments passed to createShovePosition!");
	std::string gameState = * (std::string*) arguments[0].data;
	TreeResultData result {};
	result.type = TreeResultType::Game;
	result.data = (void*) new AbstractId(getAbstractFormId<PushShovePosition>(createShovePosition(gameState)));
	delete (std::string*) arguments[0].data;
	return result;
}

#endif //CGSYNCH_2_UITREENODEFUNCTIONS_H
