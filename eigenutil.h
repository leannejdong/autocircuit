#ifndef CIRCUIT_EIGENUTIL_H
#define CIRCUIT_EIGENUTIL_H
#include <iostream>
#include <vector>
#include <fstream>

#include <Eigen/Dense>
#include <Eigen/LU>

void mprove(Eigen::MatrixXf A, Eigen::VectorXf eigenb)
{
    //Eigen::FullPivLU<Eigen::MatrixXf> lu = A.cast<double>().fullPivLu();
    Eigen::FullPivLU<Eigen::MatrixXf> lu = A.fullPivLu();
    Eigen::VectorXf refinesoln = lu.solve(eigenb);
    //Eigen::VectorXf db = ((A*refinesoln).cast<double>() - eigenb.cast<double>()).cast<float>();
    Eigen::VectorXf db = A*refinesoln - eigenb;
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
