//
// Created by ardour on 10-05-22.
//

#ifndef CGSYNCH_2_GUROBISOLVER_H
#define CGSYNCH_2_GUROBISOLVER_H

#include <vector>
#include <gurobi_c++.h>
#include "Rational.h"

const int MAX_BOUNDS = 32768;

typedef std::vector<std::vector<double>> DoubleMatrix;

class GurobiSolver {
public:
	explicit GurobiSolver(const DoubleMatrix& rationalMatrix);
	double solve();
private:

	static inline GRBEnv getEnvironment();

	std::vector<double> getOptimalLeftStrategy();
	std::vector<double> getOptimalRightStrategy();
	double calculateValue(const std::vector<double>& leftStrategy, const std::vector<double>& rightStrategy);
	const DoubleMatrix& doubleMatrix;
};


#endif //CGSYNCH_2_GUROBISOLVER_H
