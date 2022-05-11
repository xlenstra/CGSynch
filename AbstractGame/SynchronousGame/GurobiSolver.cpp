//
// Created by ardour on 10-05-22.
//

#include "GurobiSolver.h"
#include <climits>

GurobiSolver::GurobiSolver(const DoubleMatrix& rationalMatrix) : doubleMatrix(rationalMatrix) {

}

double GurobiSolver::solve() {
	try {
		return calculateValue(
			getOptimalLeftStrategy(),
			getOptimalRightStrategy()
		);
	} catch (GRBException& e) {
		std::cout << "Error code = " << e.getErrorCode () << std::endl;
		std::cout << e.getMessage() << std::endl;
	} catch (...) {
		std::cout << "Exception during optimization" << std::endl;
	}
	return double(1)/0;
}

double GurobiSolver::calculateValue(
	const std::vector<double>& leftStrategy,
	const std::vector<double>& rightStrategy
) {
	double value = 0;
	for (size_t i = 0; i < leftStrategy.size(); ++i) {
		for (size_t j = 0; j < rightStrategy.size(); ++j) {
			value += doubleMatrix[i][j] * leftStrategy[i] * rightStrategy[j];
		}
	}
	return value;
}

std::vector<double> GurobiSolver::getOptimalLeftStrategy() {
	GRBEnv GurobiEnvironment = getEnvironment();
	GurobiEnvironment.start();

	GRBModel GurobiModel = GRBModel(GurobiEnvironment);

	std::vector<GRBVar> xValues;
	xValues.push_back(GurobiModel.addVar(-MAX_BOUNDS, MAX_BOUNDS, 0.0, GRB_CONTINUOUS, "x0"));
	for (size_t i = 0; i < doubleMatrix.size(); ++i) {
		xValues.push_back(GurobiModel.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "x" + std::to_string(i)));
	}

	GurobiModel.setObjective(GRBLinExpr(xValues[0]), GRB_MAXIMIZE);

	for (const auto& row : doubleMatrix) {
		GRBLinExpr constraint;
		for (size_t j = 0; j < row.size(); ++j) {
			constraint += row[j] * xValues[j];
		}
		GurobiModel.addConstr(xValues[0], GRB_GREATER_EQUAL, constraint);
	}
	GRBLinExpr constraint;
	for (size_t i = 1; i < xValues.size(); ++i) {
		constraint += xValues[i];
	}
	GurobiModel.addConstr(constraint, GRB_EQUAL, 1);

	GurobiModel.optimize();

	std::vector<double> results;
	for (size_t i = 1; i < xValues.size(); ++i) {
		results.push_back(xValues[i].get(GRB_DoubleAttr_X));
		std::cout << results.back();
	}
	std::cout << std::endl;
	return results;
}

std::vector<double> GurobiSolver::getOptimalRightStrategy() {
	GRBEnv GurobiEnvironment = getEnvironment();
	GRBModel GurobiModel = GRBModel(GurobiEnvironment);

	std::vector<GRBVar> yValues;
	yValues.push_back(GurobiModel.addVar(-MAX_BOUNDS, MAX_BOUNDS, 0.0, GRB_CONTINUOUS, "x0"));
	for (size_t i = 0; i < doubleMatrix.size(); ++i) {
		yValues.push_back(GurobiModel.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "x" + std::to_string(i)));
	}

	GurobiModel.setObjective(GRBLinExpr(yValues[0]), GRB_MINIMIZE);

	for (size_t col = 0; col < doubleMatrix[0].size(); ++col) {
		GRBLinExpr constraint;
		for (size_t row = 0; row < doubleMatrix.size(); ++row) {
			constraint += doubleMatrix[row][col] * yValues[row];
		}
		GurobiModel.addConstr(constraint, GRB_GREATER_EQUAL, yValues[0]);
	}
	GRBLinExpr constraint;
	for (size_t i = 1; i < yValues.size(); ++i) {
		constraint += yValues[i];
	}
	GurobiModel.addConstr(constraint, GRB_EQUAL, 1);

	GurobiModel.optimize();

	std::vector<double> results;
	for (size_t i = 1; i < yValues.size(); ++i) {
		results.push_back(yValues[i].get(GRB_DoubleAttr_X));
	}
	return results;
}

inline GRBEnv GurobiSolver::getEnvironment() {
	GRBEnv GurobiEnvironment = GRBEnv(true);
	GurobiEnvironment.set("OutputFlag", "0");
	GurobiEnvironment.set("LogFile", "mip1.log");
	GurobiEnvironment.start();

	return GurobiEnvironment;
}
