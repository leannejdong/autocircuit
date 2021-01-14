
#ifndef CIRCUIT_EIGENCONV_H
#define CIRCUIT_EIGENCONV_H
#include <vector>
#include </usr/include/eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

inline MatrixXf makeEigenMatrixFromVectors(const vector<vector<float>> &matvalues)
{
    size_t n_rows = matvalues.size();
    size_t n_cols = matvalues[0].size();
    MatrixXf A(n_rows,n_cols);

    for (size_t i=0; i!=n_rows; ++i) {
        assert(matvalues[i].size() == n_cols);

        for (size_t j=0; j!=n_cols; ++j) {
            A(i,j) = matvalues[i][j];
        }
    }

    return A;
}

inline VectorXf makeEigenVectorFromVectors(const vector<float> &vecvalues)
{
    size_t n = vecvalues.size();
    VectorXf b(n);

    for (size_t i = 0; i < n; ++i)
    {
        b(i) = vecvalues[i];
    }

    return b;

}

inline std::vector<float> makeVectorsFromEigen(const VectorXf &vecvalues)
{
    using std::vector;
    size_t n = vecvalues.size();
    vector<float> b(n);
    for (size_t i = 0; i < n; ++i)
    {
        b[i] = vecvalues(i);
    }
    return b;
}




#endif //CIRCUIT_EIGENCONV_H
