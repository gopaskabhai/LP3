DAA Lab C++ Programs

Assignments included:
1. Fibonacci numbers and step count
2. Job sequencing with deadlines (greedy)
3. Fractional knapsack (greedy)
4. 0/1 knapsack (dynamic programming)
5. Binomial coefficients (dynamic programming)
6. 8-Queens using backtracking with first queen position entered by user
Mini project: Sequential merge sort vs multithreaded merge sort, timing and sorted/reverse/random input tests

Compile a file with g++ -std=c++17 filename.cpp -o program
Run with ./program (Windows: program.exe)
For multithreaded merge sort on Linux/macOS, compile with:
g++ -std=c++17 -O2 -pthread mini_project_merge_sort_multithreaded.cpp -o merge_sort
On Windows MinGW, use the thread support available in your compiler; command commonly is:
g++ -std=c++17 -O2 -pthread mini_project_merge_sort_multithreaded.cpp -o merge_sort.exe

The merge sort program uses a thread-depth limit and a size threshold to avoid creating too many threads. Timings vary by machine. For a meaningful comparison, use a large n and run several times.
