/**
 * Mini Project 7: Sequential vs Multithreaded Matrix Multiplication
 * 
 * Problem Statement:
 * Write a program to implement matrix multiplication. Also implement
 * multithreaded matrix multiplication with either one thread per row 
 * or one thread per cell. Analyze and compare their performance.
 * 
 * Implemented Variants:
 * 1. Sequential Matrix Multiplication
 * 2. Multithreaded Matrix Multiplication (One thread per row)
 * 3. Multithreaded Matrix Multiplication (One thread per cell / thread pool block)
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter matrix dimension N (e.g., 200 or 500): 4
 *   Choose mode: 1 (Manual input) or 2 (Random matrix generation)
 *   If manual:
 *     Matrix A (4x4):
 *       1 2 3 4
 *       5 6 7 8
 *       9 1 2 3
 *       4 5 6 7
 *     Matrix B (4x4):
 *       1 0 0 0
 *       0 1 0 0
 *       0 0 1 0
 *       0 0 0 1
 * 
 * Expected Output:
 *   Result matrix shown (Identity multiplication gives Matrix A).
 *   Performance table comparing Sequential vs Row-threaded vs Cell-threaded.
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

// Type alias for 2D Matrix
using Matrix = vector<vector<int>>;

/**
 * Sequential Matrix Multiplication:
 * C[i][j] = sum(A[i][k] * B[k][j]) for k from 0 to N-1
 * 
 * Time Complexity: O(N^3)
 * Space Complexity: O(N^2)
 */
Matrix multiplySequential(const Matrix& A, const Matrix& B, int N) {
    Matrix C(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

/**
 * Helper: Computes a specific row `i` of the product matrix C.
 */
void multiplyRowWorker(const Matrix& A, const Matrix& B, Matrix& C, int row, int N) {
    for (int j = 0; j < N; j++) {
        int sum = 0;
        for (int k = 0; k < N; k++) {
            sum += A[row][k] * B[k][j];
        }
        C[row][j] = sum;
    }
}

/**
 * Multithreaded Matrix Multiplication: One thread per row.
 * Spawns N threads, where thread `i` computes row `i` independently.
 * 
 * Time Complexity: O(N^3 / P) where P is number of CPU cores.
 * Thread Creation Overhead: O(N) threads created.
 */
Matrix multiplyThreadPerRow(const Matrix& A, const Matrix& B, int N) {
    Matrix C(N, vector<int>(N, 0));
    vector<thread> threads;
    threads.reserve(N);

    for (int i = 0; i < N; i++) {
        threads.emplace_back(multiplyRowWorker, cref(A), cref(B), ref(C), i, N);
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
    return C;
}

/**
 * Helper: Computes a single cell C[row][col].
 */
void multiplyCellWorker(const Matrix& A, const Matrix& B, Matrix& C, int row, int col, int N) {
    int sum = 0;
    for (int k = 0; k < N; k++) {
        sum += A[row][k] * B[k][col];
    }
    C[row][col] = sum;
}

/**
 * Helper: Computes a batch of cells to prevent OS thread starvation for large N.
 */
void multiplyCellBatchWorker(const Matrix& A, const Matrix& B, Matrix& C, int startCell, int endCell, int N) {
    for (int idx = startCell; idx < endCell; idx++) {
        int r = idx / N;
        int c = idx % N;
        int sum = 0;
        for (int k = 0; k < N; k++) {
            sum += A[r][k] * B[k][c];
        }
        C[r][c] = sum;
    }
}

/**
 * Multithreaded Matrix Multiplication: One thread per cell (or cell-batching for large N).
 * For small N (N <= 20), spawns N*N distinct threads.
 * For large N, spawns hardware_concurrency threads with balanced cell partitioning.
 */
Matrix multiplyThreadPerCell(const Matrix& A, const Matrix& B, int N) {
    Matrix C(N, vector<int>(N, 0));
    vector<thread> threads;

    if (N <= 20) {
        // Direct one thread per cell
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                threads.emplace_back([&A, &B, &C, i, j, N]() {
                    int sum = 0;
                    for (int k = 0; k < N; k++) {
                        sum += A[i][k] * B[k][j];
                    }
                    C[i][j] = sum;
                });
            }
        }
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    } else {
        // Balanced chunked threads matching CPU hardware threads
        unsigned int numThreads = thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 4;
        int totalCells = N * N;
        int chunkSize = (totalCells + numThreads - 1) / numThreads;

        for (unsigned int t = 0; t < numThreads; t++) {
            int startCell = t * chunkSize;
            int endCell = min(startCell + chunkSize, totalCells);
            if (startCell < endCell) {
                threads.emplace_back(multiplyCellBatchWorker, cref(A), cref(B), ref(C), startCell, endCell, N);
            }
        }
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    }
    return C;
}

void printMatrix(const Matrix& M, int N) {
    for (int i = 0; i < N; i++) {
        cout << "  ";
        for (int j = 0; j < N; j++) {
            cout << setw(6) << M[i][j] << " ";
        }
        cout << "\n";
    }
}

int main() {
    cout << "========================================================\n";
    cout << "  Mini Project 7: Sequential & Multithreaded Matrix Mult\n";
    cout << "========================================================\n";

    int N;
    cout << "Enter matrix dimension N (e.g. 4 for demo or 200 for benchmark): ";
    cin >> N;

    if (N <= 0) {
        cout << "Invalid dimension.\n";
        return 1;
    }

    Matrix A(N, vector<int>(N));
    Matrix B(N, vector<int>(N));

    cout << "Choose Matrix Generation Mode:\n";
    cout << "  1. Enter elements manually\n";
    cout << "  2. Generate random elements automatically (Recommended for N >= 5)\n";
    cout << "Enter choice (1 or 2): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        cout << "Enter elements of Matrix A (" << N << "x" << N << "):\n";
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) cin >> A[i][j];

        cout << "Enter elements of Matrix B (" << N << "x" << N << "):\n";
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) cin >> B[i][j];
    } else {
        srand(time(nullptr));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() % 10;
                B[i][j] = rand() % 10;
            }
        }
        cout << "Random matrices generated successfully.\n";
    }

    if (N <= 6) {
        cout << "\nMatrix A:\n"; printMatrix(A, N);
        cout << "\nMatrix B:\n"; printMatrix(B, N);
    }

    // 1. Sequential
    auto s1 = high_resolution_clock::now();
    Matrix C_seq = multiplySequential(A, B, N);
    auto e1 = high_resolution_clock::now();
    auto time_seq = duration_cast<microseconds>(e1 - s1).count();

    // 2. Thread per Row
    auto s2 = high_resolution_clock::now();
    Matrix C_row = multiplyThreadPerRow(A, B, N);
    auto e2 = high_resolution_clock::now();
    auto time_row = duration_cast<microseconds>(e2 - s2).count();

    // 3. Thread per Cell / Partitioned Cell
    auto s3 = high_resolution_clock::now();
    Matrix C_cell = multiplyThreadPerCell(A, B, N);
    auto e3 = high_resolution_clock::now();
    auto time_cell = duration_cast<microseconds>(e3 - s3).count();

    if (N <= 6) {
        cout << "\nResult Matrix C (A x B):\n";
        printMatrix(C_seq, N);
    }

    // Verify correctness
    bool match = true;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (C_seq[i][j] != C_row[i][j] || C_seq[i][j] != C_cell[i][j]) {
                match = false;
                break;
            }
        }
    }

    cout << "\n================ Performance Comparison ================\n";
    cout << left << setw(30) << "Approach" 
         << setw(20) << "Execution Time" 
         << setw(20) << "Speedup vs Seq" << "\n";
    cout << string(68, '-') << "\n";

    cout << left << setw(30) << "1. Sequential" 
         << setw(16) << time_seq << " us" 
         << setw(20) << "1.00x (Baseline)" << "\n";

    double speedup_row = (time_row > 0) ? (double)time_seq / time_row : 1.0;
    cout << left << setw(30) << "2. Thread-Per-Row" 
         << setw(16) << time_row << " us" 
         << fixed << setprecision(2) << setw(18) << speedup_row << "x\n";

    double speedup_cell = (time_cell > 0) ? (double)time_seq / time_cell : 1.0;
    cout << left << setw(30) << "3. Thread-Per-Cell / Block" 
         << setw(16) << time_cell << " us" 
         << fixed << setprecision(2) << setw(18) << speedup_cell << "x\n";

    cout << string(68, '-') << "\n";
    cout << "Correctness Verified Across All Methods: " << (match ? "PASS (Results Match)" : "FAIL") << "\n";
    cout << "========================================================\n";

    return 0;
}
