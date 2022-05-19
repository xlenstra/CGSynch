//
// Created by s1935534 on 09/03/2022.
//

#ifndef CGSYNCH_2_PUSH_H
#define CGSYNCH_2_PUSH_H

#include "../AbstractGame.h"
#include "PushShoveUtil.h"

class Push : public AbstractGame<PushShovePosition> {
public:
	explicit Push(PushShovePosition  position);
	Push(const Push& other) = default;

	std::string getDisplayString() override;
	PushShovePosition getAnyTransposition() const override;
	std::unordered_set<PushShovePosition> getTranspositions() const override;
	void explore() override;

private:
	PushShovePosition position;
};

Push& createPushPosition(const std::string& inputString);

extern std::shared_ptr<GameDatabase<PushShovePosition, Push>> pushDatabase;


#endif //CGSYNCH_2_PUSH_H
