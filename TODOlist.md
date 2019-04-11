# Implementation priority
* ~Improve this very list~
* ~Read a graph~
* ~Make basic framework~
* ~Implement basic BFS~
* Use BFS in basic FW-BW
* Trims
* Parallel BFS

# Implementation notes
* `Snap-4.1/examples/circles/circles.cpp` has useful way of importing graphs
* OpenMP might let us avoid using queues. Need to look into this
* [OpenMP Tasks](http://pages.tacc.utexas.edu/~eijkhout/pcse/html/omp-task.html)
* [OpenMP Tasks 2](https://openmp.org/wp-content/uploads/sc13.tasking.ruud.pdf)
* Color based pivot selection is tedious. We have copied Hong's own method

# Tasks
* OpenMP
* Finish sequential baseline implementation
* Parallel baseline implementation
* Unit tests with gtest
* Trim 1
* Debug information
* More pivot methods
* Trim 2-3
* SCC output structure
* Graph analysis tool, and plotting
