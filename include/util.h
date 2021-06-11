//
// Created by leanne on 6/11/21.
//

#ifndef CIRCUIT_UTIL_H
#define CIRCUIT_UTIL_H

inline auto readMatrix(auto &stream, int r)
{
    std::string dummy;
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

inline auto read_model(auto &stream){
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

inline void printIndmTo(std::ostream &stream, const vector<vector<int>> &indm)
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

//inline void printIndm(const vector<vector<int>> &indm)
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
#endif//CIRCUIT_UTIL_H
