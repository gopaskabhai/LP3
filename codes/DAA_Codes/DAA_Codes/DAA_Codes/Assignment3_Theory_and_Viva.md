# Assignment 3: Fractional Knapsack Problem (Greedy Strategy)

## 1. Problem Statement
Write a program to solve a fractional Knapsack problem using a greedy method.

---

## 2. Theory & Core Concepts

- **Knapsack Problem Class**: Given $n$ items, each with weight $w_i$ and value/profit $v_i$, and a knapsack of maximum weight capacity $W$.
- **Fractional Property**: We are allowed to break items into arbitrary continuous fractions ($0 \le x_i \le 1$).
- **Objective Function**: Maximize total profit $\sum_{i=1}^n x_i \cdot v_i$ subject to the weight constraint $\sum_{i=1}^n x_i \cdot w_i \le W$.
- **Greedy Strategy**:
  1. Compute the value-to-weight ratio for each item: $r_i = \frac{v_i}{w_i}$.
  2. Sort items in descending order of their ratio $r_i$.
  3. Greedily select the full item with highest ratio if capacity allows ($x_i = 1$).
  4. Once an item cannot fit completely, take the exact remaining fraction $x_k = \frac{W_{\text{remaining}}}{w_k}$.
  5. The knapsack is now full; return optimal total value.

---

## 3. Time & Space Complexity Analysis

| Metric | Complexity | Derivation |
| :--- | :--- | :--- |
| **Sorting Time** | **$O(n \log n)$** | Sorting $n$ items based on value/weight ratio. |
| **Greedy Selection Loop** | **$O(n)$** | Single linear pass through sorted items. |
| **Total Time Complexity** | **$O(n \log n)$** | Dominated by sorting. |
| **Space Complexity** | **$O(1)$** auxiliary | Beyond storing the input items, only scalar accumulators are used. |

---

## 4. Alternative Methods
1. **Linear Time Selection ($O(n)$ using Quickselect / Median-of-Medians)**:
   - Instead of sorting all items ($O(n \log n)$), use a weighted selection algorithm to find the median ratio in $O(n)$ time.
   - Partition items into higher-ratio and lower-ratio subsets, achieving overall $O(n)$ worst-case time.
2. **0/1 Knapsack contrast**: If items cannot be broken (discrete choice $x_i \in \{0, 1\}$), the greedy approach fails to find the global optimum and Dynamic Programming ($O(n W)$) must be used.

---

## 5. Viva / External Questions & Answers

**Q1: Why does Greedy Strategy work for Fractional Knapsack but fail for 0/1 Knapsack?**
> **Answer**: Fractional Knapsack exhibits the **Greedy-Choice Property**: taking as much of the item with highest value density ($v/w$) is guaranteed to leave room that can be filled with the next best density, leaving no unused empty space. In 0/1 knapsack, taking an item with high density might leave an empty gap that cannot fit any other item, leading to wasted capacity.

**Q2: How do you prove the correctness of the greedy choice for fractional knapsack?**
> **Answer**: Using a standard **cut-and-paste exchange argument**: Suppose there is an optimal solution $S^*$ that does not take the item with the highest ratio $r_1$. We can replace a fraction of an item in $S^*$ with a fraction of item 1 having weight $\Delta w$. The new profit increases by $\Delta w (r_1 - r_{\text{replaced}}) \ge 0$, proving our greedy solution is at least as optimal.

**Q3: Can Fractional Knapsack be solved faster than $O(n \log n)$?**
> **Answer**: Yes! By using the $O(n)$ deterministic selection algorithm (median-of-medians Quickselect) to partition items around the median ratio without a full sort, solving it in $O(n)$ time.

**Q4: What happens if two items have identical value-to-weight ratios?**
> **Answer**: Either item can be picked first; it does not affect the optimal total value.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 assignment3_fractional_knapsack.cpp -o ass3
./ass3
```
**Sample Input:**
```text
3
10 60
20 100
30 120
50
```
**Expected Output:**
```text
Item 1 (ratio 6.0): 100% taken (wt: 10, val: 60)
Item 2 (ratio 5.0): 100% taken (wt: 20, val: 100)
Item 3 (ratio 4.0): 66.67% taken (wt: 20 of 30, val: 80)
Maximum Total Value: 240.00
```
