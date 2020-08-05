
#ifndef AUTOCIRCUIT_MESH_H
#define AUTOCIRCUIT_MESH_H
#include "graph.h"
class Mesh
{
private:
    int m, c, **mcurrent;
    std::vector<float> res;
    std::vector<float> volt;
    std::vector<float> current;
    //std::vector<std::vector<bool>> circuit;
    //std::vector<std::vector<int>> mcurrent;
    std::vector<std::vector<double>> a;
    std::vector<int> indx;

public:
    //void setdircur(int r, int c, bool mcurrent);
    void setdircur(int r, int c, int **mcurrent);
    void createmat(int m, int r, int **mcurrent, std::vector<std::vector<float>> res, int c);
    void createb (std::vector<float> b, int r, int c, int m, std::vector<std::vector<float>> volt, int **mcurrent);
    void solnrefine (std::vector<std::vector<float>> a, std::vector<std::vector<float>> alud, int n, std::vector<int> indx, std::vector<float> b, std::vector<float> x);
    float sovr (int c, int r, int **mcurrent, std::vector<float> x, int m);
    void tension (int r, std::vector<std::vector<float>> res, std::vector<std::vector<float>> current, std::vector<std::vector<float>> volt);
    void ludcmp();
};

#endif //AUTOCIRCUIT_MESH_H
