# Bachelor Project: Parallel Strongly Connected Compontent Decomposition

## Introduction

This repository contains the source code for a bachelor project (15 ECTS) in computer science. The code in here was written to support experiments that we wanted to conduct for project.

The purpose of the project was to investigate the performance of parallelized algorithms for [strongly connected component (SCC)](https://www.geeksforgeeks.org/strongly-connected-components/) decomposition. The main algorithm used for parallel strongly connected component decomposition is the [Forward-Backward (FWBW) algorithm](https://www.sandia.gov/~apinar/papers/irreg00.pdf). Most state-of-the-art algorithms use the FWBW algorithm as a starting framework, but often bundle changes (such as different partitioning strategies) and auxillary algorithms (such as trimming algorithms or alternate algorithms for pivot selection). So our goal was to test some of the more commonly used changes in isolation and thus, try to independently verify their usefulness.

The purpose of this code is to serve as a benchmarking tool. The "src/" folder includes the algorithms we tested as well as the general setup code. Timers and the like are built in to the code. The implementation of the algorithms found in this repository is therefore **not** optimized for performance and should **not** be used in real-world scenarios.

## Prerequisites

```javascript
GCC-5.0
OpenMP-5.0
Makefile
```

## Installation

Get into the root directory, then compile the source code with Makefile
```sh
make all
```

