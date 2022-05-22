//
// Created by ardour on 10-05-22.
//

#ifndef CGSYNCH_2_GUROBISOLVER_H
#define CGSYNCH_2_GUROBISOLVER_H

#include <vector>
#include <gurobi_c++.h>
#include "Rational.h"
#include "../../Util/Matrix.h"

const int MAX_BOUNDS = 32768;

class GurobiSolver {
public:
	explicit GurobiSolver(const Matrix<double>& rationalMatrix);
	double solve();
private:

	const Matrix<double>& doubleMatrix;

	static GRBEnv gurobiEnvironment;
};


#endif //CGSYNCH_2_GUROBISOLVER_H
