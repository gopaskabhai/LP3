# Mini Project 9: Naive vs Rabin-Karp String Matching

## 1. Problem Statement
Implement the Naive string matching algorithm and Rabin-Karp algorithm for string matching. Observe differences in the working of both algorithms for the same input.

---

## 2. Theory & Core Concepts

Given a text string $T[0 \dots n-1]$ and a pattern string $P[0 \dots m-1]$ ($m \le n$), find all occurrences (valid shift offsets $s$) where $T[s \dots s+m-1] == P[0 \dots m-1]$.

### A. Naive String Matching Algorithm
- **Mechanism**: Slides the pattern over the text one character at a time ($s = 0, 1, \dots, n-m$).
- At each shift $s$, it performs character-by-character comparisons: $T[s+j] \stackrel{?}{=} P[j]$ for $j = 0, \dots, m-1$.
- Upon the first mismatch, it shifts the pattern by exactly 1 position and starts matching from $P[0]$ all over again.
- **Worst-Case Input**: Text consisting of repeated identical characters with a mismatch at the end (e.g., $T = \text{"AAAAAAAAAA"}$, $P = \text{"AAAB"}$). Every shift makes $m$ comparisons before failing.

### B. Rabin-Karp Algorithm (Rolling Hash)
- **Mechanism**: Rather than checking characters directly, it maps both strings to numerical hash values.
- **Horner's Rule Hash Representation**:
  $$p = \left(\sum_{j=0}^{m-1} P[j] \cdot d^{m-1-j}\right) \pmod q$$
- **Rolling Hash Property**: When shifting from window $T[s \dots s+m-1]$ to window $T[s+1 \dots s+m]$, the new hash is computed in $\mathcal{O}(1)$ time without rescanning the substring:
  $$t_{s+1} = \left(d \cdot \big(t_s - T[s] \cdot h\big) + T[s+m]\right) \pmod q$$
  *where $h = d^{m-1} \pmod q$, $d = 256$ (alphabet size), and $q$ is a prime number.*
- **Spurious Hits**: Because hash values are taken modulo $q$, two different strings may produce the same hash value ($t_s == p$). When hashes match, character-by-character verification is performed to eliminate false positives.

---

## 3. Time & Space Complexity Analysis

| Algorithm | Pre-processing Time | Matching Time (Avg) | Matching Time (Worst) | Auxiliary Space |
| :--- | :--- | :--- | :--- | :--- |
| **Naive Algorithm** | $\mathcal{O}(0)$ | $\mathcal{O}(n)$ | $\mathcal{O}(m \cdot (n - m + 1))$ | $\mathcal{O}(1)$ |
| **Rabin-Karp** | $\mathcal{O}(m)$ | $\mathcal{O}(n + m)$ | $\mathcal{O}(m \cdot (n - m + 1))$ | $\mathcal{O}(1)$ |

---

## 4. Alternative Methods
1. **Knuth-Morris-Pratt (KMP) Algorithm**:
   - Computes a Longest Prefix Suffix (LPS) $\pi$-table in $\mathcal{O}(m)$ time.
   - Never backtracks over text characters, achieving strict worst-case $\mathcal{O}(n + m)$ time.
2. **Boyer-Moore Algorithm**:
   - Matches pattern characters right-to-left using Bad Character and Good Suffix heuristics.
   - Best-case sublinear matching time $\mathcal{O}(n / m)$; widely used in text editors (`grep`).
3. **Aho-Corasick Automaton**:
   - Builds a trie with failure links to match multiple patterns simultaneously in $\mathcal{O}(n + \sum m_i)$ time.

---

## 5. Viva / External Questions & Answers

**Q1: What is a "Spurious Hit" in the Rabin-Karp algorithm?**
> **Answer**: A spurious hit occurs when the hash value of the current text window matches the hash value of the pattern ($t_s \equiv p \pmod q$), but the actual characters of the window and pattern are different (hash collision). It requires an $\mathcal{O}(m)$ character verification that results in a mismatch.

**Q2: What is the main advantage of Rabin-Karp over KMP and Boyer-Moore?**
> **Answer**: Rabin-Karp easily generalizes to **multiple pattern matching** (e.g., detecting plagiarism across hundreds of target phrases). By storing the hashes of $k$ patterns in a hash set/Bloom filter, matching against all $k$ patterns takes $\mathcal{O}(n)$ average time.

**Q3: Why do we pick a prime number for the modulus $q$?**
> **Answer**: Using a prime number for $q$ distributes the hash values uniformly across the range $[0, q-1]$, minimizing the probability of hash collisions and spurious hits.

**Q4: How does the rolling hash achieve $\mathcal{O}(1)$ update time?**
> **Answer**: It subtracts the leading character's weighted contribution ($T[s] \cdot d^{m-1}$), shifts the remaining polynomial by multiplying by $d$, and adds the trailing new character ($T[s+m]$), all modulo $q$.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 mini_project_9_string_matching.cpp -o mp9
./mp9
```
**Sample Input:**
```text
AABAACAADAABAABA
AABA
```
**Expected Output:**
```text
1. Naive Algorithm:
   - Pattern found at indices      : 0 9 12 
   - Character Comparisons Made    : 25

2. Rabin-Karp Algorithm:
   - Pattern found at indices      : 0 9 12 
   - Hash Window Comparisons Made : 13
   - Character Comparisons (on match): 12
   - Spurious Hits (Hash Collisions) : 0
```
