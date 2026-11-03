/**
 * Assignment 5: N-Queens Problem with First Queen Placed (Backtracking)
 * 
 * Problem Statement:
 * Design n-Queens matrix having first Queen placed. 
 * Use backtracking to place remaining Queens to generate the final n-queen's matrix.
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter board size N: 4
 *   Enter column (0 to 3) for the 1st Queen in Row 0: 1
 * 
 * Expected Output:
 *   Initial Board with First Queen Placed:
 *   . Q . . 
 *   . . . . 
 *   . . . . 
 *   . . . . 
 * 
 *   Solution Found:
 *   . Q . . 
 *   . . . Q 
 *   Q . . . 
 *   . . Q . 
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/**
 * Checks if placing a Queen at (row, col) is safe.
 * A position is safe if no other queen shares:
 * 1. The same column: board[i][col] == 1
 * 2. The upper-left diagonal: board[i][j] == 1
 * 3. The upper-right diagonal: board[i][j] == 1
 */
bool isSafe(const vector<vector<int>>& board, int row, int col, int N) {
    // Check column above
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1) return false;
    }

    // Check upper-left diagonal
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) return false;
    }

    // Check upper-right diagonal
    for (int i = row - 1, j = col + 1; i >= 0 && j < N; i--, j++) {
        if (board[i][j] == 1) return false;
    }

    return true;
}

/**
 * Prints the chessboard matrix
 * 'Q' represents Queen, '.' represents empty square
 */
void printBoard(const vector<vector<int>>& board, int N) {
    for (int i = 0; i < N; i++) {
        cout << "  ";
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 1) cout << "Q ";
            else cout << ". ";
        }
        cout << "\n";
    }
}

/**
 * Recursive backtracking function to place remaining queens row by row.
 * Starts from row = 1 since row = 0 already contains the user-placed queen.
 * 
 * Time Complexity: O(N!) in worst case
 * Space Complexity: O(N^2) for the board + O(N) call stack
 */
bool solveNQueens(vector<vector<int>>& board, int row, int N, int &solutionCount) {
    // Base Case: All N queens are successfully placed
    if (row == N) {
        solutionCount++;
        cout << "\nSolution #" << solutionCount << ":\n";
        printBoard(board, N);
        return true; // Return true to find at least one solution
    }

    bool hasSolution = false;

    // Try placing a queen in all columns of the current row
    for (int col = 0; col < N; col++) {
        if (isSafe(board, row, col, N)) {
            // Place Queen
            board[row][col] = 1;

            // Recur to place queen in next row
            if (solveNQueens(board, row + 1, N, solutionCount)) {
                hasSolution = true;
                return true; // Stop after finding the first valid solution
            }

            // Backtrack: remove Queen and explore next column
            board[row][col] = 0;
        }
    }

    return hasSolution;
}

int main() {
    cout << "========================================================\n";
    cout << "   DAA Lab Assignment 5: N-Queens with 1st Queen Placed  \n";
    cout << "========================================================\n";

    int N;
    cout << "Enter board size N (e.g. 4 or 8): ";
    cin >> N;

    if (N <= 3 && N != 1) {
        cout << "Note: No solution exists for N = " << N << " (N must be 1 or >= 4).\n";
        return 0;
    }

    // Initialize N x N chessboard with 0 (empty)
    vector<vector<int>> board(N, vector<int>(N, 0));

    int firstCol;
    cout << "Enter column index (0 to " << N - 1 << ") for Queen 1 in Row 0: ";
    cin >> firstCol;

    if (firstCol < 0 || firstCol >= N) {
        cout << "Invalid column! Must be between 0 and " << N - 1 << ".\n";
        return 1;
    }

    // Place the first Queen in Row 0
    board[0][firstCol] = 1;

    cout << "\n--- Initial Board Configuration (First Queen in Row 0, Col " << firstCol << ") ---\n";
    printBoard(board, N);

    cout << "\nPlacing remaining " << N - 1 << " Queens using Backtracking...\n";

    int solutionCount = 0;
    bool found = solveNQueens(board, 1, N, solutionCount);

    if (!found) {
        cout << "\nNo valid N-Queens solution exists with the first Queen at (0, " 
             << firstCol << ").\n";
    } else {
        cout << "\nSuccessfully placed all " << N << " Queens!\n";
    }

    cout << "========================================================\n";
    return 0;
}
