# Bachelor Project: Parallel Strongly Connected Compontent Decomposition

## Introduction

This repository contains the source code for a bachelor project (15 ECTS) in computer science. The code in here was written to support experiments that we wanted to conduct for our project.

The purpose of the project was to investigate the performance of parallelized algorithms for [strongly connected component (SCC)](https://www.geeksforgeeks.org/strongly-connected-components/) decomposition. The main algorithm used for parallel strongly connected component decomposition is the [Forward-Backward (FWBW) algorithm](https://www.sandia.gov/~apinar/papers/irreg00.pdf). Most state-of-the-art algorithms use the FWBW algorithm as a starting framework, but often bundle changes (such as different partitioning strategies) and auxillary algorithms (such as trimming algorithms or alternate algorithms for pivot selection). So our goal was to test some of the more commonly used changes in isolation and thus, try to independently verify their usefulness.

The purpose of this code is to serve as a benchmarking tool. The `src/` folder includes the algorithms we tested as well as the general setup code. Timers and the like are built in to the code. The implementation of the algorithms found in this repository is therefore **not** optimized for performance and should **not** be used in real-world scenarios.

## Prerequisites

```python
GCC-5.0
OpenMP-5.0
Makefile
SNAP (we have included SNAP in this repository)
GoogleTest
```

## Installation

Get into the root directory, then compile the source code with Makefile
```sh
git clone https://github.com/fhartvigmark/BachelorProj
cd BachelorProj/
make all
```

## Usage

After installation you can run the program by going to the `src/` directory and running the `main` file. You can specify how the the code should be run by using any of the following parameters.
```sh
make src
cd src/
./main arguments
```

A detailed description of the valid input to each argument and default values can be found by calling
```sh
./main -h=
```

An example of using the program with trim level 1, sequential FWBW, max-degree pivot method and 16 workers. Also saving output and showing execution time
```sh
./main -g=graph.edges -t=1 -p=1 -w=16 -out=1 -time=1
```

### Optional parameters:

`[-g=]` Path to graph:
	Example:
	-g=PATH/TO/THE/GRAPH.edges

`[-t=]` Trim levels:

	0 = no trim
	1 = trim-1
	2 = trim-1,2
	3 = trim-1,2,3

`[-p=]` Pivot methods:

	0 = first occurrence
	1 = max degree product
	2 = max degree product with colors
	3 = randomwalk
	4 = max degree with precomputed product

`[-m=]` FWBW method:

	0 = basic FWBW
	1 = FWBW with parallel bfs
	2 = Recursive FWBW

`[-w=]` Specify maximum amount of workers

`[-rand=]` Specify amount of steps for randomwalk

`[-cutoff=]` Specify the recursion depth at which trim will no be used every iteration

`[-steps=]` Specify how often to perform trim after the cutoff depth

`[-h=]` Print help section

	//To print help section:
	-h=true

`[-out=]` Print SCC output file

`[-suffix=]` Append the specified text string to the file name of the output file


`[-timers=]` Print timers:

	0 = no printing
	1 = print to stdout
	2 = print to file
	3 = print compact version to file

`[-debug=]` Print debug information:

	0 = no printing
	1 = print to stdout
	2 = print to file
	3 = print compact version to file

### Graph format

In order to run a graph on with the code found in this repository it needs to be correctly formatted.

**Important:** SCC's are a concept unique to directed graphs. As a result, our code will treat all graphs as directed graphs.

All graphs must be in the form of edge lists. These edge lists must take the form of a single text file (though the exact file format is unimportant). Each line in the text file represents an edge. Each edge must consist of a number followed by a space and then another number. The first number specifies the node id of the from-node and the second specifies the node-id of the to-node.

Thus, `graph.txt` could look like the following:
```
1 2
1 3
2 3
3 4
```
Due to assumptions made in our code it is nescessary that a graph of n nodes is contains nodeID's from 1 to n. 

Since not all publicly available graphs are of this format, we have included `graph_converter.cpp` in the `/tools` folder to fix the enumeration. In order to use this tool the graph you are converting must already otherwise correctly formatted to our specifications (outside of the enumeration). Simply go to the `/tools` folder and run
```
make tools
cd tools/
./graph_converter PATH/TO/THE/GRAPH.txt PATH/TO/THE/CONVERTED_GRAPH.txt
```

## Tests
Unit tests can be performed with the Makefile from the root directory
```sh
make run-test
```

## Authors

[Eske Hoy Nielsen](https://www.github.com/4ever2/)

[Frederik Hartvig-Mærkedahl](https://www.github.com/fhartvigmark/)
