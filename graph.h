

#ifndef AUTOCIRCUIT_GRAPH_H
#define AUTOCIRCUIT_GRAPH_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <iterator>
#include <cassert>
#include <fstream>

using namespace std;
class graph {
    int r;
    std::vector<std::vector<bool>> adjMatrix;
    std::vector<std::vector<int>> treeAdjMat;

    int countDifference(auto &treeAdjMat_i, auto &adjMatrix_i)
    {
        int n_differences = 0;
        for(int j=0; j<treeAdjMat_i.size()-1; j++)
            if(treeAdjMat_i[j]!=adjMatrix_i[j])
                ++n_differences;
        return n_differences;
    }

public:
    // Initialize the matrix to zero
    graph(int r) : r(r), adjMatrix(r, std::vector<bool>(r, false)),
                   treeAdjMat(r, std::vector<int>(r)) {}

    void addEdge(int i, int j)
    {
        assert(i >= 0 && i < r && j > 0 && j < r);
        adjMatrix[i][j] = true;
        adjMatrix[j][i] = true;
    }

    void removeEdge(int i, int j)
    {
        assert(i >= 0 && i < r && j > 0 && j < r);
        adjMatrix[i][j] = false;
        adjMatrix[j][i] = false;
    }

    bool isEdge(int i, int j)
    {
        if (i >= 0 && i < r && j > 0 && j < r)
            return adjMatrix[i][j];
        else
            return false;
    }

    template<class OutputIterator>
    void DFSCheckCycle(std::vector<std::vector<int>> &adjMat, size_t u, size_t par, std::vector<bool> &visited,
                       std::vector<int> &parents, size_t source, OutputIterator foundCycle)
    {
        if (visited[u]) {
            if (u == source) {
                //cycle found
                parents[u] = par;
                while (true) {
                    *foundCycle++ = u;
                    u = parents[u];
                    if (u == source) {
                        *foundCycle++ = u;
                        break;
                    }
                }
            }
            return;
        }
        visited[u] = true;
        parents[u] = par;
        for (size_t v = 0; v < adjMat.size(); ++v) {
            if (adjMat[u][v] == 1 && v != parents[u]) {
                DFSCheckCycle(adjMat, v, u, visited, parents, source, foundCycle);
            }
        }
    }


    template<typename OutputIterator>
    OutputIterator Gotlieb123(OutputIterator cycles)
    {
        const auto r = adjMatrix.size();
        // Initialize adjacency matrix for spanning tree
        treeAdjMat = std::vector<std::vector<int>> (r, std::vector<int>(r, 0));
        for (int i = 0; i < r; ++i) {
            for (int j = i; j < r; ++j) {
                if (adjMatrix[i][j] == 1) {
                    treeAdjMat[i][j] = 1;
                    treeAdjMat[j][i] = 1;
                    break;
                }
            }
        }

        // BLOCK 2: Find all connected components
        /* Example: C = [ [ 1 1 1 0 0 1 ],
        *                [ 0 0 0 1 0 0 ],
        *                [ 0 0 0 0 1 0 ] ] */

        std::vector<std::vector<int>> connComponents;
        std::vector<bool> visited(r, false);
        for (int u = 0; u < r; ++u)
        {
            if (visited[u])
                continue;
            std::vector<int> component(r, 0);
            std::stack<int> s;
            s.push(u);
            while (!s.empty())
            {
                int v = s.top();
                visited[v] = true;
                component[v] = 1;
                s.pop();
                for (int w = 0; w < r; w++)
                {
                    if (treeAdjMat[v][w] && !visited[w])
                    {
                        s.push(w);
                    }
                }
            }
            connComponents.push_back(component);
        }
        // Now focus on finding cycle base
        /* Block 3: Here the connected components are amalgamated by
        adding appropriate edges to the adjacency matrix B (treeAdjMat)
        Example: edge(2, 5) and (2, 6) are added back to B
        */
        std::vector<bool> checked(r);
        for (auto const &cmpt : connComponents)
            for (int j = 0; j < r; ++j)
                if (cmpt[j] == 1)
                    for (int k = 0; k < r; k++)
                        if (adjMatrix[j][k] == 1 && cmpt[k] == 0 && !checked[k])
                        {
                            treeAdjMat[k][j] = 1;
                            treeAdjMat[j][k] = 1;
                            checked[k] = true;
                        }
        // BLOCK 4
        /* Collect all edges eliminated from the original adjacency matrix to
        build the spanning tree matrix
        */
        std::vector<std::pair<int, int>> eliminatedEdges;
        for (int i = 0; i < r; ++i)
            for (int j = i; j < r; ++j)
                if (treeAdjMat[i][j] !=adjMatrix[i][j])
                    eliminatedEdges.emplace_back(i, j);

        // count how many sides been eliminated to get the spanning tree
        // FINAL: Iterate through each eliminated edge, try adding it into mat B(treeAdjMat)
        // The use DFS to check the cycle, the source node is the first node of the edge
        for (auto edge: eliminatedEdges)
        {
            visited = std::vector<bool>(r, false);
            std::vector<int> parents(r, -1);
            treeAdjMat[edge.first][edge.second] = treeAdjMat[edge.second][edge.first] = 1;
            DFSCheckCycle(treeAdjMat, edge.first, -1, visited, parents, edge.first, cycles);
            treeAdjMat[edge.first][edge.second] = treeAdjMat[edge.second][edge.first] = 0;
        }
        return cycles;
    }

    const std::vector<std::vector<bool>> &getAdjMat() const
    {
        return adjMatrix;
    }

    std::vector<std::vector<int>> Gotlieb4(int r, int *m, std::vector<std::vector<bool>> &adjMatrix)
    {
        *m = 0; int i, j, k, c, nu, done;

        for(i=0; i<r; i++)
        {
            std::vector<int> &treeAdjMat_i = treeAdjMat[i];
            std::vector<bool> &adjMatrix_i = adjMatrix[i];
            //int n_differences = 0;
            assert(r==treeAdjMat_i.size());
            *m += countDifference(treeAdjMat_i,adjMatrix_i);
        }
        int &count = *m;
        count /= 2;
        //count how many sides have to be eliminated to obtain the tree graph = number of independent links
        c = r*count + count + 1;
        std::vector<std::vector<int>> indm(r);
        for (int i = 0; i<r; ++i)
        {
            indm[i].resize(c);
        }
        for (j = 0; j < c-r; j = j+r+1)
            for (i = 0; i<r; i++)
                indm[i][j] = -4;
        for (i = 0; i < r; i++)
            indm[i][c-1]=-5;
        for (k = 1; k < c; k=k+r+1)
            for(i = 0; i < r; i++)
                for(j = 0; j < r; j++)
                    indm[i][j+k] = treeAdjMat[i][j];
        // add the sides at a time
        k = 1;
        for(i = 0; i < r; i++)
            for(j = i+1; j<r; j++)
                if(adjMatrix[i][j]==1 && treeAdjMat[i][j]==0)
                {
                    indm[i][j+k]=1;
                    indm[j][i+k]=1;
                    k = k + r + 1;
                }
        /*I remove the segments that are outside the loop (see drawing)*/
        nu = 0; /*nu is the number one on a line*/
        done=0;
        for(k=1; k<c; k=k+r+1){
            while(done==0){
                done=1;
                for(i=0; i<r; i++){
                    for(j=0; j<r; j++) /*Count how many ones are on a line*/
                        if(indm[i][j+k]==1)
                            nu++;
                    if(nu==1)       /*if there is only one,  make it null*/
                        for(j=0; j<r; j++)    /*I am in the j of 1*/
                            if(indm[i][j+k]==1){
                                indm[i][j+k]=0;
                                indm[j][i+k]=0;
                                done=0;
                            }
                    nu=0;
                }
            }
            done=0;
        }

        return indm;
    }

    void printMat()
    {
        int i, j;
        for (i = 0; i < r; i++ )
        {
            for (j = 0; j < r; j++)
            {
                std::cout << to_string(adjMatrix[i][j]) << " ";
            }
            std::cout << "\t";

            for (j = 0; j < r; j++)
            {
                std::cout << std::to_string(treeAdjMat[i][j]) << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Requires a sequence of closed cycles.
template <class ForwardIterator, class OutputStream>
void print_cycles(ForwardIterator first, ForwardIterator last, OutputStream &os)
{
    using T = typename std::iterator_traits<ForwardIterator>::value_type;
    while (first != last)
    {
        auto const cycle_start = first++;
        first = ++find(first, last, *cycle_start);
        copy(cycle_start, first, std::ostream_iterator<T>(os, " "));
        os << "\n";
    }
}


#endif //AUTOCIRCUIT_GRAPH_H
