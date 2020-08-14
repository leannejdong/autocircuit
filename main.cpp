#include "graph.h"
#include "Mesh.cpp"
#include <fstream>
#include <sstream>

using std::vector;
using std::cout;

template <class ForwardIterator, class OutputStream>
void print_cycles(ForwardIterator first, ForwardIterator last, OutputStream &os);
static void printIndmTo(std::ostream &stream, const vector<vector<int>> &indm);
static void printIndm(const vector<vector<int>> &indm);
static vector<vector<bool>> makeBoolMatrixFromIntMatrix(const vector<vector<int>> &indm);

int main()
{
    undirected::Graph g(11);
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
    std::ofstream of("cycles.data");
    print_cycles(begin(cycles), end(cycles), cout);
    print_cycles(begin(cycles), end(cycles), of);

    g.printMat();
    cout << "\n";
    Mesh mesh;

    vector<vector<int>> foo = {{0, 1, 0},
                               {1, 0, 1},
                               {1, 0, 0}};
    print_matrix(foo);
    cout << "\n";
    mesh.setdircur(3, 4, foo);
    cout << "\n";
    print_matrix(foo);

    static const char *circuit_matrix_text =
            "0 1 0 0 0 0 1 1 0 0 0\n"
            "1 0 1 0 0 0 0 0 0 0 0\n"
            "0 1 0 1 0 0 0 1 0 0 0\n"
            "0 0 1 0 1 0 0 1 0 0 0\n"
            "0 0 0 1 0 1 0 0 0 0 0\n"
            "0 0 0 0 1 0 1 0 0 0 0\n"
            "1 0 0 0 0 1 0 0 0 0 1\n"
            "1 0 1 0 0 0 0 0 1 0 0\n"
            "0 0 0 0 1 0 0 1 0 1 0\n"
            "0 0 0 0 0 0 0 0 1 0 1\n"
            "0 0 0 0 0 0 1 0 0 1 0\n";
//
//
//            "0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0\n"
//            "1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
//            "0 1 0 1 0 0 1 0 0 0 0 0 0 0 0 0 0\n"
//            "0 0 1 0 1 0 0 1 0 0 0 0 0 0 0 0 0\n"
//            "0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 0 \n"
//            "1 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0 0\n"
//            "0 0 1 0 0 1 0 1 0 0 1 0 0 0 0 0 0\n"
//            "0 0 0 1 0 0 1 0 1 0 0 1 0 0 0 0 0\n"
//            "0 0 0 0 1 0 0 1 0 0 0 0 1 0 0 0 0 \n"
//            "0 0 0 0 0 1 0 0 0 0 1 0 0 1 0 0 0\n"
//            "0 0 0 0 0 0 1 0 0 1 0 1 0 0 1 0 0\n"
//            "0 0 0 0 0 0 0 1 0 0 1 0 1 0 0 1 0\n"
//            "0 0 0 0 0 0 0 0 1 0 0 1 0 0 0 0 1 \n"
//            "0 0 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0\n"
//            "0 0 0 0 0 0 0 0 0 0 1 0 0 1 0 1 0\n"
//            "0 0 0 0 0 0 0 0 0 0 0 1 0 0 1 0 1\n"
//            "0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1 0\n";

    std::istringstream stream(circuit_matrix_text);
    vector<vector<int>> matrix;

    std::string line;
    while (std::getline(stream, line)) {
        std::istringstream line_stream(line);

        int element;
        vector<int> row;

        while (line_stream >> element) {
            row.push_back(element);
        }

        matrix.push_back(row);
    }
    std::ofstream outfile("indm.txt");
    printIndmTo(outfile, matrix);
    vector<vector<bool>> matrixBool = makeBoolMatrixFromIntMatrix(matrix);
    //cout << matrixBool.size();
    //cout << matrixBool[0].size() << "\n";
    int r = matrixBool.size();
    int m;
    vector<vector<int>> output = g.Gotlieb4(r, &m, matrixBool);
    printIndm(output);
}
// Requires a sequence of closed cycles.
template <class ForwardIterator, class OutputStream>
void print_cycles(ForwardIterator first, ForwardIterator last, OutputStream &os)
{
    using T = typename iterator_traits<ForwardIterator>::value_type;
    while (first != last)
    {
        auto const cycle_start = first++;
        first = ++find(first, last, *cycle_start);
        copy(cycle_start, first, ostream_iterator<T>(os, " "));
        os << "\n";
    }
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

static vector<vector<bool>> makeBoolMatrixFromIntMatrix(const vector<vector<int>> &indm)
{
    vector<vector<bool>> result(indm.size());
    for (size_t i=0; i!=indm.size(); ++i) {
        result[i].resize(indm[i].size());
        for (size_t j=0; j!=indm[i].size(); ++j) {
            result[i][j] = (indm[i][j] != 0);
        }
    }
    return result;
}

// C++ automatically throw exception
/*
 * try {
    runAlgorithm();
  }
  catch (std::bad_alloc&) {
    std::cerr << "Ran out of memory\n";
  }
 */