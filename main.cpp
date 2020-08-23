#include "mesh.h"
#include "graph.h"
#include "eigenconv.h"
#include "eigenutil.h"
#include "sovr.h"
#include "tension.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include </usr/include/eigen3/Eigen/Dense>
#include </usr/include/eigen3/Eigen/LU>

constexpr auto ERROR = 1.0e-3;
static void printIndmTo(std::ostream &stream, const vector<vector<int>> &indm);
static void printIndm(const vector<vector<int>> &indm);

int main()
{
    graph g(11);
    //Graph g(11);
    g.addEdge(0, 1);
    g.addEdge(0, 6);
    g.addEdge(0, 7);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(2, 7);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(4, 8);
    g.addEdge(5, 6);
    g.addEdge(7, 8);
    g.addEdge(8, 9);
    g.addEdge(9, 10);
    g.addEdge(10, 6);

    std::vector<int> cycles;
    g.Gotlieb123(back_inserter(cycles));
    vector<vector<bool>> adjMatrix = g.getAdjMat();
    std::ofstream of("cycles.data");
    print_cycles(begin(cycles), end(cycles), cout);
    print_cycles(begin(cycles), end(cycles), of);

  //  g.printMat();

    mesh mesh1;

  //  mesh1.print_matrix(adjMatrix);
  //  cout << "\n";

//    vector<vector<int>> foo = {{0, 1, 0},
//                               {1, 0, 1},
//                               {1, 0, 0}};
//    mesh1.print_matrix(foo);
//    cout << "\n";
//    mesh1.setdircur(3, 4, foo);
//    cout << "\n";
//    mesh1.print_matrix(foo);

//    static const char *circuit_matrix_text =
//            "0 1 0 0 0 0 1 1 0 0 0\n"
//            "1 0 1 0 0 0 0 0 0 0 0\n"
//            "0 1 0 1 0 0 0 1 0 0 0\n"
//            "0 0 1 0 1 0 0 1 0 0 0\n"
//            "0 0 0 1 0 1 0 0 0 0 0\n"
//            "0 0 0 0 1 0 1 0 0 0 0\n"
//            "1 0 0 0 0 1 0 0 0 0 1\n"
//            "1 0 1 0 0 0 0 0 1 0 0\n"
//            "0 0 0 0 1 0 0 1 0 1 0\n"
//            "0 0 0 0 0 0 0 0 1 0 1\n"
//            "0 0 0 0 0 0 1 0 0 1 0\n";
//
//    std::istringstream stream(circuit_matrix_text);
//    vector<vector<int>> matrix;
//
//    std::string line;
//    while (std::getline(stream, line)) {
//        std::istringstream line_stream(line);
//
//        int element;
//        vector<int> row;
//
//        while (line_stream >> element) {
//            row.push_back(element);
//        }
//
//        matrix.push_back(row);
//    }

    // vector<vector<bool>> treeAdjMatBool = makeBoolMatrixFromIntMatrix(treeAdjMat);
    //mesh1.print_matrix(adjMatrix);
    //int r = adjMatrix.size();
    int r, m;
    std::string dummy;
    std::ifstream file_stream("inputmat.dat");
    if (!file_stream) {
        std::cerr << "Unable to open inputmat.dat\n";
        return EXIT_FAILURE;
    }

    if (!file_stream) {
        cout << "\nThe data file could not be opened!\n";
    }
    else {
        cout << "\nData file opened successfully!\n";
    }
    std::getline(file_stream, dummy);
    std::getline(file_stream, dummy);
    file_stream >> r;

    for (auto &row : adjMatrix)
    {
        row.resize(r);
    }
    std::getline(file_stream, dummy);
    std::getline(file_stream, dummy);
    for (auto &row : adjMatrix)
    {
        for(auto &&element : row)
        {
            bool value;
            file_stream >> value;
            element = value;
        }
    }

    mesh1.print_matrix(adjMatrix);

//    std::getline(file_stream, dummy);
//    for(int i=0; i<r; i++)            /*I scan the numeric values ​​and assign them to circuit*/
//        for(int j=0; j<r; j++)
//            file_stream >> adjMatrix[i][j];



    vector<vector<int>> mcurrent = g.Gotlieb4(r, &m, adjMatrix);
    //printIndm(output);
    std::ofstream outfile1("indm.txt");
    printIndmTo(outfile1, mcurrent);
    mesh1.print_matrix(mcurrent);

    int c = r*m + m + 1;
    std::cout << c << " \n";
    std::cout << r << " \n";
    mesh1.setdircur(r, c, mcurrent);
    mesh1.print_matrix(mcurrent);


    vector<vector<float>> res(r);
    for (auto &row : res)
    {
        row.resize(r);
    }

    std::getline(file_stream, dummy);
    std::getline(file_stream, dummy);

    for(auto &row : res)
    {
        for(auto &element : row)
        {
          //  bool value;
            file_stream >> element;
          //  element = value;
        }
    }
    mesh1.print_matrix(res);
    // create matrix A for the mesh method
    vector<vector<float>> a;
    a = mesh1.createmat(m, r, mcurrent, res, c);
    mesh1.print_matrix(a);

    MatrixXf A = makeEigenMatrixFromVectors(a);
    cout << "Here is the matrix A:\n" << A << endl;

//
    vector<vector<float>> volt(r);
    for (auto &row : volt)
    {
        row.resize(r);
    }

    std::getline(file_stream, dummy);
    std::getline(file_stream, dummy);

    for(auto &row : volt)
    {
        for(auto &element : row)
        {
            file_stream >> element;
        }
    }
    mesh1.print_matrix(volt);

    vector<float> b(m);

   // for (auto i : b) {cout << i << endl;}
    mesh1.createb(b, r, c, m, volt, mcurrent);

   // for (auto i : b) {cout << i << endl;}

    vector<float> x(m);

/*I save b for the next few times*/
    mesh1.createb(x, r, c, m, volt, mcurrent);
    for (auto i : x) {cout << i << endl;}

    VectorXf eigenb = makeEigenVectorFromVectors(b);
    cout << "Here is the rhs b:\n" << eigenb << endl;
    VectorXf eigenx = makeEigenVectorFromVectors(x);
    cout << "Here is the  x:\n" << eigenx << endl;
    VectorXf soln = A.colPivHouseholderQr().solve(eigenb);
    cout << "The solution is:\n" << soln << endl;
    vector<float> vec_soln = makeVectorsFromEigen(soln);

    vector<float> p(m);

    mprove(A, eigenb);

    cout << m << endl;

//    int j = 0;
//    while(j == 0)
//    {
//        for (int i = 0; i < m; i++)
//        {
//            for (int j = 0; j < m; j++)
//                p[i] += (a[i][j]*x[j]);
//            if (p[i] - b[i] <= ERROR && -ERROR <= p[i] - b[i])
//            {
//                cout << "\nThe result relative to the system of equations of the mesh method is corrected within the third decimal place.\n" << i + 1 << endl;
//                j = 1;
//            }
//            else
//            {
//                mprove(A, eigenb);
//            }
//        }
//    }
//    for (auto i : x) {cout << i << endl;}

    vector<vector<float>> current;

    current = sovr(c, r, mcurrent, vec_soln, m);
//
    mesh1.print_matrix(current);
    std::ostringstream output_stream;
    tension(r, res, current, volt, output_stream);
    std::string output = output_stream.str();
    std::cerr << output << "\n";
    std::ofstream file_stream1("tension.dat");
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

static void printIndm(const vector<vector<int>> &indm)
{
    std::ostream &stream = std::cerr;

    for (size_t i=0; i!=indm.size(); ++i) {
        for (size_t j=0; j!=indm[i].size(); ++j) {
            if (j != 0) {
                stream << "  ";
            }
            stream << indm[i][j];
        }
        stream << "\n";
    }
}

//static vector<vector<bool>> makeBoolMatrixFromIntMatrix(const vector<vector<int>> &indm)
//{
//    vector<vector<bool>> result(indm.size());
//    for (size_t i=0; i!=indm.size(); ++i) {
//        result[i].resize(indm[i].size());
//        for (size_t j=0; j!=indm[i].size(); ++j) {
//            result[i][j] = (indm[i][j] != 0);
//        }
//    }
//    return result;
//}

// C++ automatically throw exception
/*
 * try {
    runAlgorithm();
  }
  catch (std::bad_alloc&) {
    std::cerr << "Ran out of memory\n";
  }
 */