# Network automation and simulation: a graph theoretics approach

 * network automation 

  * cycles detection

  * construction of spanning tree (completed)
  
* network simulation
  
  * set loop orientation (completed)
  
  * set equation system (completed)
  
  * Find branch current and voltage (completed)
  
## Building

### Dependency required

- CMake = 3.16

- Clang >= 10.0 (the use of `auto` in parameter declaration only available with '-fconcepts')

- Eigen3.3

### Linux 

#### Build and run from terminal
```
git clone https://github.com/leannejdong/autocircuit.git
cd autocircuit/
cmake -Bbuild -H.
cmake --build build --target all
build/circuit
```

#### Quick build without Make (some warnings about eigen)
```
clang++-10 -std=c++2a main.cpp -Wall -Wextra  -o circuit -isystem /usr/include/eigen3
```

#### Build and run with gdb debugger
```
cd autocircuit
cp inputmat.dat cmake-build-debug/
cd autocircuit/cmake-build-debug/
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-10 -DCMAKE_CXX_COMPILER=clang++-10 ..
make
gdb ./circuit
run
```

#### Build from CLion (Best choice for windows user)

Open the project directory and hit `shift-f10`

## Future development

* Test on solving some practical use case circuits.

* Could move to Boost Graph Library for more efficient graph manipulation.

* Could be prettier with more generics.

## Reference/Acknowledgement

      @article{tsm2015,
      title={Using graph theory for automated electric circuit solving},
      author={L Toscano, S Stella and E Milotti},
      journal={European Journal of Physics},
      volume={36},
      year={2015}
    }
    
      @article{Gotlieb67,
      title={Algorithms for finding fundamental set of cycles for an undirected linear graph},
      author={C.C. Gotlieb, and D.G. Corneil}
      journal={Communication of ACM},
      volume={10},
      issue = {12},
      year={1967}
    }


