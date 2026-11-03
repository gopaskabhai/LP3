
/*
    BINOMIAL COEFFICIENTS USING PASCAL'S TRIANGLE

    Theory:
    The binomial coefficient C(n, k) represents the number of ways to choose
    k items from n items. Pascal's Triangle stores these coefficients:

        C(n, k) = C(n - 1, k - 1) + C(n - 1, k)

    The first and last values in every row are 1 because:

        C(n, 0) = C(n, n) = 1

    This program uses dynamic programming. It stores previously calculated
    values in dp so that every coefficient is calculated only once. Row i
    contains the coefficients C(i, 0) through C(i, i).

    Algorithm:
    1. Read the number of rows n.
    2. Set the first and last value of every row to 1.
    3. Calculate each interior value by adding the two values from the
       previous row.
    4. Display all generated rows.

    Complexity:
    - Time: O(n^2), because the program visits each position in the triangle.
    - Space: O(n^2), because all coefficients are stored in dp.

    Input:
    Enter one integer n, where 1 <= n <= 100.
    The array has space for at most 100 rows.

    Example input:
        5

    Example output:
        Binomial Coefficients:
        1
        1 1
        1 2 1
        1 3 3 1
        1 4 6 4 1

    How to compile and run on Windows using g++:
        g++ Binomial.cpp -o Binomial.exe
        .\Binomial.exe

    To provide input from a file, create input.txt containing, for example:
        5

    Then run:
        .\Binomial.exe < input.txt
*/

#include <iostream>
using namespace std;

int main() {
    int n;

    cout << "Enter number of rows: ";
    cin >> n;

    // dp[i][j] stores the binomial coefficient C(i, j).
    int dp[100][100] = {0};

    // Generate Pascal's Triangle row by row.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {

            // The boundary coefficients of every row are always 1.
            if (j == 0 || j == i) {
                dp[i][j] = 1;
            }
            else {
                // Pascal's identity: C(i,j) = C(i-1,j-1) + C(i-1,j).
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
            }
        }
    }

    // Display the generated binomial coefficients.
    cout << "\nBinomial Coefficients:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            cout << dp[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
