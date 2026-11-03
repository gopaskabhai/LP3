/**
 * Assignment 4: 0/1 Knapsack Problem using Dynamic Programming
 * 
 * Problem Statement:
 * Write a program to solve a 0-1 Knapsack problem using dynamic programming 
 * or branch and bound strategy.
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter number of items: 4
 *   Enter weight and profit/value for item 1: 2 12
 *   Enter weight and profit/value for item 2: 1 10
 *   Enter weight and profit/value for item 3: 3 20
 *   Enter weight and profit/value for item 4: 2 15
 *   Enter knapsack maximum capacity: 5
 * 
 * Expected Output:
 *   DP Table displayed.
 *   Selected Items: Item 4, Item 2, Item 1 (Total Weight = 5)
 *   Maximum Profit / Value: 37
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * Solves 0/1 Knapsack using Dynamic Programming (Bottom-Up Tabulation)
 * 
 * Recurrence Relation:
 * DP[i][w] = DP[i-1][w]                                    (if wt[i] > w)
 * DP[i][w] = max(DP[i-1][w], val[i] + DP[i-1][w - wt[i]])  (if wt[i] <= w)
 * 
 * Time Complexity: O(n * W) where n is number of items, W is knapsack capacity
 * Space Complexity: O(n * W) for the 2D DP table (can be optimized to O(W))
 */
void solve01Knapsack(int n, int W, const vector<int>& weights, const vector<int>& values) {
    // DP table: dp[i][w] represents max profit considering first i items with capacity w
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    // Fill table in bottom-up manner
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            // Option 1: Exclude the current item
            dp[i][w] = dp[i - 1][w];

            // Option 2: Include the current item if capacity permits
            if (weights[i] <= w) {
                dp[i][w] = max(dp[i][w], values[i] + dp[i - 1][w - weights[i]]);
            }
        }
    }

    // Print DP Table for external examiner demonstration
    cout << "\n================ Dynamic Programming Table ================\n";
    cout << setw(8) << "Items\\W";
    for (int w = 0; w <= W; w++) cout << setw(5) << w;
    cout << "\n" << string(10 + 5 * (W + 1), '-') << "\n";

    for (int i = 0; i <= n; i++) {
        if (i == 0) cout << setw(8) << "0 (Base)";
        else cout << setw(8) << ("Item " + to_string(i));
        for (int w = 0; w <= W; w++) {
            cout << setw(5) << dp[i][w];
        }
        cout << "\n";
    }
    cout << "===========================================================\n";

    // Backtrack to find which items were selected
    cout << "\nSelected Items in the Optimal Solution:\n";
    int current_w = W;
    int total_weight_used = 0;
    vector<int> selected_items;

    for (int i = n; i > 0; i--) {
        // If profit differs from dp[i-1][current_w], the item was selected
        if (dp[i][current_w] != dp[i - 1][current_w]) {
            selected_items.push_back(i);
            total_weight_used += weights[i];
            current_w -= weights[i];
        }
    }

    for (int i = 0; i < (int)selected_items.size(); i++) {
        int idx = selected_items[i];
        cout << "  - Item " << idx << " (Weight = " << weights[idx] 
             << ", Value = " << values[idx] << ")\n";
    }

    cout << "-----------------------------------------------------------\n";
    cout << "Total Weight Used   : " << total_weight_used << " / " << W << "\n";
    cout << "Maximum Value/Profit: " << dp[n][W] << "\n";
    cout << "===========================================================\n";
}

int main() {
    cout << "========================================================\n";
    cout << "   DAA Lab Assignment 4: 0/1 Knapsack (Dynamic Prog)    \n";
    cout << "========================================================\n";

    int n, W;
    cout << "Enter number of items: ";
    cin >> n;

    if (n <= 0) {
        cout << "Number of items must be positive.\n";
        return 0;
    }

    // 1-based indexing for convenience
    vector<int> weights(n + 1, 0);
    vector<int> values(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        cout << "Enter weight and profit/value for item " << i << " (e.g. '2 12'): ";
        cin >> weights[i] >> values[i];
    }

    cout << "Enter knapsack maximum capacity: ";
    cin >> W;

    if (W <= 0) {
        cout << "Capacity must be positive.\n";
        return 0;
    }

    solve01Knapsack(n, W, weights, values);

    return 0;
}
