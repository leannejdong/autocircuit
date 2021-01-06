//
// Created by leanne on 8/17/20.
//

#ifndef AUTOCIRCUIT_MESH_H
#define AUTOCIRCUIT_MESH_H
#include "graph.h"

class mesh {
private:
    std::vector<std::vector<float>> res;
    std::vector<std::vector<float>> volt;
    std::vector<std::vector<float>> current;
    std::vector<std::vector<int>> mcurrent;
    std::vector<std::vector<float>> a;
    std::vector<std::vector<int>> indx;

public:

    template <typename MultiArray2d>
    void setdircur(int r, int c, MultiArray2d &mcurrent)
    //void setdircur(int r, int c, std::vector<std::vector<int>> &mcurrent)
    {
        using namespace std;
        //cout << r << endl;
        assert(r > 0);
        assert(c > r);
        int k, i, j, n, ii, jj, nloop;
        for(k = 1; k < c-1; k = k + r + 1) {
            // count the number of links in each mesh
            n = 0;
            ii = -1; //initialize ii to -1 to overwrite it just once
            for (i = 0; i < r; i++) {
                auto const first = begin(mcurrent[i]) + k, last = first + r;
                auto const one = find(first, last, 1);
                if (one != last) {
                    n++;
                    ii = i++; // Incement i because we finished this iteration
                    jj = one - first; // j
                    n += count(one, last, 1); // count the remaining 1s here
                    break;
                }
            }
            // Now count the rest of the 1s, counting with the next value of i from the previous loop
            for (; i < r; i++) {
                auto const first = begin(mcurrent[i]) + k, last = first + r;
                n += count(first, last, 1);
            }
            n = n / 2;
            assert(ii != -1);
            mcurrent[jj][ii + k] = -1; //set the symmetric of the first one to -1 that I have found -> oriented the first side
            nloop = 1; // first branch of mesh has been directed
            // next, look for the next branch of mesh in row jj
            i = jj; // select row
            do {
                for (j = 0; j < r; j++)  //check columns searching for a 1
                    if (mcurrent[i][j + k] == 1) {
                        mcurrent[j][i + k] = -1; // set symmetric element to 1
                        nloop++; // increase the number of directed sides
                        i = j;  //select next row
                        break;
                    }
            }
            while (nloop < n); // iterate procedure until direction found for all branches
        }
    }

    vector<vector<float>> createmat(int m, int r, vector<vector<int>> &mcurrent, vector<vector<float>> &res, int c)
    {

        int  k, kk, ii, jj, i, j;
        vector<vector<float>> a(m);
        for (auto &row : a)
        {
            row.resize(m, 0);
        }

        //element on the diagonal
        for(k=1; k<c-1; k=k+r+1)
        {
            for(i=0; i < r; i++)
            {
                for(j=0; j < r; j++)
                {
                    if(mcurrent[i][j+k]==1 && res[i][j]!=0)
                    {
                        a[k/r][k/r] += res[i][j];
                    }
                }
            }
        }

        //element off the diagonal
        for(i=0; i < m-1; i++) {
            for (j = i + 1; j < m; j++) {
                k = 1 + (r + 1) * i;
                kk = 1 + (r + 1) * j;
                for (ii = 0; ii < r; ii++) {
                    for (jj = ii; jj < r; jj++) {
                        if (mcurrent[ii][jj + k] != 0 && mcurrent[ii][jj + kk] != 0) {
                            a[i][j] += (mcurrent[ii][jj + k] * mcurrent[ii][jj + kk]) * res[ii][jj];
                        }
                    }
                }
            }
        }

            for(i = 0; i < m; i++)
            {
                for(j = 0; j < i; j++)
                {
                    a[i][j] = a[j][i];
                }
            }
             return a;
    }

    auto createb( int r, int c, int m, std::vector<std::vector<float>> &volt, std::vector<std::vector<int>> &mcurrent)
    {
        std::vector<float> b;
        int i, j, k;
      //  std::vector<float> b;
        //initialise b to 0
        b=vector<float> (m, 0);

        for(k = 1; k < c-1; k += r+1)
        {
            for(i = 0; i < r; i++)
            {
                for(j = 0; j < r; j++)
                {
                    if(mcurrent[i][j+k]==1 && volt[i][j]!=0)
                    {
                        b[k/r]+= volt[i][j];
                    }
                }
            }
        }
        return b;

    }
    template <typename Matrix>
    void print_matrix(Matrix const &m)
    {
        for (auto const &row : m)
        {
            using T = typename std::iterator_traits<decltype(begin(row))>::value_type; //iterator_traits allowed me to get rid of the print_matrix specialization of C-style array
            copy(begin(row), end(row), ostream_iterator<T>(std::cout, " "));
            std::cout << "\n";
        }
    }

    template <typename Vector>
    void print_vector(Vector const &v)
    {
            using T = typename std::iterator_traits<decltype(begin(v))>::value_type;
            copy(begin(v), end(v), ostream_iterator<T>(std::cout, " "));
            std::cout << "\n";
    }
};


#endif //AUTOCIRCUIT_MESH_H
