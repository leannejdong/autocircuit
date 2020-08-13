#ifndef CPPAUTOCIRCUIT_GRAPH_H
#define CPPAUTOCIRCUIT_GRAPH_H
//#include </usr/include/boost/compute/iterator/zip_iterator.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <iterator>
#include <cassert>
using namespace std;
using std::vector;

namespace undirected {
    class Graph {
    private:
        int nodeCount;
        vector<vector<bool>> adjMatrix;
        vector<vector<int>> treeAdjMat;

        int countDifference(auto &treeAdjMat_i, auto &adjMatrix_i)
        {
            int n_differences = 0;
            for(int j=0; j<treeAdjMat_i.size()-1; j++)
                if(treeAdjMat_i[j]!=adjMatrix_i[j])
                    ++n_differences;
            return n_differences;
        }

    public:
        // Initialize member variables and initialize the matrix to zero
        Graph(int nodeCount) : nodeCount(nodeCount), adjMatrix(nodeCount, vector<bool>(nodeCount, false)),
                               treeAdjMat(nodeCount, vector<int>(nodeCount)) {}

        void addEdge(int i, int j)
        {
            if (i >= 0 && i < nodeCount && j >0 && j < nodeCount) {
                adjMatrix[i][j] = true;
                adjMatrix[j][i] = true;
            }
        };

        void removeEdge(int i, int j)
        {
            if (i >= 0 && i < nodeCount && j > 0 && j < nodeCount) {
                adjMatrix[i][j] = false;
                adjMatrix[j][i] = false;
            }
        };

        bool isEdge(int i, int j)
        {
            if (i >= 0 && i < nodeCount && j > 0 && j < nodeCount)
                return adjMatrix[i][j];
            else
                return false;
        };

        template <typename OutputIterator>
        void DFSCheckCycle(vector<vector<int>> &adjMat, int u, int par, vector<bool> &visited,
                                  vector<int> &parents, int source, OutputIterator &foundCycle)
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
            for (int v = 0; v < adjMat.size(); ++v) {
                if (adjMat[u][v] == 1 && v != parents[u]) {
                    DFSCheckCycle(adjMat, v, u, visited, parents, source, foundCycle);
                }
            }
        };

        template <typename OutputIterator>
        OutputIterator Gotlieb123(OutputIterator cycles)
        {
            const auto nodeCount = adjMatrix.size();
            // Initialize adjacency matrix for spanning tree
            treeAdjMat = vector<vector<int>> (nodeCount, vector<int>(nodeCount, 0));
            for (int i = 0; i < nodeCount; ++i) {
                for (int j = i; j < nodeCount; ++j) {
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

            vector<vector<int>> connComponents;
            vector<bool> visited(nodeCount, false);
            for (int u = 0; u < nodeCount; ++u)
            {
                if (visited[u])
                    continue;
                vector<int> component(nodeCount, 0);
                std::stack<int> s;
                s.push(u);
                while (!s.empty())
                {
                    int v = s.top();
                    visited[v] = true;
                    component[v] = 1;
                    s.pop();
                    for (int w = 0; w < nodeCount; w++)
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
            vector<bool> checked(nodeCount);
            for (auto const &cmpt : connComponents)
                for (int j = 0; j < nodeCount; ++j)
                    if (cmpt[j] == 1)
                        for (int k = 0; k < nodeCount; k++)
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
            vector<std::pair<int, int>> eliminatedEdges;
            for (int i = 0; i < nodeCount; ++i)
                for (int j = i; j < nodeCount; ++j)
                    if (treeAdjMat[i][j] !=adjMatrix[i][j])
                        eliminatedEdges.emplace_back(i, j);

            // count how many sides been eliminated to get the spanning tree
            // FINAL: Iterate through each eliminated edge, try adding it into mat B(treeAdjMat)
            // The use DFS to check the cycle, the source node is the first node of the edge
            for (auto edge: eliminatedEdges)
            {
                visited = vector<bool>(nodeCount, false);
                vector<int> parents(nodeCount, -1);
                treeAdjMat[edge.first][edge.second] = treeAdjMat[edge.second][edge.first] = 1;
                DFSCheckCycle(treeAdjMat, edge.first, -1, visited, parents, edge.first, cycles);
                treeAdjMat[edge.first][edge.second] = treeAdjMat[edge.second][edge.first] = 0;
            }
            return cycles;
        };

//        template <typename OutputIterator>
//        OutputIterator Gotlieb4(OutputIterator indm)

        vector<vector<int>> Gotlieb4(int r, int *m, vector<vector<bool>>& adjMatrix)
        {
            *m = 0; int i, j, k, c, nu, done;

            for(i=0; i<r; i++)
            {
                vector<int> &treeAdjMat_i = treeAdjMat[i];
                vector<bool> &adjMatrix_i = adjMatrix[i];
                int n_differences = 0;
                assert(r==treeAdjMat_i.size());
                *m += countDifference(treeAdjMat_i,adjMatrix_i);
            }
            int &count = *m;
            count /= 2;
            //count how many sides have to be eliminated to obtain the tree graph = number of independent links
            c = r*count + count + 1;
            vector<vector<int>> indm(r);
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
                    for(i=0; i<=r-1; i++){
                        for(j=0; j<=r-1; j++) /*Count how many ones are on a line*/
                            if(indm[i][j+k]==1)
                                nu++;
                        if(nu==1)       /*if there is only one,  make it null*/
                            for(j=0; j<=r-1; j++)    /*I am in the j of 1*/
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
// https://blog.galowicz.de/2016/09/04/algorithms_in_iterators/
// https://blog.galowicz.de/2017/07/02/order2_iterator/
// https://godbolt.org/z/1ve8zM


        void printMat()
        {
            int i, j;
            for (i = 0; i < nodeCount; i++ )
            {
                for (j = 0; j < nodeCount; j++)
                {
                    std::cout << std::to_string(adjMatrix[i][j]) << " ";
                }
                std::cout << "\t";

                for (j = 0; j < nodeCount; j++)
                {
                    std::cout << std::to_string(treeAdjMat[i][j]) << " ";
                }

                std::cout << std::endl;
            }
        };
    };


}


#endif //CPPAUTOCIRCUIT_GRAPH_H
