/**
 * Mini Project 8: Sequential vs Multithreaded Merge Sort Analysis
 * 
 * Problem Statement:
 * Implement merge sort and multithreaded merge sort. Compare time required
 * by both the algorithms. Also analyze the performance of each algorithm for 
 * the best case and the worst case.
 * 
 * Features:
 * - Sequential Merge Sort with exact step & comparison counters
 * - Multithreaded Merge Sort (recursive fork-join with depth cutoff to prevent thread explosion)
 * - Automated analysis across:
 *     1. Best Case: Already sorted array
 *     2. Worst Case: Alternating/Reverse adversarial array
 *     3. Average Case: Random unsorted array
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter array size N (e.g., 100000): 100000
 * 
 * Expected Output:
 *   Detailed comparison table showing:
 *     - Best-case timing & comparisons
 *     - Worst-case timing & comparisons
 *     - Random-case timing & comparisons
 *     - Sequential vs Multithreaded Speedup factors
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

// Threshold below which recursion does not spawn new threads
const int THREAD_CUTOFF_SIZE = 10000;
const int MAX_THREAD_DEPTH = 3; // 2^3 = 8 max concurrent leaf threads

// Counters for theoretical analysis
long long comparisons_seq = 0;

/**
 * Standard In-place Merge operation
 */
void merge(vector<int>& arr, int left, int mid, int right, bool countComparisons = false) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1);
    vector<int> R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (countComparisons) comparisons_seq++;
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

/**
 * Sequential Merge Sort: Divide and Conquer
 * Time Complexity: O(n log n) in all cases (Best, Average, Worst)
 * Space Complexity: O(n) auxiliary array
 */
void mergeSortSequential(vector<int>& arr, int left, int right, bool countComparisons = false) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSequential(arr, left, mid, countComparisons);
        mergeSortSequential(arr, mid + 1, right, countComparisons);
        merge(arr, left, mid, right, countComparisons);
    }
}

/**
 * Multithreaded Merge Sort:
 * Divides array into two halves, processes left half on a spawned thread and
 * right half on the calling thread, then merges them after joining.
 * 
 * Employs depth limit and size cutoff to avoid system thread starvation.
 */
void mergeSortMultithreaded(vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // If subproblem is large and depth limit not reached, spawn a new thread
        if (depth < MAX_THREAD_DEPTH && (right - left) > THREAD_CUTOFF_SIZE) {
            // Spawn thread for left half
            thread leftThread(mergeSortMultithreaded, ref(arr), left, mid, depth + 1);

            // Compute right half on current thread
            mergeSortMultithreaded(arr, mid + 1, right, depth + 1);

            // Synchronize
            leftThread.join();
        } else {
            // Fall back to sequential execution without thread creation overhead
            mergeSortSequential(arr, left, mid, false);
            mergeSortSequential(arr, mid + 1, right, false);
        }

        merge(arr, left, mid, right, false);
    }
}

/**
 * Generates worst-case input for merge sort:
 * Separates elements alternately so that at every merge step,
 * elements are drawn strictly alternately from both subarrays.
 */
void generateWorstCase(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<int> leftArr(n1);
        vector<int> rightArr(n2);

        for (int i = 0; i < n1; i++) leftArr[i] = arr[left + 2 * i];
        for (int i = 0; i < n2; i++) rightArr[i] = arr[left + 2 * i + 1];

        generateWorstCase(leftArr, 0, n1 - 1);
        generateWorstCase(rightArr, 0, n2 - 1);

        for (int i = 0; i < n1; i++) arr[left + i] = leftArr[i];
        for (int i = 0; i < n2; i++) arr[mid + 1 + i] = rightArr[i];
    }
}

int main() {
    cout << "========================================================\n";
    cout << "  Mini Project 8: Sequential vs Multithreaded Merge Sort \n";
    cout << "========================================================\n";

    int N;
    cout << "Enter array size N (e.g. 50000 or 200000): ";
    cin >> N;

    if (N <= 0) {
        cout << "Array size must be positive.\n";
        return 1;
    }

    // Prepare Test Cases: Best, Worst, and Random
    cout << "\nPreparing Test Scenarios...\n";

    // 1. Best Case: Already Sorted Array [1, 2, 3, ..., N]
    vector<int> best_original(N);
    for (int i = 0; i < N; i++) best_original[i] = i + 1;

    // 2. Worst Case: Generated adversarial alternating split
    vector<int> worst_original = best_original;
    generateWorstCase(worst_original, 0, N - 1);

    // 3. Average Case: Random Unsorted
    srand(42);
    vector<int> random_original(N);
    for (int i = 0; i < N; i++) random_original[i] = rand() % (N * 5);

    auto benchmark = [&](const string& name, const vector<int>& input) {
        vector<int> seq_copy = input;
        vector<int> mt_copy = input;

        // Sequential
        comparisons_seq = 0;
        auto s1 = high_resolution_clock::now();
        mergeSortSequential(seq_copy, 0, N - 1, true);
        auto e1 = high_resolution_clock::now();
        auto time_seq = duration_cast<microseconds>(e1 - s1).count();

        // Multithreaded
        auto s2 = high_resolution_clock::now();
        mergeSortMultithreaded(mt_copy, 0, N - 1, 0);
        auto e2 = high_resolution_clock::now();
        auto time_mt = duration_cast<microseconds>(e2 - s2).count();

        bool isSorted = is_sorted(seq_copy.begin(), seq_copy.end()) &&
                        is_sorted(mt_copy.begin(), mt_copy.end());

        double speedup = (time_mt > 0) ? (double)time_seq / time_mt : 1.0;

        cout << "\n--- Scenario: " << name << " ---\n";
        cout << "Sequential Time       : " << time_seq << " us (" << time_seq / 1000.0 << " ms)\n";
        cout << "Multithreaded Time    : " << time_mt << " us (" << time_mt / 1000.0 << " ms)\n";
        cout << "Speedup Factor        : " << fixed << setprecision(2) << speedup << "x\n";
        cout << "Key Comparisons (Seq) : " << comparisons_seq << "\n";
        cout << "Correctness Check     : " << (isSorted ? "PASSED (Verified)" : "FAILED") << "\n";
    };

    benchmark("Best Case (Already Sorted Array)", best_original);
    benchmark("Worst Case (Adversarial Alternating Split)", worst_original);
    benchmark("Average Case (Random Unsorted Array)", random_original);

    cout << "\n========================================================\n";
    cout << "Summary: Merge sort maintains O(N log N) asymptotic time\n";
    cout << "across all cases, while multithreading achieves parallel speedup\n";
    cout << "by processing sub-arrays concurrently on multiple CPU cores.\n";
    cout << "========================================================\n";

    return 0;
}
