# Experiment design
* Trim
  * Which level of Trim is worth using?
    * What is the overhead on trim levels
  * Is it worth is to run trim at every iteration?
* Pivot
  * Is max-degree better than random?
  * Is it worth it to consider changes to the graph when computing max-degree?
  * Is random walk better than strictly random?
* Perfomance overall
  * Is parellization worth it on the graph selection?
  * Is recursive FWBW parellization better than BFS based parellization?
* Graphs
  * Slashdot
  * Standford
  * Google
  * LiveJournal
  * Pattern (Needs to be converted)

## Experiments
* Trim
  * Check parallel scaling?
* Pivot
  * Check parallel scaling?
  * Run all pivot types on all graphs (without trim??) check which SCCs each pivot selects if it selects the largest first.
* Performance overall
  * Check scaling with different amount of threads
  * Run all 3 methods with best pivot and trim 3

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
* ~Fix trim-3 not handling loops correctly~
* ~Random walk iterations command argument~
* ~Timers printing change indentation~
* ~Output suffix argument~
* ~Fix SCC count in debug info also counting empty fw-bw calls also fix missing bfs debug when saving to file~
* tools
  * Synthetic graph generation tool
  * Graph analysis tool
  * OpenMP settings tool
  * ~Graph convert tool~
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
  * ~FWBW~
* BFS
  * **parBFS slow**
  * ~RelaxedSearch BFS-variant~ (needs testing on bigger graphs, also bigger graphs in unit testing)
  * RandomRelaxedSearch BFS-variant (in progress)
* **Optimization**
  * Unsigned integers
  * ~Max degree pivot preprocess.~
    * ~Timer~
  * Trim/Pivot order
  * Hastable internal KeyDat vector (probably not possible)
  * Static vs dynamic
  * BFS FW/BW emptiness check
  * Emptiness check of bfs-remaining when using maxdegree pivot?
  * ~Remove color HashMap~
    * Direct access
  * Replace Snap graph
    * Double CSR structure
    * Convert tool?
  * Replace Snap Queue (in progress)
  * ~Pivot min and max index~
  * ~Trim min and max index~
  * Run Trim based on previous iterations (in progress)

