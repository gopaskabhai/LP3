# Assignment 1: Fibonacci Numbers (Recursive and Non-Recursive) & Complexity Analysis

## 1. Problem Statement
Write a program (non-recursive and recursive) to calculate Fibonacci numbers and analyze their time and space complexity.

---

## 2. Theory & Core Concepts

The Fibonacci sequence is defined by the linear recurrence relation:
$$F(0) = 0, \quad F(1) = 1$$
$$F(n) = F(n-1) + F(n-2) \quad \text{for } n \ge 2$$

Sequence: `0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, ...`

### A. Non-Recursive (Iterative) Approach
- **Mechanism**: Maintains two running accumulators (`a` for $F(i-1)$ and `b` for $F(i)$). It iteratively advances through a single loop up to $n$.
- **State Transition**: `next = a + b; a = b; b = next;`

### B. Recursive Approach
- **Mechanism**: Uses the divide-and-conquer paradigm. Each invocation divides calculating $F(n)$ into two sub-problems: $F(n-1)$ and $F(n-2)$.
- **Recursion Tree**: Highly branching binary tree where identical sub-problems (e.g., $F(2)$, $F(3)$) are computed repeatedly exponentially many times.

---

## 3. Time & Space Complexity Analysis

| Approach | Time Complexity | Space Complexity | Why / Derivation |
| :--- | :--- | :--- | :--- |
| **Iterative (Non-Recursive)** | **$O(n)$** | **$O(1)$** | Single loop executed $n-1$ times; only 3 scalar variables needed. |
| **Recursive (Naive)** | **$O(2^n)$** (Exact: $O(\phi^n) \approx 1.618^n$) | **$O(n)$** | Recurrence: $T(n) = T(n-1) + T(n-2) + O(1)$. Maximum call stack depth equals $n$. |
| **Memoization / DP** | **$O(n)$** | **$O(n)$** | Each state $0 \dots n$ computed exactly once and stored. |
| **Matrix Exponentiation** | **$O(\log n)$** | **$O(\log n)$** or **$O(1)$** | $\begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n = \begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix}$ computed via binary exponentiation. |
| **Binet's Formula** | **$O(1)$** (assuming constant time `pow`) | **$O(1)$** | $F(n) = \frac{\phi^n - \psi^n}{\sqrt{5}}$, prone to floating point inaccuracies for large $n$. |

---

## 4. Alternative Methods
1. **Dynamic Programming (Bottom-Up Tabulation or Top-Down Memoization)**: Store previously solved subproblems in an array of size $n+1$, bringing time down from $O(2^n)$ to $O(n)$ and space to $O(n)$.
2. **Matrix Exponentiation ($O(\log n)$)**:
   $$\begin{pmatrix} F(n+1) & F(n) \\ F(n) & F(n-1) \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n$$
   Using repeated squaring, power computation takes logarithmic steps.

---

## 5. Viva / External Questions & Answers

**Q1: Why is pure recursion inefficient for calculating Fibonacci numbers?**
> **Answer**: Pure recursion has overlapping subproblems. Computing $F(5)$ requires computing $F(4)$ and $F(3)$; then $F(4)$ again computes $F(3)$ and $F(2)$. The number of redundant calls grows exponentially as $O(1.618^n)$, leading to repeated calculations of identical values.

**Q2: What is the auxiliary space of recursive Fibonacci and why?**
> **Answer**: $O(n)$. Although the total number of calls is exponential, the recursive calls are resolved depth-first. At any moment, the maximum height of the call stack (depth of the recursion tree) is $n$.

**Q3: Can Fibonacci be calculated in $O(\log n)$ time?**
> **Answer**: Yes, using Matrix Exponentiation of the transition matrix $\begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n$ via divide-and-conquer binary exponentiation in $O(\log n)$ matrix multiplications.

**Q4: How does step count help in algorithm analysis?**
> **Answer**: Step count measures the exact frequency of elementary operations (assignments, additions, comparisons) independent of hardware or compiler optimizations, directly validating the theoretical Big-O curves.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 assignment1_fibonacci.cpp -o ass1
./ass1
```
**Sample Input:**
```text
6
```
**Expected Output:**
```text
Fibonacci(6) = 8
Iterative Step Count: ~26 (O(n))
Recursive Function Calls: 25 (O(2^n))
Fibonacci Series: 0 1 1 2 3 5 8
```
