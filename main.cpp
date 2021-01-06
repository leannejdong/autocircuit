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

auto readMatrix(auto &stream, int r)
{
    std::string dummy;
//    vector<vector<float>> vec;
//    for (auto &row : vec)
//    {
//        row.resize(r);
//    }
    std::vector<std::vector<float>> vec(r,std::vector<float>(r));
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
    auto res = readMatrix(stream, r);
    auto volt = readMatrix(stream, r);
    return std::make_tuple(g, res, volt);
}
constexpr auto ERROR = 1.0e-3;
static void printIndmTo(std::ostream &stream, const vector<vector<int>> &indm);
static void printIndm(const vector<vector<int>> &indm);

int main()
{
    std::ifstream file_stream("inputmat.dat"); // create an input file stream
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
    std::cout << "Print graph " << "\n";
    mesh1.print_matrix(G.getAdjMat());
    std::cout << "Print Resistance " << "\n";
    mesh1.print_matrix(R);
    std::cout << "Print Voltage " << "\n";
    mesh1.print_matrix(V);

   // mesh1.print_matrix(G.getAdjMat());
    std::vector<int> cycles;
    G.Gotlieb123(std::back_inserter(cycles));   // SF from here
    std::ofstream of("cycles.data");
    std::cout << "Print Cycles " << "\n";
    print_cycles(std::begin(cycles), std::end(cycles), std::cout);
    print_cycles(std::begin(cycles), std::end(cycles), of);
   // int m;
    auto [m, mcurrent] = G.Gotlieb4(/*G.size(), &m, G.getAdjMat()*/);
//    //printIndm(output);
    std::ofstream outfile1("indm.txt");
    printIndmTo(outfile1, mcurrent);
    std::cout << "Print mcurrent " << "\n";
    mesh1.print_matrix(mcurrent);
    auto c = G.size()*m + m + 1;
   // std::cerr << c << "\n";
    mesh1.setdircur(G.size(), c, mcurrent);
   // std::cerr << G.size() << "\n";
    auto a = mesh1.createmat(m, G.size(),mcurrent, R, c);
    std::cout << "Print A " << "\n";
    mesh1.print_matrix(a);
    MatrixXf A = makeEigenMatrixFromVectors(a);
    cout << "Here is the matrix A:\n" << A << endl;
    auto b = mesh1.createb(G.size(), c, m, V, mcurrent);
    auto x = mesh1.createb(G.size(), c, m, V, mcurrent);
    cout << "Here is the rhs b:\n" << "\n";
    mesh1.print_vector(b);
    cout << "Here is the rhs x:\n" << "\n";
    mesh1.print_vector(x);
    VectorXf eigenb = makeEigenVectorFromVectors(b);
    cout << "Here is the rhs b:\n" << eigenb << "\n";
    VectorXf eigenx = makeEigenVectorFromVectors(x);
    cout << "Here is the  x:\n" << eigenx << endl;
    VectorXf soln0 = A.colPivHouseholderQr().solve(eigenb);
    cout << "The solution is:\n" << soln0 << endl;
    vector<float> vec_soln = makeVectorsFromEigen(soln0);
    Eigen::FullPivLU<Eigen::MatrixXf> lu(A);
    VectorXf soln = lu.solve(eigenb);
    cout << "The solution is : \n" << soln << endl;

    double err2 = computeDifference(A, soln, eigenb).squaredNorm();

    const int max_it = 10;

    std::cout << "Error before: " << std::sqrt(err2) << std::endl;
    int it = 0;
    do {
        // refine solution
        solnrefine(lu, A, eigenb, soln);
        // compute new error
        // could also just use
        err2 = computeDifference(A, soln, eigenb).squaredNorm();
        it++;
        // more for debug
        std::cout << " Error after step: " << it << ": " << std::sqrt(err2) << std::endl;
    } while (err2 > ERROR * ERROR && it < max_it);

    vec_soln = makeVectorsFromEigen(soln);

    auto current = sovr(c, G.size(), mcurrent, vec_soln, m);
    mesh1.print_matrix(current);
    std::ostringstream output_stream;
    voltage(G.size(), R, current, V, output_stream);
    std::string output = output_stream.str();
    std::cerr << output << "\n";
    std::ofstream file_stream1("voltage.dat");
    file_stream1 << output;
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
