# Report tasks
* Redo intro
* Split theory into two sections (related work, prelimenaries)
* Make comparative table in related works section
* Write small paragraph for each algorithm
* Begin implementation section
  * Structure of code
  * Tools and libraries
  * Why we did things how we did them
  * Which things where implemented and why
  * ?Issues and potential areas of improvement?
* Begin experimentation section
  * Real world graph choices
  * Synthetic Graph production
  * Experimental procedure and hardware
  * Results
    * What's best
    * Why (analysis)
    * Recommendations
* Future works
* Acknowledgements
* Conclusion
* Abstract
* Read the fuck out of it

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
   * ~Parallel FWBW~
   * Recursive FWBW
   * ~Parallel Trim~
   * ~Parallel pivot~
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
* ~Trim 1~
* Debug information
* ~Help argument~
* ~More pivot methods~
   * ~In-out degree~
   * ~Rand-walk~
* ~Trim 2-3~
* SCC output structure _(in progress)_
* Graph analysis tool, and plotting
* Testing tool
 Read output files, group by SCC, compare groupings
* ~Improve MAKEFILE~
