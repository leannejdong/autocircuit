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
graph read_graph(std::istream &stream){
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
    return g;
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
    int m;
    mesh mesh1;
    vector<vector<float>> res(r);
    for (auto &row : res)
    {
        row.resize(r);
    }
    std::getline(stream, dummy);
    std::getline(stream, dummy);

    for(auto &row : res)
    {
        for(auto &element : row)
        {
            //  bool value;
            stream >> element;
            //  element = value;
        }
    }

    vector<vector<float>> volt(r);
    for (auto &row : volt)
    {
        row.resize(r);
    }

    std::getline(stream, dummy);
    std::getline(stream, dummy);

    for(auto &row : volt)
    {
        for(auto &element : row)
        {
            stream >> element;
        }
    }
    return std::make_tuple(g, res, volt);
}

//graph read_resistance(std::istream &stream){
//    std::string dummy;
//}
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
    graph g = read_graph(file_stream);
    mesh mesh1;
    mesh1.print_matrix(g.getAdjMat());
    std::vector<int> cycles;
    g.Gotlieb123(std::back_inserter(cycles));   // SF from here
    std::ofstream of("cycles.data");
    print_cycles(std::begin(cycles), std::end(cycles), std::cout);
    print_cycles(std::begin(cycles), std::end(cycles), of);

//    vector<vector<int>> mcurrent = g.Gotlieb4(r, &m, adjMatrix);
//    //printIndm(output);
//    std::ofstream outfile1("indm.txt");
//    printIndmTo(outfile1, mcurrent);
//    mesh1.print_matrix(mcurrent);






}