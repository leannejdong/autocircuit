#include "Mesh.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <vector>

template <typename MultiArray2d>
void Mesh::setdircur(int r, int c, MultiArray2d &mcurrent)
{
    using namespace std;
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

template <typename Matrix>
void print_matrix(Matrix const &m)
{
    for (auto const &row : m)
    {
        using T = typename iterator_traits<decltype(begin(row))>::value_type; //iterator_traits allowed me to get rid of the print_matrix specialization of C-style array
        copy(begin(row), end(row), ostream_iterator<T>(cout, " "));
        cout << "\n";
    }
}

/* Having found the set of cycles, i.e. the set of independent current loops, we need to set the loop orientation, i.e.
 * the arbitrary direction of current flow. When we set loop orientation we also change the graph from undirected
 * to directed. Hence, adjacency matrix need to be modified accordingly and it will be no longer symmetric */