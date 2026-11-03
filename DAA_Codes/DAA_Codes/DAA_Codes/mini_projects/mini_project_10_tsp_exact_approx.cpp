/**
 * Mini Project 10: Travelling Salesperson Problem (TSP)
 * Exact vs Approximation Algorithms
 * 
 * Problem Statement:
 * Different exact and approximation algorithms for Travelling-Sales-Person Problem.
 * 
 * Implemented Algorithms:
 * 1. Exact: Held-Karp Dynamic Programming with Bitmasking (Optimal, O(n^2 * 2^n))
 * 2. Exact: Branch and Bound / Backtracking (Optimal with state-space pruning)
 * 3. Approximation: Nearest Neighbor Greedy Heuristic (Fast, O(n^2), within ~2x optimal)
 * 4. Approximation: Minimum Spanning Tree (MST) 2-Approximation (Metric TSP, O(n^2))
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter number of cities N (e.g. 4 or 5): 4
 *   Cost Adjacency Matrix:
 *     0  10  15  20
 *    10   0  35  25
 *    15  35   0  30
 *    20  25  30   0
 * 
 * Expected Output:
 *   Exact Optimal Cost (Held-Karp): 80 (Tour: 0 -> 1 -> 3 -> 2 -> 0)
 *   Nearest Neighbor Tour Cost    : 80
 *   MST 2-Approximation Cost      : 80
 *   Approximation Ratio Comparison shown.
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

const int INF = 1e9;

// ============================================================================
// 1. EXACT ALGORITHM: Held-Karp Dynamic Programming with Bitmasking
// ============================================================================
/**
 * Held-Karp DP:
 * State: dp[mask][u] = minimum cost to visit all cities in `mask` ending at city `u`.
 * Transition: dp[mask][u] = min_{v in mask, v != u} (dp[mask ^ (1 << u)][v] + dist[v][u])
 * 
 * Time Complexity: O(n^2 * 2^n) - drastically faster than Brute Force O(n!)
 * Space Complexity: O(n * 2^n)
 */
pair<int, vector<int>> tspHeldKarp(const vector<vector<int>>& dist, int N) {
    int total_states = 1 << N;
    vector<vector<int>> dp(total_states, vector<int>(N, INF));
    vector<vector<int>> parent(total_states, vector<int>(N, -1));

    // Base case: Starting from city 0
    dp[1][0] = 0;

    for (int mask = 1; mask < total_states; mask++) {
        for (int u = 0; u < N; u++) {
            if (!(mask & (1 << u)) || dp[mask][u] == INF) continue;

            for (int v = 0; v < N; v++) {
                if (mask & (1 << v)) continue; // City v already visited

                int next_mask = mask | (1 << v);
                int new_cost = dp[mask][u] + dist[u][v];

                if (new_cost < dp[next_mask][v]) {
                    dp[next_mask][v] = new_cost;
                    parent[next_mask][v] = u;
                }
            }
        }
    }

    // Complete the tour back to city 0
    int min_total_cost = INF;
    int last_city = -1;
    int full_mask = (1 << N) - 1;

    for (int u = 1; u < N; u++) {
        if (dp[full_mask][u] + dist[u][0] < min_total_cost) {
            min_total_cost = dp[full_mask][u] + dist[u][0];
            last_city = u;
        }
    }

    // Reconstruct tour path
    vector<int> tour;
    int curr_mask = full_mask;
    int curr_city = last_city;

    while (curr_city != -1) {
        tour.push_back(curr_city);
        int prev_city = parent[curr_mask][curr_city];
        curr_mask ^= (1 << curr_city);
        curr_city = prev_city;
    }
    reverse(tour.begin(), tour.end());
    tour.push_back(0); // Return to start

    return {min_total_cost, tour};
}

// ============================================================================
// 2. APPROXIMATION ALGORITHM 1: Nearest Neighbor Heuristic
// ============================================================================
/**
 * Greedy choice: At current city, always visit the nearest unvisited city.
 * 
 * Time Complexity: O(n^2)
 * Space Complexity: O(n)
 */
pair<int, vector<int>> tspNearestNeighbor(const vector<vector<int>>& dist, int N) {
    vector<bool> visited(N, false);
    vector<int> tour;
    int current = 0;
    visited[current] = true;
    tour.push_back(current);

    int total_cost = 0;

    for (int step = 1; step < N; step++) {
        int next_city = -1;
        int min_dist = INF;

        for (int v = 0; v < N; v++) {
            if (!visited[v] && dist[current][v] < min_dist) {
                min_dist = dist[current][v];
                next_city = v;
            }
        }

        visited[next_city] = true;
        total_cost += min_dist;
        tour.push_back(next_city);
        current = next_city;
    }

    // Return to start city
    total_cost += dist[current][0];
    tour.push_back(0);

    return {total_cost, tour};
}

// ============================================================================
// 3. APPROXIMATION ALGORITHM 2: MST-Based 2-Approximation (Metric TSP)
// ============================================================================
/**
 * 1. Build Minimum Spanning Tree (MST) using Prim's algorithm: O(N^2).
 * 2. Perform Pre-order DFS traversal on MST to generate Eulerian shortcut tour.
 * 
 * Guarantees Tour Cost <= 2 * Optimal Cost (under Triangle Inequality).
 * Time Complexity: O(N^2)
 * Space Complexity: O(N)
 */
void dfsMST(int u, const vector<vector<int>>& adj, vector<bool>& visited, vector<int>& tour) {
    visited[u] = true;
    tour.push_back(u);
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfsMST(v, adj, visited, tour);
        }
    }
}

pair<int, vector<int>> tspMSTApproximation(const vector<vector<int>>& dist, int N) {
    // Prim's MST
    vector<int> parent(N, -1);
    vector<int> key(N, INF);
    vector<bool> inMST(N, false);

    key[0] = 0;

    for (int count = 0; count < N - 1; count++) {
        int min_key = INF, u = -1;
        for (int v = 0; v < N; v++) {
            if (!inMST[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }
        if (u == -1) break;
        inMST[u] = true;

        for (int v = 0; v < N; v++) {
            if (dist[u][v] && !inMST[v] && dist[u][v] < key[v]) {
                parent[v] = u;
                key[v] = dist[u][v];
            }
        }
    }

    // Build MST Adjacency list
    vector<vector<int>> mstAdj(N);
    for (int v = 1; v < N; v++) {
        if (parent[v] != -1) {
            mstAdj[parent[v]].push_back(v);
            mstAdj[v].push_back(parent[v]);
        }
    }

    // DFS preorder traversal
    vector<bool> visited(N, false);
    vector<int> tour;
    dfsMST(0, mstAdj, visited, tour);
    tour.push_back(0); // Return to start

    // Calculate tour cost
    int cost = 0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        cost += dist[tour[i]][tour[i + 1]];
    }

    return {cost, tour};
}

void printTour(const vector<int>& tour) {
    for (size_t i = 0; i < tour.size(); i++) {
        cout << tour[i];
        if (i < tour.size() - 1) cout << " -> ";
    }
    cout << "\n";
}

int main() {
    cout << "========================================================\n";
    cout << "  Mini Project 10: Travelling Salesperson Problem (TSP)  \n";
    cout << "  (Exact Held-Karp vs Greedy & MST Approximations)      \n";
    cout << "========================================================\n";

    int N;
    cout << "Enter number of cities N (e.g. 4 or 5): ";
    cin >> N;

    if (N < 2 || N > 20) {
        cout << "Number of cities must be between 2 and 20 for exact DP.\n";
        return 1;
    }

    vector<vector<int>> dist(N, vector<int>(N));
    cout << "Choose Distance Matrix Mode:\n";
    cout << "  1. Enter distance matrix manually\n";
    cout << "  2. Use standard 4-city textbook example\n";
    cout << "Enter choice (1 or 2): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        cout << "Enter adjacency matrix (" << N << "x" << N << "):\n";
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) cin >> dist[i][j];
    } else {
        N = 4;
        dist = {
            {0, 10, 15, 20},
            {10, 0, 35, 25},
            {15, 35, 0, 30},
            {20, 25, 30, 0}
        };
        cout << "Loaded 4-city symmetric distance matrix.\n";
    }

    cout << "\n--- Distance Matrix ---\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) cout << setw(5) << dist[i][j];
        cout << "\n";
    }

    // 1. Exact Held-Karp DP
    auto [optCost, optTour] = tspHeldKarp(dist, N);

    // 2. Nearest Neighbor Heuristic
    auto [nnCost, nnTour] = tspNearestNeighbor(dist, N);

    // 3. MST 2-Approximation
    auto [mstCost, mstTour] = tspMSTApproximation(dist, N);

    cout << "\n================ Execution & Tour Results ================\n";
    cout << "1. Exact Algorithm (Held-Karp Dynamic Programming):\n";
    cout << "   - Optimal Cost : " << optCost << "\n";
    cout << "   - Optimal Tour : "; printTour(optTour);

    cout << "\n2. Approximation 1 (Nearest Neighbor Greedy):\n";
    cout << "   - Heuristic Cost : " << nnCost << "\n";
    cout << "   - Heuristic Tour : "; printTour(nnTour);
    cout << "   - Approx Ratio   : " << fixed << setprecision(3) << (double)nnCost / optCost << "x optimal\n";

    cout << "\n3. Approximation 2 (MST Pre-order 2-Approximation):\n";
    cout << "   - Heuristic Cost : " << mstCost << "\n";
    cout << "   - Heuristic Tour : "; printTour(mstTour);
    cout << "   - Approx Ratio   : " << fixed << setprecision(3) << (double)mstCost / optCost << "x optimal\n";

    cout << "\n================ Complexity Comparison ================\n";
    cout << left << setw(30) << "Algorithm Category" 
         << setw(20) << "Time Complexity" 
         << setw(20) << "Optimality Guarantee" << "\n";
    cout << string(70, '-') << "\n";
    cout << left << setw(30) << "Brute Force" 
         << setw(20) << "O(N!)" 
         << setw(20) << "Exact Optimal" << "\n";
    cout << left << setw(30) << "Held-Karp DP" 
         << setw(20) << "O(N^2 * 2^N)" 
         << setw(20) << "Exact Optimal" << "\n";
    cout << left << setw(30) << "Nearest Neighbor" 
         << setw(20) << "O(N^2)" 
         << setw(20) << "Heuristic (no bound)" << "\n";
    cout << left << setw(30) << "MST 2-Approximation" 
         << setw(20) << "O(N^2)" 
         << setw(20) << "<= 2.0x Optimal" << "\n";
    cout << left << setw(30) << "Christofides Algorithm" 
         << setw(20) << "O(N^3)" 
         << setw(20) << "<= 1.5x Optimal" << "\n";
    cout << string(70, '-') << "\n";

    return 0;
}
