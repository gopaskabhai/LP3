# Mini Project 10: Travelling Salesperson Problem (TSP) - Exact vs Approximation Algorithms

## 1. Problem Statement
Different exact and approximation algorithms for Travelling-Sales-Person Problem (TSP).

---

## 2. Theory & Core Concepts

Given a list of $N$ cities and symmetric/asymmetric distances between each pair of cities, the **Travelling Salesperson Problem (TSP)** seeks the shortest possible Hamiltonian cycle that visits every city exactly once and returns to the origin city.
- **Complexity Class**: TSP is **NP-Hard** in optimization form and **NP-Complete** in decision form.
- No polynomial-time algorithm is known for finding the exact optimal tour.

### A. Exact Algorithms:
1. **Brute Force Permutation**:
   - Evaluates all $(N - 1)! / 2$ Hamiltonian tours.
   - Time Complexity: $\mathcal{O}(N!)$, completely intractable for $N > 12$.
2. **Held-Karp Dynamic Programming (Bitmask DP)**:
   - State: $DP(S, u) =$ minimum cost path starting at city 0, visiting all cities in subset $S$, and ending at city $u$.
   - Recurrence:
     $$DP(S, u) = \min_{v \in S \setminus \{u\}} \Big( DP(S \setminus \{u\}, v) + \text{dist}(v, u) \Big)$$
   - Subsets $S$ are represented as bit integers (bitmasks): $mask \in [0, 2^N - 1]$.
   - Time Complexity: $\mathcal{O}(N^2 2^N)$ — reduces execution time for $N=20$ from $20! \approx 2.4 \times 10^{18}$ down to $20^2 \times 2^{20} \approx 4.2 \times 10^8$ operations.
3. **Branch and Bound**:
   - Explores a state-space search tree.
   - Uses reduced cost matrices (row and column reductions) to compute a lower bound for each node.
   - Prunes any branch whose lower bound exceeds the best known tour.

### B. Approximation & Heuristic Algorithms:
1. **Nearest Neighbor (Greedy Heuristic)**:
   - Starts at city 0; continually steps to the nearest unvisited city until all are visited, then returns to 0.
   - Time Complexity: $\mathcal{O}(N^2)$.
   - While fast, it can get trapped in bad choices toward the end. In general graphs, the approximation factor can grow as $\Theta(\log N)$.
2. **MST-Based 2-Approximation (Metric TSP)**:
   - **Condition**: Satisfies the **Triangle Inequality**: $\text{dist}(u, w) \le \text{dist}(u, v) + \text{dist}(v, w)$.
   - Steps:
     1. Compute a Minimum Spanning Tree (MST) using Prim's algorithm ($\mathcal{O}(N^2)$).
     2. Double the edges of the MST to form an Eulerian multigraph where total weight $= 2 \times \text{weight}(\text{MST})$.
     3. Perform a preorder DFS traversal, skipping already visited vertices (taking shortcuts).
   - **Theorem**: Because removing any edge from the optimal TSP tour leaves a spanning tree, $\text{weight}(\text{MST}) \le \text{OPT}$.
     $$\text{Cost}(\text{Tour}) \le 2 \times \text{weight}(\text{MST}) \le 2 \times \text{OPT}$$
     Guaranteed **2-approximation ratio**!
3. **Christofides' Algorithm**:
   - Combines MST with a Minimum Weight Perfect Matching on odd-degree vertices.
   - Guarantees a **1.5-approximation ratio** ($\text{Cost} \le 1.5 \times \text{OPT}$) in $\mathcal{O}(N^3)$ time.

---

## 3. Complexity & Approximation Summary

| Algorithm | Category | Time Complexity | Space Complexity | Approximation Factor |
| :--- | :--- | :--- | :--- | :--- |
| **Brute Force** | Exact | $\mathcal{O}(N!)$ | $\mathcal{O}(N)$ | $1.0$ (Exact) |
| **Held-Karp DP** | Exact | $\mathcal{O}(N^2 \cdot 2^N)$ | $\mathcal{O}(N \cdot 2^N)$ | $1.0$ (Exact) |
| **Branch and Bound** | Exact | $\mathcal{O}(N!)$ worst, faster avg | $\mathcal{O}(N^2)$ | $1.0$ (Exact) |
| **Nearest Neighbor** | Heuristic | $\mathcal{O}(N^2)$ | $\mathcal{O}(N)$ | $\frac{1}{2} \lceil \log_2 N \rceil + \frac{1}{2}$ |
| **MST-Preorder** | Approximation | $\mathcal{O}(N^2)$ | $\mathcal{O}(N)$ | $\le 2.0 \times \text{OPT}$ |
| **Christofides** | Approximation | $\mathcal{O}(N^3)$ | $\mathcal{O}(N^2)$ | $\le 1.5 \times \text{OPT}$ |

---

## 4. Viva / External Questions & Answers

**Q1: What is the difference between an Approximation Algorithm and a Heuristic?**
> **Answer**: 
> - An **approximation algorithm** provides a mathematically proven guarantee on the solution quality (e.g., MST 2-approximation is provably at most $2.0 \times \text{OPT}$ on any metric graph).
> - A **heuristic** (like nearest neighbor or genetic algorithm) provides good practical answers quickly, but lacks a guaranteed worst-case bound on how close it is to optimal.

**Q2: What is Metric TSP and why is the Triangle Inequality crucial?**
> **Answer**: Metric TSP assumes edge weights satisfy the Triangle Inequality: $\text{dist}(A, C) \le \text{dist}(A, B) + \text{dist}(B, C)$. This ensures that taking a "shortcut" directly from $A$ to $C$ will never increase the total path cost, making shortcutting in MST / Christofides valid.

**Q3: Can General TSP (without triangle inequality) have a constant factor approximation algorithm?**
> **Answer**: No! If $P \ne NP$, general TSP cannot be approximated within any constant factor $\rho \ge 1$ (or even $2^N$), because any polynomial-time approximation for general TSP could be used to solve the Hamiltonian Cycle problem in polynomial time.

**Q4: How does Held-Karp Bitmask DP reduce time from $N!$ to $N^2 2^N$?**
> **Answer**: In brute force, the order of visited intermediate cities creates redundant permutations. In Held-Karp, the state only tracks $(S, u)$ — the *set* of visited cities and the *current* city. It doesn't matter in which permutation the cities in $S$ were visited, only the minimum cost to reach $u$ with set $S$. This dynamic programming memoization prunes duplicate search paths.

---

## 5. How to Run & Verify
```bash
g++ -std=c++17 mini_project_10_tsp_exact_approx.cpp -o mp10
./mp10
```
**Sample Input:**
```text
4
2
```
*(Chooses the standard 4-city symmetric benchmark matrix)*

**Expected Output:**
```text
1. Exact Algorithm (Held-Karp Dynamic Programming):
   - Optimal Cost : 80
   - Optimal Tour : 0 -> 1 -> 3 -> 2 -> 0

2. Approximation 1 (Nearest Neighbor Greedy):
   - Heuristic Cost : 80
   - Heuristic Tour : 0 -> 1 -> 3 -> 2 -> 0
   - Approx Ratio   : 1.000x optimal

3. Approximation 2 (MST Pre-order 2-Approximation):
   - Heuristic Cost : 80
   - Heuristic Tour : 0 -> 1 -> 3 -> 2 -> 0
   - Approx Ratio   : 1.000x optimal
```
