# Assignment 6: Quick Sort Analysis (Deterministic vs Randomized Variant)

## 1. Problem Statement
Write a program for analysis of quick sort by using deterministic and randomized variant.

---

## 2. Theory & Core Concepts

- **Divide and Conquer Paradigm**:
  1. **Divide**: Partition the array $A[low \dots high]$ around a pivot element $p$ such that all elements $\le p$ are to the left, and all elements $> p$ are to the right.
  2. **Conquer**: Recursively sort the left and right subarrays.
  3. **Combine**: In-place, no extra combining needed.

### A. Deterministic Quick Sort
- **Pivot Selection**: Static/deterministic rule (e.g., always picking the first element, last element, or middle element).
- **The Achilles' Heel**: When the input is already sorted, reverse sorted, or contains many duplicates, picking the first or last element produces an extremely unbalanced partition ($0$ elements on one side, $n-1$ on the other).
- **Recurrence**: $T(n) = T(n-1) + T(0) + \Theta(n) \implies \Theta(n^2)$.

### B. Randomized Quick Sort
- **Pivot Selection**: A pivot is chosen uniformly at random from the range $[low, high]$:
  $$\text{pivotIndex} = low + (\text{rand}() \pmod{high - low + 1})$$
  Swap $A[\text{pivotIndex}]$ with $A[high]$ and partition normally.
- **Why Randomization Helps**: No single adversarial input (like already sorted or reverse sorted) can systematically trigger the worst-case behavior.
- **Expected Running Time**: $O(n \log n)$ on *any* input array with very high probability.

---

## 3. Time & Space Complexity Analysis

| Metric | Deterministic Quick Sort | Randomized Quick Sort | Explanation |
| :--- | :--- | :--- | :--- |
| **Best Case Time** | **$\Omega(n \log n)$** | **$\Omega(n \log n)$** | Partitions split array exactly in half ($n/2 : n/2$). |
| **Average Case Time** | **$\Theta(n \log n)$** | **$\Theta(n \log n)$** | Any constant proportional split (e.g. 99-to-1) yields logarithmic depth. |
| **Worst Case Time** | **$O(n^2)$** | **$O(n^2)$** (extremely improbable: probability $< 1/n!$) | Extremely unbalanced split occurring at every single step. |
| **Space Complexity** | **$O(\log n)$** avg, **$O(n)$** worst | **$O(\log n)$** expected | Auxiliary stack space for recursion. |
| **Stability** | Not Stable | Not Stable | Long-distance element swaps can reorder identical keys. |

---

## 4. Alternative Partitioning & Pivot Strategies
1. **Hoare's Partitioning Scheme**: Uses two pointers converging from both ends; performs approximately three times fewer swaps than the Lomuto scheme.
2. **Median-of-Three Partitioning**: Takes the median of `arr[low]`, `arr[mid]`, and `arr[high]`. This avoids the $O(n^2)$ worst case on sorted arrays deterministically.
3. **Dual-Pivot Quick Sort (Yaroslavskiy)**: Used in standard Java / Android runtime libraries. Picks two pivots and divides array into three segments, yielding fewer memory cache misses.
4. **Introsort**: Starts with Quick Sort; if recursion depth exceeds $2 \log n$, it switches to Heap Sort to guarantee an $O(n \log n)$ worst-case bound.

---

## 5. Viva / External Questions & Answers

**Q1: Why does Deterministic Quick Sort degrade to $O(n^2)$ on already sorted arrays?**
> **Answer**: When the last element is chosen as the pivot in an already sorted array, it is the largest element. Partitioning places all $n-1$ elements in the left subarray and 0 in the right subarray. The recursion tree degenerates into a linear chain of depth $n$, performing $\sum_{i=1}^n i = \frac{n(n-1)}{2} \approx O(n^2)$ comparisons.

**Q2: Does Randomized Quick Sort eliminate the $O(n^2)$ worst case entirely?**
> **Answer**: Theoretically, the worst-case time is still $O(n^2)$, but that only happens if the random number generator happens to select the extreme maximum or minimum element at *every single level* of the recursion tree. The probability of this happening is $\frac{1}{n!}$, which is practically zero for any decent array size.

**Q3: Which partition scheme is used in standard implementations, Lomuto or Hoare?**
> **Answer**: Hoare's scheme is faster in practice because it does 3x fewer swaps on average and handles duplicate keys better. Lomuto is easier to understand and analyze mathematically.

**Q4: Compare Quick Sort with Merge Sort.**
> **Answer**:
> - **Quick Sort**: In-place ($O(\log n)$ stack space), cache-friendly, but $O(n^2)$ worst case (unless randomized/introsort). Unstable.
> - **Merge Sort**: Stable, guaranteed $O(n \log n)$ worst case, but requires $O(n)$ auxiliary memory.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 assignment6_quick_sort_analysis.cpp -o ass6
./ass6
```
**Sample Input:**
```text
500
1
```
*(Array size 500, option 1 for already sorted array)*

**Expected Output:**
```text
================ Performance Analysis Table ================
Metric                      Deterministic         Randomized            
----------------------------------------------------------------------
Pivot Strategy              Fixed (arr[high])     Random element        
Total Comparisons           124750                ~4600                 
Total Swaps                 125249                ~3100                 
Execution Time (us)         ~2000                 ~70                   
Correctly Sorted?           Yes                   Yes                   
----------------------------------------------------------------------

Observation: Notice how on sorted data, Deterministic Quick Sort performs
~ N^2/2 comparisons (124750), while Randomized Quick Sort achieves 
optimal O(N log N) performance (4643 comparisons)!
```
