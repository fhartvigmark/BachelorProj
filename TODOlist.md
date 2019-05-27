# Report tasks
* ~Redo intro~
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
* **Comments**
* **Code cleanup**
* ~Refactor pivot to have sequential and parallel public method like trim~
* tools
  * Synthetic graph generation tool
  * Graph analysis tool
  * OpenMP settings tool
  * ~Testing tool~
* Test on big graphs
* Debug information
  * Verbose setting
  * FWBW-tree
* Threads
  * ~Max threads option~ (Not fully tested)
  * Dynamic thread allocation / thread pool
* Output folder
* Graph folder
* Testing plan
* Make code consistent
  * ~Trim~
  * ~Pivot~
  * ~BFS~ (investigate parBFS)
  * FWBW
* BFS
  * parBFS slow
  * ~RelaxedSearch BFS-variant~ (needs testing on bigger graphs, also bigger graphs in unit testing)
  * RandomRelaxedSearch BFS-variant (in progress)
* Optimization
  * Max degree pivot preprocess.
  * Trim/Pivot order
  * Hashtable pack
  * Hastable internal KeyDat vector (probably not possible)
  * Static vs dynamic
  * BFS FW/BW emptiness check
  * Emptiness check of remaining when using maxdegree pivot?

