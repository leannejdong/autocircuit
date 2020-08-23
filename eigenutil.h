#ifndef CIRCUIT_EIGENUTIL_H
#define CIRCUIT_EIGENUTIL_H
#include <iostream>
#include <vector>
#include <fstream>

#include <Eigen/Dense>
#include <Eigen/LU>

inline void mprove(Eigen::MatrixXf A_float, Eigen::VectorXf eigenb_float)
{
    Eigen::MatrixXd A = A_float.cast<double>();
    Eigen::VectorXd eigenb = eigenb_float.cast<double>();
    Eigen::FullPivLU<Eigen::MatrixXd> lu = A.fullPivLu();
    Eigen::VectorXd refinesoln = lu.solve(eigenb);
    Eigen::VectorXd db = A*refinesoln - eigenb;
    refinesoln = refinesoln - lu.solve(db);
    // std::cout <<"The soln is : \n" << refinesoln << endl;
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
