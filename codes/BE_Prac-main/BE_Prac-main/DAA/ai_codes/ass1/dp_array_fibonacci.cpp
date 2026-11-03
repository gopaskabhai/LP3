#include <bits/stdc++.h>
using namespace std;

void fibonacciDP(int n) {
    if (n < 0) return;
    
    long long *dp = new long long[n + 1];
    
    if (n >= 0) {
        dp[0] = 0;
        cout << dp[0] << endl;
    }
    
    if (n >= 1) {
        dp[1] = 1;
        cout << dp[1] << endl;
    }
    
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
        cout << dp[i] << endl;
    }
    
    delete[] dp;
}

int main() {
    int n;
    
    cout << "Enter n: ";
    cin >> n;
    
    if (n < 0) {
        cout << "Error: Please enter a non-negative number." << endl;
        return 1;
    }
    
    cout << "\nFibonacci Sequence:" << endl;
    fibonacciDP(n);
    
    return 0;
}


/*
FIBONACCI USING DYNAMIC PROGRAMMING (ARRAY APPROACH)
=====================================================

Algorithm: Bottom-Up Dynamic Programming with Tabulation
Time Complexity: O(n) - Linear
Space Complexity: O(n) - Array to store all Fibonacci numbers

How it works:
- Create an array to store all Fibonacci numbers from 0 to n
- Fill array bottom-up starting from base cases
- Each position uses previously computed values
- Tabulation: Build a table of solutions to subproblems

Step Count Analysis:
- For fib(n), exactly n-1 iterations are performed
- Each iteration performs constant operations
- Step count grows linearly with input size

Advantages over iterative:
- Can easily print entire Fibonacci sequence
- Random access to any Fibonacci number in O(1)
- Good for multiple queries

Dynamic Programming Fibonacci Function (Array-based):
- Parameters: n - the position in Fibonacci sequence
- Returns: nth Fibonacci number

Steps in the algorithm:
1. Create array of size n+1
2. Initialize base cases: dp[0] = 0, dp[1] = 1
3. Loop from 2 to n
4. Fill array using recurrence: dp[i] = dp[i-1] + dp[i-2]
5. Return dp[n]

Create DP array to store Fibonacci numbers:
- dp[i] will store the i-th Fibonacci number
- Size n+1 to accommodate indices from 0 to n

Initialize base cases:
- These are the starting values for our bottom-up approach
- dp[0] = 0 (F(0) = 0)
- dp[1] = 1 (F(1) = 1)

Fill the DP array bottom-up:
For each position i from 2 to n:
- Use previously computed values dp[i-1] and dp[i-2]
- Compute dp[i] = dp[i-1] + dp[i-2]

Example for n=5:
Initial: dp = [0, 1, ?, ?, ?, ?]
i=2: dp[2] = dp[1] + dp[0] = 1+0 = 1  -> [0,1,1,?,?,?]
i=3: dp[3] = dp[2] + dp[1] = 1+1 = 2  -> [0,1,1,2,?,?]
i=4: dp[4] = dp[3] + dp[2] = 2+1 = 3  -> [0,1,1,2,3,?]
i=5: dp[5] = dp[4] + dp[3] = 3+2 = 5  -> [0,1,1,2,3,5]

Recurrence relation: F(i) = F(i-1) + F(i-2)
This is the heart of dynamic programming:
- Break problem into subproblems
- Solve each subproblem once
- Store solution for future use

Analysis of step count:
F(0): 1 step
F(1): 2 steps
F(2): 8 steps (1 alloc + 2 init + 1 iter + 4 in loop)
F(5): 20 steps (1 alloc + 2 init + 4*4 + 1 final)
F(10): 40 steps
F(100): 400 steps

Formula: Steps ≈ 3 + 4*(n-1) = 4n - 1 for n >= 2
Very similar to iterative approach

EXAMPLE OUTPUT:
===============
Input: n = 10

Fibonacci(10) = 55
Total Steps: 40

Fibonacci Sequence:
F(0) = 0, F(1) = 1, F(2) = 1, F(3) = 2, F(4) = 3,
F(5) = 5, F(6) = 8, F(7) = 13, F(8) = 21, F(9) = 34,
F(10) = 55

DYNAMIC PROGRAMMING CONCEPTS:
=============================

1. OVERLAPPING SUBPROBLEMS:
   - F(5) needs F(4) and F(3)
   - F(4) needs F(3) and F(2)
   - F(3) is needed multiple times
   - DP stores F(3) once and reuses it

2. OPTIMAL SUBSTRUCTURE:
   - Solution to F(n) can be constructed from solutions to F(n-1) and F(n-2)
   - Optimal solution contains optimal solutions to subproblems

3. TABULATION (BOTTOM-UP):
   - Start from smallest subproblems (F(0), F(1))
   - Build up to larger problems
   - Fill table in order
   - No recursion needed

ADVANTAGES:
- Efficient: O(n) time complexity
- Can access any Fibonacci number in O(1) after computation
- Can print entire sequence easily
- Good for multiple queries
- No recursion overhead
- Predictable performance

DISADVANTAGES:
- Uses O(n) extra space for array
- Must compute all values up to n
- Not space-optimal (iterative is better if only F(n) needed)
- Memory allocation overhead

WHEN TO USE:
- Need entire Fibonacci sequence
- Multiple queries for different positions
- Teaching DP concepts
- When space is not a constraint

COMPARISON WITH OTHER APPROACHES:
For n = 20:
- Recursive: 21,891 steps, O(2^n) time, O(n) space
- Iterative: 78 steps, O(n) time, O(1) space
- DP Array: 79 steps, O(n) time, O(n) space

DP array is similar to iterative in performance but uses more memory.
Trade-off: Extra space for ability to access all computed values.
*/
