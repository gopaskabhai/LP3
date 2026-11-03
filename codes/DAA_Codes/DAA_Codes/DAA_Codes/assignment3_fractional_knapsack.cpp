/**
 * Assignment 3: Fractional Knapsack Problem using Greedy Strategy
 * 
 * Problem Statement:
 * Write a program to solve a fractional Knapsack problem using a greedy method.
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter number of items: 3
 *   Enter item 1 (weight value): 10 60
 *   Enter item 2 (weight value): 20 100
 *   Enter item 3 (weight value): 30 120
 *   Enter knapsack capacity: 50
 * 
 * Expected Output:
 *   Item 1: 100% taken (Weight: 10, Value: 60)
 *   Item 2: 100% taken (Weight: 20, Value: 100)
 *   Item 3: 66.67% taken (Weight: 20 of 30, Value: 80)
 *   Maximum Total Value: 240
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// Structure to represent an item with weight and value
struct Item {
    int id;          // Original item number
    double weight;   // Item weight
    double value;    // Item profit/value
    double ratio;    // Value-to-weight ratio (value / weight)
};

/**
 * Greedy Choice Comparator:
 * Sort items in descending order of value-to-weight ratio.
 * This ensures we greedily pick the items giving the most value per unit weight.
 */
bool compareByRatio(const Item &a, const Item &b) {
    return a.ratio > b.ratio;
}

/**
 * Solves Fractional Knapsack using Greedy Algorithm
 * 
 * Time Complexity: O(n log n) - dominated by sorting the items
 * Space Complexity: O(1) auxiliary space (or O(n) to store items)
 */
void fractionalKnapsack(int n, double capacity, vector<Item>& items) {
    // Calculate ratio for each item
    for (int i = 0; i < n; i++) {
        items[i].ratio = items[i].value / items[i].weight;
    }

    // Sort items by ratio in descending order
    sort(items.begin(), items.end(), compareByRatio);

    double totalValue = 0.0;
    double currentWeight = 0.0;
    double remainingCapacity = capacity;

    cout << "\n================ Selection Details ================\n";
    cout << fixed << setprecision(2);

    for (int i = 0; i < n && remainingCapacity > 0; i++) {
        // Case 1: Can take the entire item
        if (items[i].weight <= remainingCapacity) {
            remainingCapacity -= items[i].weight;
            currentWeight += items[i].weight;
            totalValue += items[i].value;
            cout << "Item " << items[i].id << ": Taken 100.00% (Weight: " 
                 << items[i].weight << ", Value: " << items[i].value << ")\n";
        } 
        // Case 2: Take fractional portion of the item to fill remaining capacity
        else {
            double fraction = remainingCapacity / items[i].weight;
            double fractionValue = items[i].value * fraction;
            totalValue += fractionValue;
            currentWeight += remainingCapacity;
            cout << "Item " << items[i].id << ": Taken " << (fraction * 100.0) 
                 << "% (Weight: " << remainingCapacity << " of " << items[i].weight 
                 << ", Value: " << fractionValue << ")\n";
            remainingCapacity = 0; // Knapsack full
        }
    }

    cout << "---------------------------------------------------\n";
    cout << "Total Weight in Knapsack : " << currentWeight << " / " << capacity << "\n";
    cout << "Maximum Total Value      : " << totalValue << "\n";
    cout << "===================================================\n";
}

int main() {
    cout << "========================================================\n";
    cout << "   DAA Lab Assignment 3: Fractional Knapsack (Greedy)   \n";
    cout << "========================================================\n";

    int n;
    cout << "Enter number of items: ";
    cin >> n;

    if (n <= 0) {
        cout << "Number of items must be greater than 0.\n";
        return 0;
    }

    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].id = i + 1;
        cout << "Enter weight and value of item " << i + 1 << " (e.g. '10 60'): ";
        cin >> items[i].weight >> items[i].value;
    }

    double capacity;
    cout << "Enter knapsack capacity: ";
    cin >> capacity;

    if (capacity <= 0) {
        cout << "Knapsack capacity must be positive.\n";
        return 0;
    }

    fractionalKnapsack(n, capacity, items);

    return 0;
}
