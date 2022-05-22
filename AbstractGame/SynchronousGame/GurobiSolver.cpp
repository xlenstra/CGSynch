//
// Created by ardour on 10-05-22.
//

#include "GurobiSolver.h"
#include <climits>

GRBEnv GurobiSolver::gurobiEnvironment = GRBEnv(true);

GurobiSolver::GurobiSolver(const Matrix<double>& rationalMatrix) : doubleMatrix(rationalMatrix) {
	gurobiEnvironment.set("OutputFlag", "0");
	gurobiEnvironment.set("LogFile", "mip1.log");
	gurobiEnvironment.start();
}

double GurobiSolver::solve() {
	// We solve the problem optimizing for the right player, as for that the matrix is ordered in the correct way
	// Formula taken from the syllabus of the course "Discrete Besliskunde" by L.C.M. KallenBerg, F.M. Spieksma and M.J.H. van den Bergh
	// Version "Najaar 2019", published by The Mathematical Institute at Leiden University.
	try {
		GRBModel gurobiModel = GRBModel(gurobiEnvironment);

		std::vector<GRBVar> yValues;
		yValues.push_back(gurobiModel.addVar(-MAX_BOUNDS, MAX_BOUNDS, 0.0, GRB_CONTINUOUS, "y0"));
		for (size_t i = 1; i <= doubleMatrix[0].size(); ++i) {
			yValues.push_back(gurobiModel.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "y" + std::to_string(i)));
		}

		gurobiModel.setObjective(GRBLinExpr(yValues[0]), GRB_MINIMIZE);

		for (const auto& row : doubleMatrix) {
			GRBLinExpr boundedByMatrixConstraint;
			for (size_t j = 0; j < row.size(); ++j) {
				boundedByMatrixConstraint += row.at(j) * yValues.at(j+1);
			}
			gurobiModel.addConstr(yValues[0], GRB_GREATER_EQUAL, boundedByMatrixConstraint);
		}
		GRBLinExpr sumToOneConstraint;
		for (size_t i = 1; i < yValues.size(); ++i) {
			sumToOneConstraint += yValues[i];
		}
		gurobiModel.addConstr(sumToOneConstraint, GRB_EQUAL, 1);

		gurobiModel.optimize();

		return yValues[0].get(GRB_DoubleAttr_X);
	} catch (GRBException& e) {
		std::cout << "Error code = " << e.getErrorCode () << std::endl;
		std::cout << e.getMessage() << std::endl;
	} catch (...) {
		std::cout << "Exception during optimization" << std::endl;
	}
	return 0.0;
}

