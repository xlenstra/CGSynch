//
// Created by ardour on 10-05-22.
//

#include "GurobiSolver.h"
#include <climits>

GRBEnv GurobiSolver::gurobiEnvironment = GRBEnv(true);

GurobiSolver::GurobiSolver(const DoubleMatrix& rationalMatrix) : doubleMatrix(rationalMatrix) {
	gurobiEnvironment.set("OutputFlag", "0");
	gurobiEnvironment.set("LogFile", "mip1.log");
	gurobiEnvironment.start();
}

double GurobiSolver::solve() {
	try {
		GRBModel gurobiModel = GRBModel(gurobiEnvironment);

		std::vector<GRBVar> xValues;
		xValues.push_back(gurobiModel.addVar(-MAX_BOUNDS, MAX_BOUNDS, 0.0, GRB_CONTINUOUS, "x0"));
		for (size_t i = 0; i < doubleMatrix.size(); ++i) {
			xValues.push_back(gurobiModel.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "x" + std::to_string(i)));
		}

		gurobiModel.setObjective(GRBLinExpr(xValues[0]), GRB_MAXIMIZE);

		for (const auto& row : doubleMatrix) {
			GRBLinExpr upperBound;
			for (size_t j = 0; j < row.size(); ++j) {
				upperBound += row[j] * xValues[j];
			}
			gurobiModel.addConstr(xValues[0], GRB_LESS_EQUAL, upperBound);
		}
		GRBLinExpr constraint;
		for (size_t i = 1; i < xValues.size(); ++i) {
			constraint += xValues[i];
		}
		gurobiModel.addConstr(constraint, GRB_EQUAL, 1);

		gurobiModel.optimize();

		return xValues[0].get(GRB_DoubleAttr_X);
	} catch (GRBException& e) {
		std::cout << "Error code = " << e.getErrorCode () << std::endl;
		std::cout << e.getMessage() << std::endl;
	} catch (...) {
		std::cout << "Exception during optimization" << std::endl;
	}
	return double(1)/0;
}

