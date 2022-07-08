//
// Created by Xander Lenstra on 09/03/2022.
//
#pragma once

#ifndef CGSYNCH_2_PUSH_H
#define CGSYNCH_2_PUSH_H

#include "AbstractGame.h"
#include "PushShoveUtil.h"

class Push : public AbstractGame<PushShovePosition> {
public:
	explicit Push(PushShovePosition  position);
	Push(const Push& other) = default;

	std::string getDisplayString() override;
	[[nodiscard]] PushShovePosition getAnyTransposition() const override;
	[[nodiscard]] std::unordered_set<PushShovePosition> getTranspositions() const override;
	void exploreAlternating() override;
	void exploreSynched() override;
	bool determineDecidedSynchedValue() override;

private:
	PushShovePosition position;
};

Push& createPushPosition(const std::string& inputString);

#endif //CGSYNCH_2_PUSH_H
