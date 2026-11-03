# Assignment 2: Huffman Encoding using Greedy Strategy

## 1. Problem Statement
Write a program to implement Huffman Encoding using a greedy strategy.

---

## 2. Theory & Core Concepts

- **Lossless Data Compression**: Huffman Coding is a variable-length entropy encoding algorithm designed by David Huffman in 1952.
- **Key Observation**: More frequently occurring characters are assigned shorter bit codes, while less frequent characters receive longer bit codes.
- **Prefix Rule (Prefix Property)**: No assigned code word is a prefix of any other code word. This makes the encoded bitstream uniquely decodable without delimiters.
- **Greedy Strategy**:
  1. Build a min-priority queue where each character is a single-node tree with weight equal to its frequency.
  2. Greedily extract the two nodes having the smallest frequencies.
  3. Create an internal parent node with frequency equal to the sum of the two children.
  4. Reinsert the parent back into the priority queue.
  5. Repeat until a single tree (the Huffman Tree) remains.
  6. Left edge = '0', Right edge = '1'. Leaf paths represent binary codes.

---

## 3. Time & Space Complexity Analysis

| Metric | Complexity | Derivation |
| :--- | :--- | :--- |
| **Tree Building Time** | **$O(n \log n)$** | For $n$ unique characters: min-heap initialization takes $O(n)$, and there are $n-1$ extractions/insertions where each heap operation takes $O(\log n)$. |
| **Code Generation Time** | **$O(n)$** | Tree traversal (DFS) visits $2n - 1$ total nodes in the binary tree. |
| **Total Time Complexity** | **$O(n \log n)$** | Dominated by the priority queue operations. |
| **Space Complexity** | **$O(n)$** | Min-heap stores at most $n$ nodes; the binary tree has exactly $2n - 1$ nodes; hash map stores $n$ entries. |

---

## 4. Alternative Methods
1. **Sorted Input Array ($O(n)$ Linear Time)**: If the characters and frequencies are already pre-sorted, we can use two FIFO queues instead of a priority queue to construct the Huffman Tree in strictly $O(n)$ time.
2. **Shannon-Fano Coding**: A top-down recursive splitting technique that partitions symbols into two subsets of approximately equal probability. Huffman coding is mathematically guaranteed to be equal to or better than Shannon-Fano.
3. **Adaptive (Dynamic) Huffman Coding (Vitter / FGK Algorithm)**: Updates the tree on-the-fly in a single pass over the stream without transmitting a pre-computed frequency table.

---

## 5. Viva / External Questions & Answers

**Q1: Why is Huffman coding considered a greedy algorithm?**
> **Answer**: At each step, it locally selects the two smallest frequency symbols to combine, never reconsidering previous combinations. This local greedy choice is mathematically proven to produce a globally optimal prefix-free binary tree.

**Q2: What is the Prefix Rule and why is it essential?**
> **Answer**: The Prefix Property states that no character's binary code can be a prefix of another character's code (e.g., if 'a' is `01`, no other character can start with `01`). This ensures unambiguous, deterministic instantaneous decoding without delimiters.

**Q3: How many total nodes are present in a Huffman tree for $n$ distinct characters?**
> **Answer**: A Huffman tree is a full (strictly proper) binary tree where every node has either 0 or 2 children. For $n$ leaf nodes (distinct characters), there are exactly $n - 1$ internal nodes, yielding a total of $2n - 1$ nodes.

**Q4: In which case will Huffman coding produce fixed-length codes?**
> **Answer**: When all characters have uniform (equal) probabilities (e.g., $1/2^k$), the resulting tree is completely balanced, giving each symbol a code of length $k$.

---

## 6. How to Run & Verify
```bash
g++ -std=c++17 assignment2_huffman_encoding.cpp -o ass2
./ass2
```
**Sample Input:**
```text
6
a 5
b 9
c 12
d 13
e 16
f 45
```
**Expected Output:**
```text
f: 0 (1 bit)
c: 100 (3 bits)
d: 101 (3 bits)
a: 1100 (4 bits)
b: 1101 (4 bits)
e: 111 (3 bits)
```
*(Notice 'f' with highest frequency 45 gets the shortest 1-bit code '0')*
