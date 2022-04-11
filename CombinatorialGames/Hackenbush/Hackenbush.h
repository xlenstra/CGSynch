//
// Created by ardour on 25-03-22.
//

#ifndef CGSYNCH_2_HACKENBUSH_H
#define CGSYNCH_2_HACKENBUSH_H

#include "RulesetUtil.h"
#include "AbstractGame.h"
#include "HackenbushUtil.h"

class Hackenbush : public AbstractGame<HackenbushPosition> {
public:
	explicit Hackenbush(const HackenbushPosition& position);
	std::string getDisplayString() override;
	NormalGraph getAnyTransposition() const override;
	void explore() override;
	std::unordered_set<NormalGraph> getTranspositions() const override;

private:
	HackenbushPosition position;
};

extern std::shared_ptr<GameDatabase<HackenbushPosition, Hackenbush>> hackenbushDatabase;

#endif //CGSYNCH_2_HACKENBUSH_H
