#include <iostream>
using namespace std;

#define N 8

// Function to print the chessboard
void printSolution(int board[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << (board[i][j] ? " Q " : " . ");
        cout << endl;
    }
}

// Function to check if placing a queen is safe
bool isSafe(int board[N][N], int row, int col)
{
    int i, j;

    // Check this column on upper side
    for (i = 0; i < row; i++)
        if (board[i][col])
            return false;

    // Check upper left diagonal
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j])
            return false;

    // Check upper right diagonal
    for (i = row, j = col; i >= 0 && j < N; i--, j++)
        if (board[i][j])
            return false;

    return true;
}

// Recursive function to solve N-Queens
bool solveNQ(int board[N][N], int row)
{
    // Base case: All queens placed
    if (row >= N)
        return true;

    // Try placing queen in all columns of current row
    for (int col = 0; col < N; col++)
    {
        if (isSafe(board, row, col))
        {
            board[row][col] = 1; // Place queen

            // Recurse for next row
            if (solveNQ(board, row + 1))
                return true;

            // If placing queen in (row,col) doesn't lead to solution
            board[row][col] = 0; // Backtrack
        }
    }
    return false;
}

int main()
{
    int board[N][N] = {0};

    // Place the first queen manually (as per question)
    board[0][0] = 1;

    // Start from second row (since first queen is already placed)
    if (solveNQ(board, 1) == false)
    {
        cout << "Solution does not exist";
        return 0;
    }

    printSolution(board);
    return 0;
}
