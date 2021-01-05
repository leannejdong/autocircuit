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
constexpr auto ERROR = 1.0e-3;
static void printIndmTo(std::ostream &stream, const vector<vector<int>> &indm);
static void printIndm(const vector<vector<int>> &indm);

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
    auto [G, R, V] = read_model(file_stream);

    mesh mesh1;
    mesh1.print_matrix(G.getAdjMat());
    mesh1.print_matrix(R);
//    mesh1.print_matrix(V);

   // mesh1.print_matrix(G.getAdjMat());
    std::vector<int> cycles;
    G.Gotlieb123(std::back_inserter(cycles));   // SF from here
    std::ofstream of("cycles.data");
    print_cycles(std::begin(cycles), std::end(cycles), std::cout);
    print_cycles(std::begin(cycles), std::end(cycles), of);
   // int m;
    auto [m, mcurrent] = G.Gotlieb4(/*G.size(), &m, G.getAdjMat()*/);
//    //printIndm(output);
    std::ofstream outfile1("indm.txt");
    printIndmTo(outfile1, mcurrent);
    mesh1.print_matrix(mcurrent);
    auto c = G.size()*m + m + 1;
    std::cerr << c << "\n";
    mesh1.setdircur(G.size(), c, mcurrent);
    std::cerr << G.size() << "\n";
//    auto a = mesh1.createmat(m, G.size(),mcurrent, R, c);
//    mesh1.print_matrix(a);

}

static void printIndmTo(std::ostream &stream, const vector<vector<int>> &indm)
{
    //ostream &stream = cerr;
    for (size_t i=0; i!=indm.size(); ++i)
    {
        for (size_t j=0; j!=indm[i].size(); ++j)
        {
            if(j != 0)
            {
                stream << " ";
            }
            stream << indm[i][j];
        }
        stream << "\n";
    }
}

//static void printIndm(const vector<vector<int>> &indm)
//{
//    std::ostream &stream = std::cerr;
//
//    for (size_t i=0; i!=indm.size(); ++i) {
//        for (size_t j=0; j!=indm[i].size(); ++j) {
//            if (j != 0) {
//                stream << "  ";
//            }
//            stream << indm[i][j];
//        }
//        stream << "\n";
//    }
//}
