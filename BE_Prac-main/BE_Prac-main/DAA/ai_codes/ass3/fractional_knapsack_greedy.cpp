#include <bits/stdc++.h>
using namespace std;

struct Item {
    int weight;
    int value;
    double ratio;
};

bool comparison(Item a, Item b) {
    return a.ratio > b.ratio;
}

void fractionalKnapsack(Item items[], int n, int capacity) {
    sort(items, items + n, comparison);
    
    double totalValue = 0.0;
    int remainingCapacity = capacity;
    
    cout << "\nKnapsack Filling Process:\n";
    cout << "-------------------------\n";
    cout << "Capacity: " << capacity << "\n\n";
    
    for (int i = 0; i < n; i++) {
        if (remainingCapacity == 0) {
            break;
        }
        
        if (items[i].weight <= remainingCapacity) {
            remainingCapacity -= items[i].weight;
            totalValue += items[i].value;
            cout << "Item " << (i + 1) << ": Take full (Weight: " << items[i].weight 
                 << ", Value: " << items[i].value << ", Fraction: 1.0)\n";
        } else {
            double fraction = (double)remainingCapacity / items[i].weight;
            totalValue += items[i].value * fraction;
            cout << "Item " << (i + 1) << ": Take partial (Weight: " << remainingCapacity 
                 << "/" << items[i].weight << ", Value: " << (items[i].value * fraction) 
                 << ", Fraction: " << fraction << ")\n";
            remainingCapacity = 0;
        }
    }
    
    cout << "\n--- RESULTS ---\n";
    cout << "Maximum value in knapsack: " << totalValue << "\n";
}

int main() {
    int n, capacity;
    
    cout << "Enter number of items: ";
    cin >> n;
    
    cout << "Enter knapsack capacity: ";
    cin >> capacity;
    
    Item *items = new Item[n];
    
    cout << "\nEnter item details (Weight Value):\n";
    for (int i = 0; i < n; i++) {
        cout << "Item " << (i + 1) << ": ";
        cin >> items[i].weight >> items[i].value;
        items[i].ratio = (double)items[i].value / items[i].weight;
    }
    
    cout << "\nInput Items:\n";
    cout << "Item\tWeight\tValue\tValue/Weight\n";
    for (int i = 0; i < n; i++) {
        cout << (i + 1) << "\t" << items[i].weight << "\t" << items[i].value 
             << "\t" << fixed << setprecision(2) << items[i].ratio << "\n";
    }
    
    fractionalKnapsack(items, n, capacity);
    
    delete[] items;
    return 0;
}


/*
FRACTIONAL KNAPSACK PROBLEM - GREEDY METHOD
============================================

Algorithm: Greedy Approach
Time Complexity: O(n log n) where n is number of items
Space Complexity: O(1) excluding input storage

Problem Statement:
Given:
- n items, each with a weight and value
- A knapsack with maximum capacity W
- Items can be broken into fractions

Goal: Maximize the total value in the knapsack
Constraint: Total weight should not exceed capacity W

Difference from 0/1 Knapsack:
- 0/1 Knapsack: Can only take item completely or leave it (no fractions)
- Fractional Knapsack: Can take fraction of an item

Greedy Strategy:
1. Calculate value-to-weight ratio for each item
2. Sort items by this ratio in descending order
3. Fill knapsack with items having highest ratio first
4. If item doesn't fit completely, take fractional part

Why Greedy Works Here:
- For fractional knapsack, greedy gives optimal solution
- Taking highest value/weight ratio first maximizes total value
- Each local choice (highest ratio) leads to global optimum
- Note: For 0/1 knapsack, greedy does NOT guarantee optimal solution

Algorithm Steps:
1. Calculate value/weight ratio for all items
2. Sort items by ratio in descending order
3. Initialize total value = 0, remaining capacity = W
4. For each item in sorted order:
   - If item fits completely: take entire item
   - If item doesn't fit: take fraction that fits
   - Update total value and remaining capacity
5. Return total value

Example:
Items: [(weight=10, value=60), (weight=20, value=100), (weight=30, value=120)]
Capacity: 50

Step 1: Calculate ratios
Item 1: 60/10 = 6.0
Item 2: 100/20 = 5.0
Item 3: 120/30 = 4.0

Step 2: Sort by ratio (descending)
[(10,60,6.0), (20,100,5.0), (30,120,4.0)]

Step 3: Fill knapsack
- Take Item 1 completely: weight=10, value=60, remaining=40
- Take Item 2 completely: weight=20, value=100, remaining=20
- Take Item 3 partially: weight=20/30, value=120*(20/30)=80, remaining=0

Total Value = 60 + 100 + 80 = 240

Mathematical Formula:
Maximum Value = Σ(value[i] * fraction[i]) for all items i
where fraction[i] = 1 if item fits completely
               = remaining_capacity / weight[i] if partial

Time Complexity Analysis:
- Calculating ratios: O(n)
- Sorting items: O(n log n)
- Filling knapsack: O(n)
- Overall: O(n log n)

Space Complexity Analysis:
- Item array: O(n)
- No extra space for algorithm
- Overall: O(n) for storage, O(1) for algorithm

Comparison with 0/1 Knapsack:
Property              | Fractional Knapsack | 0/1 Knapsack
---------------------|---------------------|----------------
Item splitting       | Allowed             | Not allowed
Greedy solution      | Optimal             | Not optimal
Algorithm            | Greedy              | Dynamic Programming
Time complexity      | O(n log n)          | O(nW)
Space complexity     | O(1)                | O(nW)

Advantages:
- Simple and intuitive greedy approach
- Efficient O(n log n) time complexity
- Guaranteed optimal solution
- Easy to implement
- No need for complex DP tables

Disadvantages:
- Only works for fractional variant
- Requires sorting which adds overhead
- Cannot be used for 0/1 knapsack (discrete items)

Real-world Applications:
- Resource allocation in continuous quantities
- Investment portfolio optimization
- Fluid or powder packaging
- Bandwidth allocation in networks
- Fuel loading optimization
- Chemical mixture problems
- Continuous resource distribution

Proof of Correctness:
Greedy choice property:
- Let items be sorted by value/weight ratio: r1 ≥ r2 ≥ ... ≥ rn
- Suppose optimal solution takes fraction f1 of item 1
- If f1 < 1 and some item j with rj < r1 is taken more, we can:
  - Replace item j with item 1 to increase value (contradiction)
- Therefore, taking maximum of item 1 first is optimal

Optimal substructure:
- After taking item 1, remaining problem is same with reduced capacity
- Optimal solution to original = item 1 + optimal solution to subproblem

Input Format:
First line: Number of items (n)
Second line: Knapsack capacity (W)
Next n lines: Weight Value for each item

Example Input:
3
50
10 60
20 100
30 120

Expected Output:
Item 1: Take full (Weight: 10, Value: 60, Fraction: 1.0)
Item 2: Take full (Weight: 20, Value: 100, Fraction: 1.0)
Item 3: Take partial (Weight: 20/30, Value: 80, Fraction: 0.666667)

Maximum value in knapsack: 240
*/
