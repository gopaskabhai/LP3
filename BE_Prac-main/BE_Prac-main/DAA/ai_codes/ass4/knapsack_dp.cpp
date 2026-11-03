#include <bits/stdc++.h>
using namespace std;

int knapsackDP(int weights[], int values[], int n, int capacity) {
    int **dp = new int*[n + 1];
    for (int i = 0; i <= n; i++) {
        dp[i] = new int[capacity + 1];
    }
    
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (weights[i - 1] <= w) {
                dp[i][w] = max(values[i - 1] + dp[i - 1][w - weights[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    
    cout << "\nDP Table:\n";
    cout << "Item\\Cap\t";
    for (int w = 0; w <= capacity; w++) {
        cout << w << "\t";
    }
    cout << "\n";
    
    for (int i = 0; i <= n; i++) {
        cout << i << "\t\t";
        for (int w = 0; w <= capacity; w++) {
            cout << dp[i][w] << "\t";
        }
        cout << "\n";
    }
    
    int result = dp[n][capacity];
    
    cout << "\nItems selected:\n";
    int w = capacity;
    for (int i = n; i > 0 && result > 0; i--) {
        if (result != dp[i - 1][w]) {
            cout << "Item " << i << " (Weight: " << weights[i - 1] 
                 << ", Value: " << values[i - 1] << ")\n";
            result -= values[i - 1];
            w -= weights[i - 1];
        }
    }
    
    int maxValue = dp[n][capacity];
    
    for (int i = 0; i <= n; i++) {
        delete[] dp[i];
    }
    delete[] dp;
    
    return maxValue;
}

int main() {
    int n, capacity;
    
    cout << "Enter number of items: ";
    cin >> n;
    
    cout << "Enter knapsack capacity: ";
    cin >> capacity;
    
    int *weights = new int[n];
    int *values = new int[n];
    
    cout << "\nEnter item details (Weight Value):\n";
    for (int i = 0; i < n; i++) {
        cout << "Item " << (i + 1) << ": ";
        cin >> weights[i] >> values[i];
    }
    
    cout << "\nInput Items:\n";
    cout << "Item\tWeight\tValue\n";
    for (int i = 0; i < n; i++) {
        cout << (i + 1) << "\t" << weights[i] << "\t" << values[i] << "\n";
    }
    
    int maxValue = knapsackDP(weights, values, n, capacity);
    
    cout << "\n--- RESULTS ---\n";
    cout << "Maximum value: " << maxValue << "\n";
    
    delete[] weights;
    delete[] values;
    return 0;
}


/*
0-1 KNAPSACK PROBLEM - DYNAMIC PROGRAMMING APPROACH
====================================================

Algorithm: Dynamic Programming (Bottom-Up Tabulation)
Time Complexity: O(n*W) where n = items, W = capacity
Space Complexity: O(n*W) for DP table

Problem Statement:
Given:
- n items, each with a weight and value
- A knapsack with maximum capacity W
- Each item can be taken ONLY ONCE (0 or 1)

Goal: Maximize total value without exceeding capacity
Constraint: Cannot take fractions of items

Key Difference from Fractional Knapsack:
- Fractional: Can take partial items → Greedy works → O(n log n)
- 0-1: Must take complete item or leave it → DP needed → O(nW)

Why Greedy Doesn't Work:
Consider: Items = [(5,10), (4,40), (6,30), (3,50)], Capacity = 10
Greedy (by value/weight): Takes item 4 (ratio=16.67) and 2 (ratio=10) → Value = 90
Optimal: Takes items 2 and 3 (weights 4+6=10) → Value = 70
Wait, greedy won! Try another: [(10,60), (20,100), (30,120)], Capacity = 50, no fractions
Greedy: Takes item 1 (6.0) + 2 (5.0) = 160, can't fit 3
Optimal: Takes all except... wait this is same!
Better example: [(1,1), (2,6), (5,18), (6,22), (7,28)], Capacity = 11
Greedy by ratio: (7,28) + (1,1) = 29 with weight 8
Optimal: (5,18) + (6,22) = 40 with weight 11
This shows greedy fails for 0-1 knapsack!

Dynamic Programming Approach:
Define: dp[i][w] = maximum value using first i items with capacity w

Recurrence Relation:
dp[i][w] = {
    0                                           if i=0 or w=0 (base case)
    dp[i-1][w]                                  if weight[i-1] > w (can't include item)
    max(dp[i-1][w],                             (don't include item)
        value[i-1] + dp[i-1][w-weight[i-1]])   (include item)
}

Intuition:
For each item i and capacity w, we have 2 choices:
1. Don't include item i: value = dp[i-1][w]
2. Include item i (if it fits): value = value[i] + dp[i-1][w-weight[i]]
Take maximum of both choices

Algorithm Steps:
1. Create DP table of size (n+1) × (W+1)
2. Initialize first row and column to 0 (base cases)
3. Fill table row by row using recurrence relation
4. dp[n][W] contains the maximum value
5. Backtrack to find which items were selected

Example:
Items: [(2,12), (1,10), (3,20), (2,15)]
Capacity: 5

DP Table:
     0  1  2  3  4  5
0    0  0  0  0  0  0
1    0  0 12 12 12 12
2    0 10 12 22 22 22
3    0 10 12 22 30 32
4    0 10 12 22 30 37

Backtracking from dp[4][5]=37:
- dp[4][5]=37 ≠ dp[3][5]=32 → Item 4 included
- Move to dp[3][5-2]=dp[3][3]=22
- dp[3][3]=22 ≠ dp[2][3]=22 → Item 3 not included
- dp[2][3]=22 ≠ dp[1][3]=12 → Item 2 included
- Move to dp[1][3-1]=dp[1][2]=12
- dp[1][2]=12 ≠ dp[0][2]=0 → Item 1 included

Selected: Items 1, 2, 4 with total value = 37

Time Complexity Analysis:
- Building DP table: O(n*W)
- Each cell computed in O(1)
- Backtracking: O(n)
- Overall: O(n*W) - Pseudo-polynomial time

Space Complexity Analysis:
- DP table: O(n*W)
- Can be optimized to O(W) using 1D array
- Input storage: O(n)

Space Optimization:
Instead of 2D table, use 1D array:
- Process items one by one
- Update array from right to left (to avoid overwriting needed values)
- Reduces space to O(W)

Advantages:
- Guarantees optimal solution
- Can reconstruct which items to take
- Efficient for reasonable W values
- No recursion overhead

Disadvantages:
- Pseudo-polynomial time (depends on W, not just n)
- O(n*W) space can be large for big W
- Not suitable when W is very large (e.g., 10^9)
- Slower than greedy for fractional variant

When to Use:
- When items cannot be divided (discrete items)
- When W is reasonable (< 10^6)
- When optimal solution is required
- When you need to know which items to select

Real-world Applications:
- Resource allocation with discrete units
- Budget optimization with fixed items
- Cargo loading with containers
- Investment selection with fixed amounts
- Memory allocation in systems
- Project selection with budget constraint

Comparison: DP vs Branch & Bound
Property          | DP              | Branch & Bound
-----------------|-----------------|------------------
Approach         | Bottom-up       | Top-down with pruning
Time (worst)     | O(n*W)          | O(2^n)
Time (average)   | O(n*W)          | Often much better
Space            | O(n*W)          | O(n)
Guaranteed time  | Yes             | No
Best for         | Medium W        | Small n or good bounds

Input Format:
First line: Number of items (n)
Second line: Knapsack capacity (W)
Next n lines: Weight Value for each item

Example Input:
4
5
2 12
1 10
3 20
2 15

Expected Output:
DP Table showing computation process
Items selected with weights and values
Maximum value: 37
*/
