# Mini Project 8: Sequential vs Multithreaded Merge Sort Analysis

## 1. Problem Statement
Implement merge sort and multithreaded merge sort. Compare the time required by both algorithms. Also analyze the performance of each algorithm for the best case and the worst case.

---

## 2. Theory & Core Concepts

Merge Sort is an archetype of the **Divide-and-Conquer** algorithm design paradigm:
1. **Divide**: Split the array into two halves at index $mid = left + (right - left) / 2$.
2. **Conquer**: Recursively sort both sub-arrays.
3. **Combine**: Merge the two sorted sub-arrays into a single contiguous sorted array using an auxiliary buffer.

### Best Case vs Worst Case in Merge Sort:
- **Best Case (Already Sorted Array)**:
  - During the merge step, all elements of the left sub-array are smaller than the first element of the right sub-array.
  - The loop finishes early after only $n/2$ comparisons.
  - Number of comparisons: $\approx \frac{n}{2} \log_2 n$.
  - Time complexity is still strictly **$\Theta(n \log n)$** because recursive splitting and data copying take linear time at every level.
- **Worst Case (Adversarial Alternating Array)**:
  - Elements in left and right sub-arrays strictly alternate in magnitude (e.g., $L = [1, 3, 5]$, $R = [2, 4, 6]$).
  - Every single element must be compared before being placed in the merged array.
  - Number of comparisons: $n - 1$ per merge level, totaling $\approx n \log_2 n - n + 1$.
  - Time complexity is strictly **$\Theta(n \log n)$**.

---

## 3. Multithreading Strategy & Concurrency Limits

1. **Fork-Join Parallelism**:
   - The left half sort is dispatched to a background thread: `std::thread(sort, left, mid)`.
   - The right half is processed on the parent thread: `sort(mid + 1, right)`.
   - The parent calls `thread.join()` to synchronize before merging.
2. **Cutoff Threshold & Depth Control**:
   - Naive recursion spawns $2^k$ threads, quickly exhausting the OS process table and crashing the program.
   - We enforce two guards:
     - `MAX_THREAD_DEPTH = 3` (limits concurrent active leaf threads to $2^3 = 8$, matching hardware CPU cores).
     - `THREAD_CUTOFF_SIZE = 10,000` (subproblems smaller than 10,000 elements run sequentially to prevent thread creation latency from overshadowing sorting time).

---

## 4. Time & Space Complexity Analysis

| Metric | Sequential Merge Sort | Multithreaded Merge Sort | Explanation |
| :--- | :--- | :--- | :--- |
| **Best Case Time** | $\Theta(n \log n)$ | $\Theta\left(\frac{n \log n}{P}\right) + \mathcal{O}(n)$ | Merge step requires copying even if comparisons terminate early. |
| **Average Case Time** | $\Theta(n \log n)$ | $\Theta\left(\frac{n \log n}{P}\right) + \mathcal{O}(n)$ | Logarithmic recursion tree height $\log_2 n$. |
| **Worst Case Time** | $\Theta(n \log n)$ | $\Theta\left(\frac{n \log n}{P}\right) + \mathcal{O}(n)$ | Total comparisons approach $n \log_2 n$. |
| **Space Complexity** | $\mathcal{O}(n)$ | $\mathcal{O}(n) + \mathcal{O}(P \log n)$ | Auxiliary memory needed to merge sub-arrays. |
| **Stability** | **Stable** | **Stable** | Relative order of duplicate elements is preserved. |

*Note: In multithreaded merge sort, the final merge of size $n$ is performed sequentially on one thread, which imposes an $\mathcal{O}(n)$ lower bound in parallel execution time.*

---

## 5. Viva / External Questions & Answers

**Q1: Why is Merge Sort preferable over Quick Sort for linked lists, and vice-versa for arrays?**
> **Answer**:
> - **Linked Lists**: Merge Sort can merge in $\mathcal{O}(1)$ auxiliary space without random access (pointers just need re-linking).
> - **Arrays**: Quick Sort is an in-place sort with superior CPU cache locality and zero dynamic array allocations, whereas Merge Sort requires an extra $\mathcal{O}(n)$ buffer array.

**Q2: Does Merge Sort have different asymptotic time complexities for best, average, and worst cases?**
> **Answer**: No! Unlike Quick Sort (which degrades from $\mathcal{O}(n \log n)$ to $\mathcal{O}(n^2)$), Merge Sort always splits the problem into two equal halves regardless of initial array ordering. Its recurrence relation is always $T(n) = 2T(n/2) + \Theta(n)$, giving $\Theta(n \log n)$ unconditionally.

**Q3: Why don't we create a new thread at every single recursive call?**
> **Answer**: Creating a thread incurs heavy OS overhead (allocating kernel stack, registers, scheduling). When array size drops below a threshold (e.g., 10,000 elements), the time spent creating a thread exceeds the time taken to sort the elements sequentially.

**Q4: Is Merge Sort an in-place sorting algorithm?**
> **Answer**: Standard Merge Sort is **not** in-place because the merge procedure requires an auxiliary array of size $n$. While in-place merge variants exist (like block merge sort), they increase constant factor runtime overhead.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 -O2 -pthread mini_project_8_merge_sort_multithreaded.cpp -o mp8
./mp8
```
**Sample Input:**
```text
100000
```
**Expected Output:**
```text
--- Scenario: Best Case (Already Sorted Array) ---
Sequential Time       : ~6500 us
Multithreaded Time    : ~2200 us
Speedup Factor        : ~2.9x
Key Comparisons (Seq) : ~850000
Correctness Check     : PASSED (Verified)

--- Scenario: Worst Case (Adversarial Alternating Split) ---
Sequential Time       : ~8200 us
Multithreaded Time    : ~2800 us
Speedup Factor        : ~2.9x
Key Comparisons (Seq) : ~1560000
Correctness Check     : PASSED (Verified)
```
