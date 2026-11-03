#include <bits/stdc++.h>
using namespace std;

int fibonacciRecursive(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
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
    for (int i = 0; i <= n; i++) {
        cout << fibonacciRecursive(i) << endl;
    }

    return 0;
}



/*
FIBONACCI USING RECURSIVE APPROACH
===================================

Algorithm: Basic Recursion
Time Complexity: O(2^n) - Exponential (Very Slow)
Space Complexity: O(n) - Recursion stack depth

How it works:
- Base cases: fib(0) = 0, fib(1) = 1
- Recursive case: fib(n) = fib(n-1) + fib(n-2)
- Creates a binary tree of recursive calls
- Many redundant calculations (same values computed multiple times)

Step Count Analysis:
- For fib(n), approximately 2^n function calls are made
- Each call performs constant operations (comparison, addition)
- Step count grows exponentially with input size

Recursive Fibonacci Function:
- Parameters: n - the position in Fibonacci sequence
- Returns: nth Fibonacci number

Steps in each call:
1. Increment step counter
2. Check base cases (n == 0 or n == 1)
3. If base case, return the value
4. Otherwise, make two recursive calls
5. Add the results and return

Recursive case explanation:
- F(n) = F(n-1) + F(n-2)
- This creates TWO recursive calls, leading to exponential growth
- Example for F(5):
                   F(5)
                  /    \
              F(4)      F(3)
             /   \      /   \
          F(3)  F(2)  F(2) F(1)
         /  \   / \   / \
      F(2) F(1)...  ...

Analysis of step count:
F(0): 1 step
F(1): 1 step
F(2): 3 steps
F(3): 5 steps
F(4): 9 steps
F(5): 15 steps
F(10): 177 steps
F(20): 21,891 steps
F(30): 2,692,537 steps

WARNING: Very inefficient for large n!

EXAMPLE OUTPUT:
===============
Input: n = 5

Fibonacci(5) = 5
Total Steps: 15

Explanation:
F(5) calls F(4) and F(3)
F(4) calls F(3) and F(2)
F(3) calls F(2) and F(1)  [called twice]
F(2) calls F(1) and F(0)  [called three times]
Total: 15 function calls

ADVANTAGES:
- Simple and easy to understand
- Direct translation of mathematical definition
- Good for teaching recursion concepts

DISADVANTAGES:
- Extremely slow for large n (exponential time)
- Many redundant calculations
- Stack overflow risk for very large n
- Not practical for n > 30-40
*/
