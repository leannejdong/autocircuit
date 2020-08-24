#ifndef CIRCUIT_EIGENUTIL_H
#define CIRCUIT_EIGENUTIL_H
#include <iostream>
#include <vector>
#include <fstream>

#include <Eigen/Dense>
#include <Eigen/LU>

inline Eigen::VectorXf computeDifference(const Eigen::MatrixXf &A, const Eigen::VectorXf &x, const Eigen::VectorXf &b)
{
    return ((A * x).cast<double>() - b.cast<double>()).cast<float>();
}

template <typename Solver>

inline void solnrefine(const Solver &solver, const Eigen::MatrixXf &A, const Eigen::VectorXf &b, Eigen::VectorXf &refinesoln)
{
    //Eigen::FullPivLU<Eigen::MatrixXf> lu(A);
    Eigen::VectorXf db = computeDifference(A, refinesoln, b);
    refinesoln = refinesoln - solver.solve(db);
}


std::vector<std::vector<float>> makeMatrix(int n_rows, int n_cols)
{
    std::vector<std::vector<float>> result(n_rows);
    for (auto &row : result) {
        row.resize(n_cols);
    }
    return result;
}



#endif //CIRCUIT_EIGENUTIL_H
