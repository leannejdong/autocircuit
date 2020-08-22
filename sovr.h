#ifndef CIRCUIT_SOVR_H
#define CIRCUIT_SOVR_H
#include <iostream>
#include "eigenutil.h"
#include <vector>

std::vector<std::vector<float> > sovr(int c, int r, std::vector<std::vector<int> > &mcurrent, std::vector<float> &x, int m)
{
    int i, j, k, n;
    using std::vector;
    auto temp = makeMatrix(r, c);
    float sum;

    // multiply the  temp matrix by the values of the mesh currents in each individual mesh
    for (k = 1; k < c-1; k = k + r + 1)
    {
        for ( i = 0; i < r; i++)
        {
            for (j = 0; j < r; j++)
            {
                if(mcurrent[i][j+k] == 1)
                {
                    temp[i][j+k] = x[k/r];
                } else if (mcurrent[i][j+k] == -1)
                {
                    temp[i][j+k] = -x[k/r];
                } else
                {
                    temp[i][j+k] = 0;
                }

            }
        }
    }

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < r + 1 ; j++)
        {
            for (k = 1; k < m; k++)
            {
                temp[i][j] +=  + temp[i][j+k*(r+1)];
            }
        }
    }

    auto current = makeMatrix(r, r);

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < r; j++)
        {
            current[i][j] = temp[i][j+1];
        }
    }

    //Check that the results are correct by applying the law of nodes (and angles)
    // note that the law of the nodes is verified if the sum of the terms on each row is 0 (4.d.p)
    k = 0;
    for (i = 0; i < r; i++)
    {
        sum = 0;
        for (j = 0; j < r; j++)
        {
            sum += current[i][j];
        }
        if (sum >= 0.0001 && sum <= -0.0001)
        {
            std::cout << "\n There was an error while calculating currents\n";
            k = 1;
            break;
        }
    }
    if(k==0)
    {
        std::cout << "\nComputed currents fit the law of nodes\n";
    }

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < r; j++)
        {
            if (current[i][j] <= 0)
            {
                current[i][j] = 0;
            }
        }
    }

    std::ofstream file_stream("current.dat");
    file_stream << "The current matrix stores the currents between nodes in the circuit.\n";
    file_stream << "Entry with (row, column) indexes (A, B) identifies a current flowing from node A to node B, while the corresponding (B, A) entry is empty.\n";
    file_stream << "Current matrix: \n";

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < r - 1; j++)
        {
            file_stream << current[i][j] << "\t";
        }
        file_stream << current[i][r-1] << "\n";
    }

    if (!file_stream)
    {
        std::cout << "Error in writing file current.data\n";
    } else {
        std::cout << "File current.dat has been written\n";
    }

    return current;
}

#endif //CIRCUIT_SOVR_H
