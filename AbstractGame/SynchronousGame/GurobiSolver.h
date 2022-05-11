//
// Created by ardour on 10-05-22.
//

#ifndef CGSYNCH_2_GUROBISOLVER_H
#define CGSYNCH_2_GUROBISOLVER_H

#include <vector>
#include "Rational.h"

typedef std::vector<std::vector<Rational>> RationalMatrix;

class GurobiSolver {
public:
	explicit GurobiSolver(const RationalMatrix& rationalMatrix);
	Rational solve();
private:
	std::vector<Rational> getOptimalLeftStrategy();
	std::vector<Rational> getOptimalRightStrategy();
	Rational calculateValue(const std::vector<Rational>& leftStrategy, const std::vector<Rational>& rightStrategy);
	const RationalMatrix& rationalMatrix;
};


#endif //CGSYNCH_2_GUROBISOLVER_H
