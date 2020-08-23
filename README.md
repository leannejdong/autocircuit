# Network automation and simulation: a graph theoretics approach
### A hobby  high school physic C++ project (with warnings about Eigen)

* cycles detection

  * construction of spanning tree (completed)
  
  * construction of independent loops (completed)

* network simulation
  
  * set loop oritentation (completed)
  
  * set equation system (completed)
  
  * Find branch current and voltage (completed)
  
## Building

### Dependency required

- CMake = 3.16

- Clang >= 10.0 (the use of `auto` in parameter declartion only available with '-fconcepts')

### Linux

#### Build and run from terminal
```
git clone https://github.com/leannejdong/autocircuit.git
cd autocircuit/
cmake -Bbuild -H.
cmake --build build --target all
build/circuit
```

#### Quick build without CMake
```
clang++-10 -std=c++2a main.cpp -Wall -Wextra  -o main
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

#### Build from CLion

Open the project directory and hit `shift-f10`

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


