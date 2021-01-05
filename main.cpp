#include "mesh.h"
#include "graph.h"
#include "eigenconv.h"
#include "eigenutil.h"
#include "sovr.h"
#include "voltage.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

auto readMatrix(auto &stream)
{
    int r; std::string dummy;
    vector<vector<float>> vec;
    for (auto &row : vec)
    {
        row.resize(r);
    }
    std::getline(stream, dummy);
    std::getline(stream, dummy);
    for(auto &row : vec)
    {
        for(auto &element : row)
        {
            //  bool value;
            stream >> element;
            //  element = value;
        }
    }
    return vec;
}

auto read_model(auto &stream){
    std::string dummy;
    std::getline(stream, dummy);
    std::getline(stream, dummy);
    int r;
    stream >> r;
    std::getline(stream, dummy);
    std::getline(stream, dummy);
    graph g(r);
    for (int i = 0;i != r;++i)
        for (int j = 0;j != r;++j){
            bool value;
            stream >> value;
            if(i <= j && value)
                g.addEdge(i,j);
        }
    vector<vector<float>> res = readMatrix(stream);
    vector<vector<float>> volt = readMatrix(stream);
    return std::make_tuple(g, res, volt);
}

int main()
{
    std::ifstream file_stream("inputmat8.dat"); // create an input file stream
    if (!file_stream) {
        std::cerr << "Unable to open inputmat.dat\n";
        return EXIT_FAILURE;
    }
    if (!file_stream) {
        std::cout << "\nThe data file could not be opened!\n";
    }
    else {
        std::cout << "\nData file opened successfully!\n";
    }
    auto[G, R, V] = read_model(file_stream);
    mesh mesh1;
    mesh1.print_matrix(G.getAdjMat());
    std::vector<int> cycles;
    G.Gotlieb123(std::back_inserter(cycles));   // SF from here
    std::ofstream of("cycles.data");
    print_cycles(std::begin(cycles), std::end(cycles), std::cout);
    print_cycles(std::begin(cycles), std::end(cycles), of);

//    vector<vector<int>> mcurrent = g.Gotlieb4(r, &m, adjMatrix);
//    //printIndm(output);
//    std::ofstream outfile1("indm.txt");
//    printIndmTo(outfile1, mcurrent);
//    mesh1.print_matrix(mcurrent);






}