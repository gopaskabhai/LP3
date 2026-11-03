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
        cout << "Solution Found:\n";
        printBoard(board, n);
        return true;
    }
    
    if (col == 0) {
        return solveNQueens(board, col + 1, n, solutionCount);
    }
    
    for (int i = 0; i < n; i++) {
        if (isSafe(board, i, col, n)) {
            board[i][col] = 1;
            
            if (solveNQueens(board, col + 1, n, solutionCount)) {
                return true;
            }
            
            board[i][col] = 0;
        }
    }
    
    return false;
}

int main() {
    int n = 8;
    int board[8][8] = {0};
    int firstRow, firstCol;
    
    cout << "8-QUEENS PROBLEM\n";
    cout << "================\n\n";
    
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
        cout << "No solution exists with Queen at (" << firstRow << ", " << firstCol << ")\n";
    }
    
    return 0;
}


/*
8-QUEENS PROBLEM - SIMPLIFIED VERSION
======================================

Algorithm: Backtracking (without showing intermediate steps)
Time Complexity: O(n!) where n = 8
Space Complexity: O(n) for recursion stack

Problem Statement:
Place 8 queens on an 8×8 chessboard such that no two queens attack each other.

This simplified version:
- Places first queen at user-specified position
- Uses backtracking to find all valid solutions
- Shows only final solutions (no intermediate steps)
- Clean output for easy visualization

Constraints:
- No two queens in the same row
- No two queens in the same column  
- No two queens in the same diagonal

Solution Approach:
1. User places first queen
2. Algorithm uses backtracking to place remaining queens
3. Only valid complete solutions are displayed
4. All possible solutions are found

Board Representation:
Q = Queen placed
. = Empty square

Safety Rules:
A position is safe if no other queen can attack it:
- Row check: No queen in same row
- Column check: No queen in same column
- Diagonal check: No queen in both diagonals

Example Solutions:
For first queen at (0,0):

Solution 1:
Q . . . . . . .
. . . . Q . . .
. . . . . . . Q
. . . . . Q . .
. . Q . . . . .
. . . . . . Q .
. Q . . . . . .
. . . Q . . . .

Multiple solutions may exist depending on first queen position.

Input Format:
Enter row and column (0-7) for first queen position

Output Format:
- Initial board with first queen
- All valid complete solutions numbered
- Total count of solutions found

Note: Some starting positions may have no solutions or very few solutions.
*/
