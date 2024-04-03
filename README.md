# Game of Life

## About the project

Conway's Game of Life, often simply referred to as Life, is a classic cellular automaton conceptualized by the British mathematician John Horton Conway in 1970. As a zero-player game, Life's evolution is solely determined by its initial state, devoid of any further input. Players engage with Life by establishing an initial configuration and then observing how it progresses autonomously. Its computational power extends to being Turing complete, enabling it to simulate diverse phenomena such as universal constructors and Turing machines.

Inspired by this intriguing concept, our project focuses on implementing Conway's Game of Life in the C programming language. Tasked with specific requirements as part of our university coursework, we aimed to adapt the fundamental rules of the game while leveraging C's capabilities to create two distinct versions: a single-process implementation and a multi-process implementation.

### Single-Process Version
- In the single-process rendition of the game, all computations occur within a singular execution thread. This version meticulously adheres to the principles of the Game of Life while operating within the constraints of a single process environment.

### Multi-Process Version
- Contrastingly, the multi-process variant of our implementation harnesses the power of parallelism by spawning multiple child processes to handle distinct sections of the cellular grid. By distributing workload across multiple processes, this version offers potential performance enhancements, especially when dealing with larger grids and complex configurations.

Through this project, we delve into the realms of both sequential and parallel computing paradigms, exploring the nuances of implementing a timeless mathematical concept within the confines of a programming language. Our endeavor not only serves educational purposes but also underscores the versatility and applicability of C in computational domains.

## Getting Started

### Prerequesites

This is was made just with C code, so the only thing that you need is a C compiler

- On Linux

    sudo apt-get update && sudo apt-get install gcc
    
    sudo apt-get install libncurses-dev

- On Windows
    
    If you want to run it on Windows, we recommend installing a compiler via [VScode](https://code.visualstudio.com/docs/cpp/config-mingw) or use [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) to execute it directly on Linux

### Clone
To clone the project, run

    git clone https://github.com/PedroDecolin/repo-name.git

### Usage
Now, just compile the version _single_ or _mult_ that you want

    gcc <c-file>.c -o <exe-file>.exe -lncurses

To run

    ./<exe-file> <pattern>

The avaible patterns are

    0 BLINKER
    1 TOAD
    2 BEACON
    3 PULSAR
    4 PENTADECATHLON
    5 RANDOM

## Troublshooting

Here is a simple list of the possible errors codes that can be generated, and how t solve it

    404 : INVALID PATTERN
        > Patter chosen ins't valid
        < Choose another pattern
    *405 : NO WORKERS
        > There aren't children processes
        < Alter NWORERS (line 29)
    406 : INVALID CELL STATE
        > One cell assumed a impossible value
        < Verify file integrity
    407 : NOT ENOUGH ARGUMENTS
        > Pattern not selcted
        < See "Usage"
    408 : TOO MANY ARGUMENTS
        > The program accepts only one pattern
        < See "Usage"
    *409 : MORE WORKERS THAN CELLS
        > There are more children processes then cells
        < Alter NWORKERS (line 29)
    410 : MISSING IMPORTANT FILE
        > A binary file doesn't exists
        < Compile and execute "file-generator.c"


- Errors with an (*) can only be generated in "multi-process-game.c"

## Contact
Gregor Umbelino | emaildogregor@gmail.com | [GitHub](https://github.com/GregAbacaxi)

Pedro Decolin | ph.filla@gmail.com | [GitHub](https://github.com/PedroDecolin)