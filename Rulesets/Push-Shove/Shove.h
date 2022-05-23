//
// Created by s1935534 on 09/03/2022.
//

#include "../AbstractGame.h"
#include "PushShoveUtil.h"

#ifndef CGSYNCH_2_SHOVE_H
#define CGSYNCH_2_SHOVE_H

class Shove : public AbstractGame<PushShovePosition> {
public:
	explicit Shove(PushShovePosition  position);
	std::string getDisplayString() override;
	PushShovePosition getAnyTransposition() const override;
	std::unordered_set<PushShovePosition> getTranspositions() const override;
	void exploreAlternating() override;
	bool tryToDetermineAlternatingId() override;
	void exploreSynched() override;

private:
	PushShovePosition position;
};


Shove& createShovePosition(const std::string& inputString);

extern std::shared_ptr<GameDatabase<PushShovePosition, Shove>> shoveDatabase;




#endif //CGSYNCH_2_SHOVE_H
