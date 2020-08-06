#include "graph.h"
#include "Mesh.cpp"

template <class ForwardIterator>
void print_cycles(ForwardIterator first, ForwardIterator last);

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
    g.Gotlieb(back_inserter(cycles));
    print_cycles(begin(cycles), end(cycles));

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
}

// Requires a sequence of closed cycles.
template <class ForwardIterator>
void print_cycles(ForwardIterator first, ForwardIterator last)
{
    using T = typename iterator_traits<ForwardIterator>::value_type;
    while (first != last)
    {
        auto const cycle_start = first++;
        first = ++find(first, last, *cycle_start);
        copy(cycle_start, first, ostream_iterator<T>(cout, " "));
        cout << "\n";
    }
}