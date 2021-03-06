//
// Created by Xander Lenstra on 25-03-22.
//

#ifndef CGSYNCH_2_HACKENBUSH_H
#define CGSYNCH_2_HACKENBUSH_H

#include "../RulesetUtil.h"
#include "../AbstractGame.h"
#include "HackenbushUtil.h"

class Hackenbush : public AbstractGame<HackenbushPosition> {
public:
	explicit Hackenbush(const HackenbushPosition& position);
	std::string getDisplayString() override;
	[[nodiscard]] HackenbushPosition getAnyTransposition() const override;
	void exploreAlternating() override;
	[[nodiscard]] std::unordered_set<HackenbushPosition> getTranspositions() const override;
	void exploreSynched() override;
	bool determineDecidedSynchedValue() override;

private:
	HackenbushPosition position;
};

Hackenbush& createHackenbushPosition(const int&, const std::string& inputString);


extern const std::shared_ptr<GameDatabase<HackenbushPosition, Hackenbush>> hackenbushDatabase;

#endif //CGSYNCH_2_HACKENBUSH_H
