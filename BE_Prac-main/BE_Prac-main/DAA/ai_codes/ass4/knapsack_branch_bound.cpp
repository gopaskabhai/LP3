#include <bits/stdc++.h>
using namespace std;

struct Item {
    int weight;
    int value;
    int index;
};

struct Node {
    int level;
    int profit;
    int weight;
    double bound;
};

bool comparison(Item a, Item b) {
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}

double calculateBound(Node u, int n, int capacity, Item items[]) {
    if (u.weight >= capacity) {
        return 0;
    }
    
    double bound = u.profit;
    int j = u.level + 1;
    int totalWeight = u.weight;
    
    while (j < n && totalWeight + items[j].weight <= capacity) {
        totalWeight += items[j].weight;
        bound += items[j].value;
        j++;
    }
    
    if (j < n) {
        bound += (capacity - totalWeight) * ((double)items[j].value / items[j].weight);
    }
    
    return bound;
}

int knapsackBranchBound(Item items[], int n, int capacity) {
    sort(items, items + n, comparison);
    
    queue<Node> Q;
    Node u, v;
    
    u.level = -1;
    u.profit = 0;
    u.weight = 0;
    u.bound = calculateBound(u, n, capacity, items);
    
    Q.push(u);
    
    int maxProfit = 0;
    int nodesExplored = 0;
    
    cout << "\nBranch and Bound Tree Exploration:\n";
    cout << "-----------------------------------\n";
    
    while (!Q.empty()) {
        u = Q.front();
        Q.pop();
        nodesExplored++;
        
        if (u.level == n - 1) {
            continue;
        }
        
        v.level = u.level + 1;
        
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].value;
        
        if (v.weight <= capacity && v.profit > maxProfit) {
            maxProfit = v.profit;
            cout << "Node " << nodesExplored << ": Include item " << (v.level + 1) 
                 << " → Profit: " << v.profit << ", Weight: " << v.weight << " [NEW MAX]\n";
        } else if (v.weight <= capacity) {
            cout << "Node " << nodesExplored << ": Include item " << (v.level + 1) 
                 << " → Profit: " << v.profit << ", Weight: " << v.weight << "\n";
        }
        
        v.bound = calculateBound(v, n, capacity, items);
        
        if (v.bound > maxProfit) {
            Q.push(v);
        } else {
            cout << "  → Pruned (bound " << v.bound << " ≤ maxProfit " << maxProfit << ")\n";
        }
        
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = calculateBound(v, n, capacity, items);
        
        if (v.bound > maxProfit) {
            Q.push(v);
            cout << "Node " << nodesExplored << ": Exclude item " << (v.level + 1) 
                 << " → Profit: " << v.profit << ", Weight: " << v.weight << "\n";
        } else {
            cout << "Node " << nodesExplored << ": Exclude item " << (v.level + 1) 
                 << " → Pruned (bound " << v.bound << " ≤ maxProfit " << maxProfit << ")\n";
        }
    }
    
    cout << "\nTotal nodes explored: " << nodesExplored << "\n";
    
    return maxProfit;
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
        items[i].index = i + 1;
    }
    
    cout << "\nInput Items:\n";
    cout << "Item\tWeight\tValue\tValue/Weight\n";
    for (int i = 0; i < n; i++) {
        double ratio = (double)items[i].value / items[i].weight;
        cout << items[i].index << "\t" << items[i].weight << "\t" << items[i].value 
             << "\t" << fixed << setprecision(2) << ratio << "\n";
    }
    
    int maxProfit = knapsackBranchBound(items, n, capacity);
    
    cout << "\n--- RESULTS ---\n";
    cout << "Maximum value: " << maxProfit << "\n";
    
    delete[] items;
    return 0;
}


/*
0-1 KNAPSACK PROBLEM - BRANCH AND BOUND APPROACH
=================================================

Algorithm: Branch and Bound with Best-First Search
Time Complexity: O(2^n) worst case, often much better with pruning
Space Complexity: O(n) for queue

Branch and Bound Strategy:
Branch and Bound is an algorithmic technique that:
1. Systematically explores solution space (branching)
2. Eliminates suboptimal branches early (bounding)
3. Uses a bound function to estimate best possible solution in a branch
4. Prunes branches that cannot lead to better solution than current best

Core Components:

1. State Space Tree:
   - Each node represents a decision: include or exclude an item
   - Root = no items considered
   - Left child = include current item
   - Right child = exclude current item
   - Leaf nodes = complete solutions

2. Bound Function:
   - Calculates upper bound on maximum profit achievable from current node
   - Uses fractional knapsack for remaining items (optimistic estimate)
   - If bound ≤ current best, prune this branch

3. Search Strategy:
   - BFS: Uses queue (FIFO)
   - DFS: Uses stack (LIFO)
   - Best-First: Uses priority queue (by bound)
   This implementation uses BFS with pruning

Algorithm Steps:
1. Sort items by value/weight ratio (descending)
2. Create root node with profit=0, weight=0
3. Add root to queue
4. While queue not empty:
   a. Remove node from queue
   b. If leaf node, update maxProfit if better
   c. Create child for including next item:
      - If feasible and bound > maxProfit, add to queue
   d. Create child for excluding next item:
      - If bound > maxProfit, add to queue
5. Return maxProfit

Bound Calculation:
For a node at level i with current weight w and profit p:
1. Add profits of items that fit completely
2. Add fractional profit of first item that doesn't fit
3. This gives optimistic upper bound

Why It Works:
- Explores only promising branches
- Prunes branches that cannot improve current best
- Sorting by ratio improves bound estimates
- Often explores far fewer than 2^n nodes

Example:
Items sorted by ratio: [(2,40), (5,30), (10,50), (5,10)]
Capacity: 16

State Space Tree (pruning shown):
                    Root(0,0)
                   /         \
          Include1(2,40)    Exclude1(0,0)
             /      \           /      \
      Inc2(7,70) Exc2(2,40) Inc2(5,30) Exc2(0,0)
         [Pruned]  [Cont]    [Cont]    [Pruned]

Nodes explored << 2^4 = 16 due to pruning

Time Complexity Analysis:
Worst case: O(2^n) - explores all nodes
Average case: Much better due to pruning
- Depends on bound function quality
- Good bounds → more pruning → fewer nodes
Best case: O(n log n) - only sorting needed

Space Complexity Analysis:
- Queue size: O(2^n) worst case
- In practice: O(n) due to pruning
- Much better than DP's O(n*W) when W is large

Comparison with Dynamic Programming:

Aspect               | DP                    | Branch & Bound
---------------------|----------------------|------------------------
Time complexity      | O(n*W) guaranteed    | O(2^n) worst, often better
Space complexity     | O(n*W)               | O(n) typical
Best when            | W is small           | n is small or good bounds
Guarantees           | Polynomial time      | Exponential worst case
Works for large W    | No                   | Yes
Implementation       | Simpler              | More complex

When to Use Branch and Bound:
- When W is very large (DP infeasible)
- When n is small (< 20-25)
- When tight bounds can be calculated
- When average case performance is good
- When memory is limited

Advantages:
- Works with any capacity size
- Low space complexity
- Can find optimal quickly with good pruning
- Flexible (can use different search strategies)
- Can be stopped early for approximate solution

Disadvantages:
- Worst case exponential time
- No guarantee of polynomial time
- Performance depends on input
- More complex to implement
- Hard to predict runtime

Optimization Techniques:
1. Sort by value/weight ratio (done here)
2. Use best-first search instead of BFS
3. Calculate tighter bounds
4. Use dynamic programming for bound calculation
5. Implement both left and right child pruning

Real-world Applications:
- Cryptography (when W is very large)
- Resource allocation with large budgets
- Scheduling with many time slots
- Network optimization problems
- Combinatorial optimization

Pruning Example:
Current maxProfit = 50
Node with bound = 45
→ Prune! This branch cannot improve solution

Node with bound = 60
→ Explore! Might lead to better solution

Input Format:
First line: Number of items (n)
Second line: Knapsack capacity (W)
Next n lines: Weight Value for each item

Example Input:
4
16
2 40
5 30
10 50
5 10

Expected Output:
Shows tree exploration with pruning decisions
Nodes explored count
Maximum value achieved
*/
