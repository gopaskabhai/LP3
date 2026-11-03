# Assignment 4: 0/1 Knapsack Problem (Dynamic Programming)

## 1. Problem Statement
Write a program to solve a 0-1 Knapsack problem using dynamic programming or branch and bound strategy.

---

## 2. Theory & Core Concepts

- **Problem Definition**: Given $n$ items, item $i$ with weight $w_i$ and value $v_i$, and total capacity $W$. Each item must either be fully taken ($x_i = 1$) or left behind ($x_i = 0$).
- **Subproblem Overlap & Optimal Substructure**: An optimal choice on $i$ items either excludes item $i$ (yielding optimal solution for $i-1$ items with capacity $w$), or includes item $i$ (yielding value $v_i$ plus optimal solution for $i-1$ items with remaining capacity $w - w_i$).
- **DP Recurrence Relation**:
  $$DP[i][w] = \begin{cases} 
  DP[i-1][w] & \text{if } w_i > w \\ 
  \max\Big(DP[i-1][w], \; v_i + DP[i-1][w - w_i]\Big) & \text{if } w_i \le w 
  \end{cases}$$
- **Backtracking Selected Items**: Start at $DP[n][W]$. If $DP[i][w] \ne DP[i-1][w]$, item $i$ was picked; decrement $w \leftarrow w - w_i$ and continue upward.

---

## 3. Time & Space Complexity Analysis

| Metric | Complexity | Explanation |
| :--- | :--- | :--- |
| **Time Complexity** | **$O(n \cdot W)$** | Table has $(n+1) \times (W+1)$ cells; each cell takes $O(1)$ constant time to compute. |
| **Space Complexity (2D Table)** | **$O(n \cdot W)$** | Memory required to store the full DP table for backtracking selected items. |
| **Space Complexity (1D Space-Optimized)** | **$O(W)$** | Since row $i$ only depends on row $i-1$, we can iterate backward from $W$ down to $w_i$ using a single array of size $W+1$. |
| **Nature of Time Complexity** | **Pseudo-polynomial** | Running time depends on the numerical value of $W$, not just input length $\log W$. The problem is NP-complete. |

---

## 4. Alternative Methods
1. **Branch and Bound (Best-First Search)**:
   - Uses a State Space Tree.
   - Computes an upper bound for each node using the Fractional Knapsack relaxation (greedy).
   - Prunes subtrees whose upper bound $\le$ best known feasible solution.
   - Works effectively when $W$ is extraordinarily large (e.g., $W = 10^{12}$), where DP fails.
2. **Memoized Recursion (Top-Down DP)**: Solves only the subproblems reachable from $(n, W)$, avoiding computing unreachable states.
3. **FPTAS (Fully Polynomial-Time Approximation Scheme)**: Scales values by a factor to obtain a $(1 - \epsilon)$ approximate answer in polynomial time $O(n^3 / \epsilon)$.

---

## 5. Viva / External Questions & Answers

**Q1: Why is the DP algorithm for 0/1 Knapsack called "Pseudo-Polynomial"?**
> **Answer**: The input size is measured in bits. Number $W$ takes $\log_2(W)$ bits to represent. The time complexity $O(n \cdot W) = O(n \cdot 2^k)$ is actually **exponential** in the number of bits $k$ required to represent the number $W$. If $W$ is very large (e.g., $10^9$), DP becomes impractical.

**Q2: What is the difference between Fractional Knapsack and 0/1 Knapsack?**
> **Answer**:
> - **Fractional Knapsack**: Items are divisible ($0 \le x_i \le 1$), solved optimally using a Greedy approach in $O(n \log n)$ time.
> - **0/1 Knapsack**: Items are indivisible ($x_i \in \{0, 1\}$), greedy choice fails, solved using Dynamic Programming in $O(n W)$ or Branch & Bound.

**Q3: How can we reduce space complexity from $O(n \cdot W)$ to $O(W)$?**
> **Answer**: Notice that $DP[i][w]$ only depends on entries from the previous row $DP[i-1]$. We can use a 1D array `dp[w]` and update it in **reverse order** (from $w = W$ down to $w_i$) so that values from the previous iteration are not prematurely overwritten.

**Q4: How do you identify which items contributed to the maximum profit?**
> **Answer**: Trace back from cell $(n, W)$. If $DP[i][w] == DP[i-1][w]$, item $i$ was excluded. If $DP[i][w] > DP[i-1][w]$, item $i$ was included; subtract $w_i$ from $w$ and repeat for $i-1$.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 assignment4_01_knapsack.cpp -o ass4
./ass4
```
**Sample Input:**
```text
4
2 12
1 10
3 20
2 15
5
```
**Expected Output:**
```text
DP Table:
 Items\W    0    1    2    3    4    5
----------------------------------------
0 (Base)    0    0    0    0    0    0
  Item 1    0    0   12   12   12   12
  Item 2    0   10   12   22   22   22
  Item 3    0   10   12   22   30   32
  Item 4    0   10   15   25   30   37

Selected Items: Item 4, Item 2, Item 1
Total Weight Used: 5 / 5
Maximum Value/Profit: 37
```
