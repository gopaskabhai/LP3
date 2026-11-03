/**
 * Assignment 1: Fibonacci Numbers (Recursive and Non-Recursive) with Step Count & Complexity Analysis
 * 
 * Problem Statement:
 * Write a program non-recursive and recursive program to calculate Fibonacci numbers 
 * and analyze their time and space complexity.
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter n (term index, 0-indexed): 6
 * 
 * Expected Output:
 *   --- Iterative (Non-Recursive) Approach ---
 *   Fibonacci(6) = 8
 *   Step count (basic operations): 19
 *   Time Complexity: O(n)
 *   Space Complexity: O(1)
 * 
 *   --- Recursive Approach ---
 *   Fibonacci(6) = 8
 *   Total recursive function calls: 25
 *   Step count (operations): 50
 *   Time Complexity: O(2^n)
 *   Space Complexity: O(n) [Call stack depth]
 * ============================================================================
 */

#include <iostream>
#include <iomanip>

using namespace std;

// Global step counters for recursive analysis
long long recursive_steps = 0;
long long recursive_calls = 0;

/**
 * Iterative (Non-Recursive) Fibonacci Calculation
 * Calculates F(n) where F(0) = 0, F(1) = 1, F(2) = 1, ...
 * 
 * Time Complexity: O(n) - Single loop running n times
 * Space Complexity: O(1) - Only a few scalar variables (a, b, c) used
 */
long long fibonacciIterative(int n, long long &steps) {
    steps = 0;
    
    // Step: check base cases
    steps++;
    if (n <= 0) return 0;
    steps++;
    if (n == 1) return 1;

    long long a = 0; steps++; // F(0)
    long long b = 1; steps++; // F(1)
    long long c = 0; steps++;

    // Loop runs from 2 to n (n - 1 iterations)
    for (int i = 2; i <= n; i++) {
        steps++; // loop comparison and increment
        c = a + b; steps++;
        a = b;     steps++;
        b = c;     steps++;
    }
    steps++; // loop termination check
    return b;
}

/**
 * Recursive Fibonacci Calculation
 * Calculates F(n) by calling fibonacciRecursive(n-1) + fibonacciRecursive(n-2)
 * 
 * Time Complexity: O(2^n) (More precisely O(1.618^n), exponential)
 * Space Complexity: O(n) (Maximum call stack depth is n)
 */
long long fibonacciRecursive(int n) {
    recursive_calls++;
    recursive_steps++; // Function entry & condition check

    if (n <= 0) {
        recursive_steps++;
        return 0;
    }
    if (n == 1) {
        recursive_steps++;
        return 1;
    }

    recursive_steps++; // Addition step
    return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
}

int main() {
    int n;
    cout << "========================================================\n";
    cout << "   DAA Lab Assignment 1: Fibonacci (Iterative vs Recursive)\n";
    cout << "========================================================\n";
    cout << "Enter n (term index, e.g., 0 for F(0), 6 for F(6)): ";
    cin >> n;

    if (n < 0) {
        cout << "Error: Please enter a non-negative integer (n >= 0).\n";
        return 1;
    }

    // 1. Non-recursive (Iterative) execution
    long long iter_steps = 0;
    long long iter_result = fibonacciIterative(n, iter_steps);

    cout << "\n---------------- Non-Recursive (Iterative) ----------------\n";
    cout << "Fibonacci(" << n << ") = " << iter_result << "\n";
    cout << "Step Count (elementary operations) : " << iter_steps << "\n";
    cout << "Time Complexity                    : O(n)\n";
    cout << "Space Complexity                   : O(1)\n";

    // 2. Recursive execution
    // Warning: For n > 35, pure recursion becomes noticeably slow due to O(2^n) complexity
    if (n > 40) {
        cout << "\n[Skipping pure recursive calculation for n > 40 to avoid hanging (exponential time)]\n";
    } else {
        recursive_steps = 0;
        recursive_calls = 0;
        long long rec_result = fibonacciRecursive(n);

        cout << "\n---------------- Recursive Approach ----------------\n";
        cout << "Fibonacci(" << n << ") = " << rec_result << "\n";
        cout << "Total Recursive Function Calls     : " << recursive_calls << "\n";
        cout << "Step Count (operations)            : " << recursive_steps << "\n";
        cout << "Time Complexity                    : O(2^n) [Golden Ratio phi^n]\n";
        cout << "Space Complexity                   : O(n) [Call stack depth]\n";
    }

    // Print Fibonacci series up to n
    cout << "\nFibonacci Series up to " << n << " terms:\n";
    long long f0 = 0, f1 = 1;
    for (int i = 0; i <= n; i++) {
        if (i == 0) cout << 0 << " ";
        else if (i == 1) cout << 1 << " ";
        else {
            long long fn = f0 + f1;
            cout << fn << " ";
            f0 = f1;
            f1 = fn;
        }
    }
    cout << "\n========================================================\n";

    return 0;
}
