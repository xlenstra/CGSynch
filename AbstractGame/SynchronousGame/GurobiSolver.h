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

	const DoubleMatrix& doubleMatrix;

	static GRBEnv gurobiEnvironment;
};


#endif //CGSYNCH_2_GUROBISOLVER_H
