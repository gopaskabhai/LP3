#include <bits/stdc++.h>
using namespace std;

void printBoard(int board[][8], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                cout << "Q ";
            } else {
                cout << ". ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

bool isSafe(int board[][8], int row, int col, int n) {
    for (int i = 0; i < col; i++) {
        if (board[row][i] == 1) {
            return false;
        }
    }
    
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) {
            return false;
        }
    }
    
    for (int i = row, j = col; i < n && j >= 0; i++, j--) {
        if (board[i][j] == 1) {
            return false;
        }
    }
    
    return true;
}

bool solveNQueens(int board[][8], int col, int n, int &solutionCount) {
    if (col >= n) {
        solutionCount++;
        cout << "Solution " << solutionCount << ":\n";
        printBoard(board, n);
        return true;
    }
    
    if (col == 0) {
        return solveNQueens(board, col + 1, n, solutionCount);
    }
    
    bool solutionFound = false;
    
    for (int i = 0; i < n; i++) {
        if (isSafe(board, i, col, n)) {
            board[i][col] = 1;
            
            cout << "Trying: Place Queen at (" << i << ", " << col << ")\n";
            
            if (solveNQueens(board, col + 1, n, solutionCount)) {
                solutionFound = true;
            }
            
            board[i][col] = 0;
            cout << "Backtracking: Remove Queen from (" << i << ", " << col << ")\n";
        }
    }
    
    return solutionFound;
}

int main() {
    int n = 8;
    int board[8][8] = {0};
    int firstRow, firstCol;
    
    cout << "8-QUEENS PROBLEM USING BACKTRACKING\n";
    cout << "====================================\n\n";
    
    cout << "Enter position for first Queen (row col) [0-7]: ";
    cin >> firstRow >> firstCol;
    
    if (firstRow < 0 || firstRow >= n || firstCol < 0 || firstCol >= n) {
        cout << "Invalid position! Using default (0, 0)\n";
        firstRow = 0;
        firstCol = 0;
    }
    
    board[firstRow][firstCol] = 1;
    
    cout << "\nInitial Board with First Queen at (" << firstRow << ", " << firstCol << "):\n";
    printBoard(board, n);
    
    cout << "Starting Backtracking...\n";
    cout << "------------------------\n";
    
    int solutionCount = 0;
    
    if (firstCol == 0) {
        solveNQueens(board, 0, n, solutionCount);
    } else {
        bool found = solveNQueens(board, 0, n, solutionCount);
        if (!found) {
            board[firstRow][firstCol] = 0;
            for (int col = 0; col < n; col++) {
                if (col == firstCol) continue;
                for (int row = 0; row < n; row++) {
                    board[row][col] = 0;
                }
            }
            board[firstRow][firstCol] = 1;
            solveNQueens(board, 0, n, solutionCount);
        }
    }
    
    if (solutionCount == 0) {
        cout << "\nNo solution exists with Queen at (" << firstRow << ", " << firstCol << ")\n";
    } else {
        cout << "\nTotal solutions found: " << solutionCount << "\n";
    }
    
    return 0;
}


/*
8-QUEENS PROBLEM USING BACKTRACKING
====================================

Algorithm: Backtracking
Time Complexity: O(n!) where n = 8
Space Complexity: O(n) for recursion stack

Problem Statement:
Place 8 queens on an 8×8 chessboard such that no two queens attack each other.

Constraints:
- No two queens in the same row
- No two queens in the same column
- No two queens in the same diagonal

Additional Constraint:
- First queen position is pre-placed by user
- Find all valid configurations with this constraint

What is Backtracking?
Backtracking is an algorithmic technique that:
1. Tries to build solution incrementally
2. Abandons solution (backtracks) as soon as it determines current path cannot lead to valid solution
3. Recursively explores all possibilities
4. Uses pruning to avoid exploring invalid paths

Backtracking Template:
function solve(state):
    if state is goal:
        return solution
    
    for each possible choice:
        if choice is valid:
            make choice
            solve(new state)
            undo choice (backtrack)

N-Queens Strategy:
1. Place queens column by column (one queen per column)
2. For each column, try placing queen in each row
3. Check if placement is safe (no conflicts)
4. If safe, place queen and move to next column
5. If placement leads to dead end, backtrack and try next row
6. Repeat until all queens placed or all options exhausted

Safety Check:
A position (row, col) is safe if no queen exists in:
1. Same row (check all previous columns in that row)
2. Upper diagonal (check diagonal going up-left)
3. Lower diagonal (check diagonal going down-left)

Note: We only check left side because we place queens left to right

Algorithm Steps:
1. Get initial queen position from user
2. Place first queen on board
3. Start from column 0 (or next column after first queen)
4. For each column:
   a. Try each row in that column
   b. Check if position is safe
   c. If safe:
      - Place queen
      - Recursively solve for next column
      - If successful, record solution
      - Remove queen (backtrack)
   d. If not safe, try next row
5. Display all found solutions

Example Execution:
Initial: Q at (0,0)

Step 1: Try column 1
- Try (0,1): Unsafe (same row as Q at (0,0))
- Try (1,1): Unsafe (diagonal)
- Try (2,1): Safe! Place Q
...continue recursively

If dead end reached:
- Remove last placed queen
- Try next position
- Continue backtracking

Visualization:
Q . . . . . . .    Starting configuration
. . ? . . . . .    Trying different positions
. . . . ? . . .    Backtracking when stuck
...

Solution:
Q . . . . . . .
. . . . Q . . .
. . . . . . . Q
. . . . . Q . .
. . Q . . . . .
. . . . . . Q .
. Q . . . . . .
. . . Q . . . .

Time Complexity Analysis:
- Worst case: O(n!) 
- For 8-queens: ~8! = 40,320 combinations
- With pruning: Much fewer (typically 100-200 recursive calls)
- Each safety check: O(n)
- Overall with pruning: O(n! × n) but practical performance much better

Space Complexity Analysis:
- Board storage: O(n²)
- Recursion stack depth: O(n)
- Overall: O(n²)

Why Backtracking?
- Exhaustive search would check 2^64 configurations
- Backtracking prunes invalid paths early
- Reduces search space dramatically
- Still explores all valid solutions

Optimization Techniques:
1. Column-wise placement (done here)
   - Ensures one queen per column automatically
   - Reduces search space

2. Bit manipulation
   - Use bits to track attacked positions
   - Faster than array checking

3. Symmetry elimination
   - Eliminate symmetric solutions
   - Reduces unique solutions by ~8x

4. Constraint propagation
   - Mark all attacked cells immediately
   - Further reduces search space

Number of Solutions:
- 8-Queens has 92 total solutions
- 12 fundamental solutions (others are rotations/reflections)
- With first queen fixed, solutions reduce significantly

Real-world Applications:
- Resource allocation where resources conflict
- Scheduling with mutual exclusion constraints
- Graph coloring problems
- Sudoku solving
- Crossword puzzle generation
- Circuit board design
- Frequency assignment in wireless networks

Advantages:
- Finds all solutions
- Memory efficient (only O(n) recursion stack)
- Easy to understand and implement
- Can be stopped early if only one solution needed
- Natural pruning of invalid paths

Disadvantages:
- Exponential worst-case time
- Can be slow for large n
- Not suitable when n > 20-25
- Difficult to parallelize

Variations:
1. N-Queens: Generalize to n×n board
2. Find first solution only
3. Count solutions without storing
4. Find all unique solutions (no symmetry)

Input Format:
Enter row and column for first queen (0-indexed)
Example: 0 0 (places queen at top-left corner)

Expected Output:
Shows initial board
Displays backtracking process
Shows all valid solutions
Reports total solutions found

If no solution exists with given first position, reports failure

Example Run:
Input: First Queen at (0, 0)
Output: Multiple solutions with backtracking steps shown

Note: Some starting positions may have no valid solutions
Example: Queen at (0, 1) drastically reduces possible solutions
*/
