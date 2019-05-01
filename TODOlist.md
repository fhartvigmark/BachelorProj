# Implementation priority
* ~Improve this very list~
* ~Read a graph~
* ~Make basic framework~
* ~Implement basic BFS~
* ~Use BFS in basic FW-BW~
* Trims
* ~Parallel BFS~

# Implementation notes
* `Snap-4.1/examples/circles/circles.cpp` has useful way of importing graphs
* OpenMP might let us avoid using queues. Need to look into this
* [OpenMP Tasks](http://pages.tacc.utexas.edu/~eijkhout/pcse/html/omp-task.html)
* [OpenMP Tasks 2](https://openmp.org/wp-content/uploads/sc13.tasking.ruud.pdf)
* Color based pivot selection is tedious. We have copied Hong's own method

# Tasks
* ~OpenMP~
* ~Finish sequential baseline implementation~
* Parallel baseline implementation _(in progress)_
   * ~Parallel BFS~
   * Parallel FWBW
   * Parallel Trim
   * Parallel pivot
* Unit tests with gtest
    * ~Pivot tests~
        * Empty graph
        * No matching color
        * More than one Matching color
    * ~Enhanced graph test~
        * Colorgen
        * Hashmap
            * All nodes present
            * All nodes 0
    * Test on big graphs
* Trim 1 _(in progress)_
* Debug information
* More pivot methods
   * ~In-out degree~
   * Rand-walk
* Trim 2-3
* SCC output structure
* Graph analysis tool, and plotting
* Improve MAKEFILE
