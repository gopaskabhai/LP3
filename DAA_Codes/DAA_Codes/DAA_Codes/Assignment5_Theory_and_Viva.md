# Assignment 5: N-Queens Problem with First Queen Placed (Backtracking)

## 1. Problem Statement
Design an $N$-Queens matrix having the first Queen placed. Use backtracking to place the remaining Queens to generate the final $n$-queen's matrix.

---

## 2. Theory & Core Concepts

- **N-Queens Problem**: Place $N$ non-attacking queens on an $N \times N$ chessboard such that no two queens attack each other.
- **Attack Rules**: Queens attack horizontally (same row), vertically (same column), and diagonally.
- **Fixed Position (Constraint)**: The user fixes the position of the first Queen in Row 0 (at column $c_0$). The algorithm must place remaining $N - 1$ queens in rows $1$ through $N - 1$ respecting this initial placement.
- **Backtracking Paradigm**:
  - Systematic search over the state space tree.
  - Places one queen per row.
  - For row $r$, tries each column $c \in [0, N-1]$.
  - If $(r, c)$ is safe from all queens in rows $0 \dots r-1$, it places the queen and recursively solves row $r + 1$.
  - If a dead-end is reached (no column is safe), it removes the queen (backtracks) and attempts the next column.

---

## 3. Safety Check Logic & Complexity

### Safety Conditions for $(r, c)$:
1. **Vertical Column**: $\text{board}[i][c] == 0$ for all $0 \le i < r$.
2. **Major Diagonal (Upper Left)**: $r - i == c - j$
3. **Minor Diagonal (Upper Right)**: $r - i == j - c$

### Time & Space Complexity Analysis

| Metric | Complexity | Explanation |
| :--- | :--- | :--- |
| **Worst-Case Time** | **$O(N!)$** | Upper bounded by $N \cdot (N-1) \cdot (N-2) \cdots 1 = N!$. Pruning invalid branches cuts down the search space substantially in practice. |
| **Space Complexity** | **$O(N^2)$** | Board representation of size $N \times N$. |
| **Auxiliary Stack Space** | **$O(N)$** | Recursion stack depth is at most $N$ frames. |

---

## 4. Alternative Methods & Optimizations
1. **$O(1)$ Bitmask / Boolean Lookup for Safety Check**:
   - Instead of checking loops across columns and diagonals ($O(N)$), maintain 3 boolean arrays or bitmasks:
     - `colUsed[N]`
     - `diag1Used[2N - 1]` (indexed by `row - col + N - 1`)
     - `diag2Used[2N - 1]` (indexed by `row + col`)
   - Yields an instantaneous $O(1)$ safety test per candidate position.
2. **Genetic Algorithm / Simulated Annealing**: Heuristic search used for huge boards ($N \ge 1000$).
3. **Warnsdorff's / Min-Conflicts Heuristic**: Selects variables and values causing the fewest future conflicts.

---

## 5. Viva / External Questions & Answers

**Q1: What is Backtracking and how does it differ from Brute Force?**
> **Answer**: Brute force generates all $N^N$ (or $\binom{N^2}{N}$) board configurations and tests every single one. Backtracking constructs candidate solutions incrementally, abandoning (pruning) a candidate immediately as soon as a constraint is violated, saving immense computation time.

**Q2: What is the minimum value of $N$ for which an N-Queens solution exists?**
> **Answer**: For $N = 1$, trivial solution exists. For $N = 2$ and $N = 3$, **no solution exists**. For all $N \ge 4$, valid solutions always exist.

**Q3: Can a solution always be found if the first queen is placed arbitrarily?**
> **Answer**: Not necessarily! For small boards like $N = 4$, placing the first queen at $(0, 0)$ leaves no valid placement for the remaining 3 queens. However, placing the first queen at $(0, 1)$ or $(0, 2)$ yields valid solutions.

**Q4: How do you verify diagonal safety without 2D loops?**
> **Answer**: Two squares $(r_1, c_1)$ and $(r_2, c_2)$ share a diagonal if and only if:
> $$|r_1 - r_2| == |c_1 - c_2|$$
> or by keeping hash sets of $(r - c)$ and $(r + c)$.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 assignment5_n_queens.cpp -o ass5
./ass5
```
**Sample Input:**
```text
4
1
```
*(Board size $N=4$, first queen in row 0 at column index 1)*

**Expected Output:**
```text
--- Initial Board Configuration (First Queen in Row 0, Col 1) ---
  . Q . . 
  . . . . 
  . . . . 
  . . . . 

Placing remaining 3 Queens using Backtracking...

Solution #1:
  . Q . . 
  . . . Q 
  Q . . . 
  . . Q . 

Successfully placed all 4 Queens!
```
