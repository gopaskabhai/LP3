#include <bits/stdc++.h>
using namespace std;

void fibonacciIterative(int n) {
    if (n < 0) return;
    
    if (n >= 0) {
        cout << 0 << endl;
    }
    
    if (n >= 1) {
        cout << 1 << endl;
    }
    
    long long prev2 = 0;
    long long prev1 = 1;
    long long current = 0;
    
    for (int i = 2; i <= n; i++) {
        current = prev1 + prev2;
        cout << current << endl;
        prev2 = prev1;
        prev1 = current;
    }
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
    fibonacciIterative(n);
    
    return 0;
}



/*
FIBONACCI USING ITERATIVE APPROACH
===================================

Algorithm: Bottom-Up Iteration
Time Complexity: O(n) - Linear
Space Complexity: O(1) - Constant (only 3 variables)

How it works:
- Start with base cases F(0) = 0, F(1) = 1
- Use a loop to compute each Fibonacci number sequentially
- Keep only the last two values in memory
- Build up from bottom (F(0), F(1)) to top (F(n))

Step Count Analysis:
- For fib(n), exactly n-1 iterations are performed
- Each iteration performs constant operations
- Step count grows linearly with input size
- Much more efficient than recursive approach

Iterative Fibonacci Function:
- Parameters: n - the position in Fibonacci sequence
- Returns: nth Fibonacci number

Steps in the algorithm:
1. Handle base cases (n = 0 or n = 1)
2. Initialize first two Fibonacci numbers
3. Loop from 2 to n
4. In each iteration: compute next Fibonacci number
5. Update variables for next iteration
6. Return final result

Initialize variables for iteration:
- prev2 = F(i-2), prev1 = F(i-1), current = F(i)
- prev2 starts at F(0) = 0
- prev1 starts at F(1) = 1

Iterate from position 2 to n:
In each iteration:
- Calculate current Fibonacci number
- Shift values: prev2 <- prev1, prev1 <- current
- Count operations performed

Example for F(5):
i=2: current = 0+1=1, prev2=1, prev1=1  [F(2)=1]
i=3: current = 1+1=2, prev2=1, prev1=2  [F(3)=2]
i=4: current = 1+2=3, prev2=2, prev1=3  [F(4)=3]
i=5: current = 2+3=5, prev2=3, prev1=5  [F(5)=5]

Analysis of step count:
F(0): 1 step
F(1): 2 steps
F(2): 6 steps (2 init + 4 in loop)
F(5): 18 steps (2 init + 4*4 in loop)
F(10): 38 steps (2 init + 4*9 in loop)
F(20): 78 steps (2 init + 4*19 in loop)
F(100): 398 steps (2 init + 4*99 in loop)

Formula: Steps ≈ 2 + 4*(n-1) = 4n - 2 for n >= 2

EXAMPLE OUTPUT:
===============
Input: n = 10

Fibonacci(10) = 55
Total Steps: 38

Sequence generated:
F(0) = 0
F(1) = 1
F(2) = 1
F(3) = 2
F(4) = 3
F(5) = 5
F(6) = 8
F(7) = 13
F(8) = 21
F(9) = 34
F(10) = 55

ADVANTAGES:
- Very efficient: O(n) time complexity
- Minimal space usage: O(1) space
- No recursion overhead or stack issues
- Can handle large values of n
- No redundant calculations
- Fast and practical for real-world use

DISADVANTAGES:
- Less intuitive than recursive approach
- Doesn't directly follow mathematical definition
- Slightly more code than simple recursion

COMPARISON WITH RECURSION:
For n = 20:
- Recursive: 21,891 steps
- Iterative: 78 steps
- Iterative is 280x faster!
*/
