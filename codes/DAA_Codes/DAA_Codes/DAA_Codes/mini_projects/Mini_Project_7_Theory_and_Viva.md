# Mini Project 7: Sequential & Multithreaded Matrix Multiplication

## 1. Problem Statement
Write a program to implement matrix multiplication. Also implement multithreaded matrix multiplication with either one thread per row or one thread per cell. Analyze and compare their performance.

---

## 2. Theoretical Concepts & Architecture

Matrix multiplication between two $N \times N$ matrices $A$ and $B$ produces an $N \times N$ matrix $C$, where each cell is computed as the dot product:
$$C[i][j] = \sum_{k=0}^{N-1} A[i][k] \times B[k][j]$$

### Parallelization Paradigms:
1. **Sequential Matrix Multiplication**:
   - Uses three nested loops: $i$ (rows), $j$ (columns), and $k$ (inner dot product).
   - Single-threaded execution runs on a single CPU core.
   - Operations: $N^3$ multiplications and $N^3 - N^2$ additions.

2. **Multithreaded: One Thread per Row**:
   - Spawns $N$ concurrent threads.
   - Thread $i$ computes all elements of row $i$: $C[i][0 \dots N-1]$.
   - **Advantage**: Good cache locality across row $A[i]$, zero write race conditions because every thread writes to its own distinct memory row.
   - **Thread creation count**: Exactly $N$ threads.

3. **Multithreaded: One Thread per Cell (or Partitioned Block)**:
   - Theoretically, each cell $C[i][j]$ is completely independent.
   - For small $N$ ($N \le 20$), spawning $N^2$ threads works directly.
   - For larger $N$ (e.g., $N=200$), spawning $40,000$ OS threads creates massive context-switching overhead and thread exhaustion. Therefore, modern implementations partition the $N^2$ cells across a fixed pool of hardware worker threads (matching CPU core count).

---

## 3. Time & Space Complexity Analysis

| Approach | Time Complexity | Auxiliary Space | Thread Overhead |
| :--- | :--- | :--- | :--- |
| **Sequential** | $\mathcal{O}(N^3)$ | $\mathcal{O}(N^2)$ | None |
| **Thread-per-Row** | $\mathcal{O}\left(\frac{N^3}{P}\right)$ | $\mathcal{O}(N^2) + \mathcal{O}(N)$ | Spawns $N$ OS threads |
| **Thread-per-Cell (Direct)** | $\mathcal{O}\left(\frac{N^3}{P}\right)$ | $\mathcal{O}(N^2) + \mathcal{O}(N^2)$ | Spawns $N^2$ OS threads (heavy overhead) |
| **Thread-per-Block / Pool** | $\mathcal{O}\left(\frac{N^3}{P}\right)$ | $\mathcal{O}(N^2) + \mathcal{O}(P)$ | Spawns $P$ threads ($P = \text{hardware cores}$) |

*where $P$ is the number of logical CPU cores available.*

---

## 4. Alternative Algorithms
1. **Strassen's Matrix Multiplication**:
   - Divide-and-conquer strategy using 7 recursive multiplications instead of 8.
   - Time Complexity: $\mathcal{O}(N^{\log_2 7}) \approx \mathcal{O}(N^{2.807})$.
2. **Coppersmith-Winograd / Laser Method**:
   - Theoretical fastest algorithms ($\mathcal{O}(N^{2.371})$), but large constant factors make them impractical for standard matrices.
3. **Cache-Aware Tiled (Block) Matrix Multiplication**:
   - Reorders loops to exploit L1/L2 CPU cache lines, dramatically reducing memory misses without threading.

---

## 5. Viva / External Questions & Answers

**Q1: Why is thread-per-row generally faster than thread-per-cell in practical systems?**
> **Answer**: Creating and destroying an operating system thread incurs kernel overhead (allocating a call stack, CPU context switching). Creating $N^2$ threads for large $N$ wastes more time managing threads than multiplying numbers. Thread-per-row creates only $N$ threads and has superior CPU L1 cache spatial locality when reading row $A[i]$.

**Q2: Is mutex synchronization needed when computing $C[i][j]$?**
> **Answer**: No! Since every thread writes to a mutually exclusive memory location in $C$ (either a unique row or a unique cell) and only reads from shared matrices $A$ and $B$, there is no race condition, eliminating the need for locks or mutexes.

**Q3: What is False Sharing in multithreaded matrix operations?**
> **Answer**: False sharing occurs when threads on different CPU cores write to independent variables that reside within the same 64-byte CPU cache line. This forces cache invalidation and stalls the CPU. Thread-per-row minimizes false sharing compared to naive fine-grained cell threading.

**Q4: What is Amdahl's Law in the context of multithreaded matrix multiplication?**
> **Answer**: Amdahl's Law states that maximum speedup is bounded by the sequential portion of the program: $S = \frac{1}{(1-p) + \frac{p}{P}}$, where $p$ is the parallel fraction and $P$ is core count. Memory bus bandwidth and thread initialization limit the ideal linear $P$-fold speedup.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 -O2 -pthread mini_project_7_matrix_multithreaded.cpp -o mp7
./mp7
```
**Sample Input:**
```text
4
1
1 2 3 4
5 6 7 8
9 1 2 3
4 5 6 7
1 0 0 0
0 1 0 0
0 0 1 0
0 0 0 1
```
*(Multiplies with 4x4 Identity matrix, resulting in original Matrix A).*
