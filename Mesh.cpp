#include "Mesh.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <vector>

void Mesh::setdircur(int r, int c, int **mcurrent)
{
    using namespace std;
    assert(r > 0);
    assert(c > r);
    assert(mcurrent);
    int k, i, j, n, ii, jj, nloop;
    for(int k = 1; k < c-1; k = k + r + 1) {
        // count the number of links in each mesh
        n = 0;
        ii = -1; //initialize ii to -1 to overwrite it just once
        for (i = 0; i < r; i++) {
            auto const first = mcurrent[i] + k, last = first + r;
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
            auto const first = mcurrent[i] + k, last = first + r;
            n += count(first, last, 1);
        }
        n = n / 2;
        assert(ii != -1);
        mcurrent[jj][ii + k] = -1; //set the symmetric of the first one to -1 that I have found -> oriented the first side
        nloop = 1; // I oriented the first side of the shirt
        // next, look for the next branch of mesh in row jj
        i = jj;
        do {
            for (j = 0; j < r; j++)
                if (mcurrent[i][j + k] == 1) {
                    mcurrent[j][i + k] = -1; // set symmetric element to 1
                    nloop++;
                    i = j;
                    break;
                }
        } while (nloop < n);
    }
}

/* Having found the set of cycles, i.e. the set of independent current loops, we need to set the loop orientation, i.e.
 * the arbitrary direction of current flow. When we set loop orientation we also change the graph from undirected
 * to directed. Hence, adjacency matrix need to be modified accordingly and it will be no longer symmetric */