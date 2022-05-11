//
// Created by ardour on 10-05-22.
//

#include "GurobiSolver.h"
#include "gurobi_c++.h"

GurobiSolver::GurobiSolver(const RationalMatrix& rationalMatrix) : rationalMatrix(rationalMatrix) {

}

Rational GurobiSolver::solve() {


}

Rational GurobiSolver::calculateValue(
	const std::vector<Rational>& leftStrategy, const std::vector<Rational>& rightStrategy
) {
	Rational value = 0;
	for (size_t i = 0; i < leftStrategy.size(); ++i) {
		for (size_t j = 0; j < rightStrategy.size(); ++j) {
			value += rationalMatrix[i][j] * leftStrategy[i] * rightStrategy[j];
		}
	}
	return value;
}
