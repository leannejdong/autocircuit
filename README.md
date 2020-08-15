# Network automation and simulation (Under Development!)

* cycles detection

  * construction of spanning tree (completed)
  
  * construction of independent loops (completed)
  
* network simulation
  
  * set loop oritentation (completed)
  
  * set equation system 
  
  * Find branch current and voltage
  
## Building

### Dependency required

- CLion (2020.2)

- CMake >= 3.16

- Clang >= 10.0 (the use of `auto` in parameter declartion only available with '-fconcepts')

### Linux

#### Build and run from terminal
```
git clone https://github.com/leannejdong/autocircuit.git
cd autocircuit
cmake -Bbuild -H.
cmake --build build --target all
cd build
./autocircuit
```

#### Build from CLion

Open the project directory and hit `shift-f10`

## Reference

      @article{tsm2015,
      title={Using graph theory for automated electric circuit solving},
      author={L Toscano, S Stella and E Milotti},
      journal={European Journal of Physics},
      volume={36},
      year={2015}
    }
    
      @article{Gotlieb67,
      title={Algorithms for finding fundamental set of cycles for an undirected linear graph},
      author={C.C. Gotlieb, and D.G. Corneil},
      journal={Communication of ACM},
      volume={10},
      issue = {12},
      year={1967}
    }


