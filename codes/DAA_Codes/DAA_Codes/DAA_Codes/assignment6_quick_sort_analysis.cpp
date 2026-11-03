/**
 * Assignment 6: Deterministic vs Randomized Quick Sort Analysis
 * 
 * Problem Statement:
 * Write a program for analysis of quick sort by using deterministic and randomized variant.
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter array size (e.g., 1000): 1000
 *   Choose input distribution:
 *     1. Random array
 *     2. Already sorted array (Worst case for deterministic first/last element pivot)
 *     3. Reverse sorted array
 * 
 * Expected Output:
 *   Deterministic Quick Sort Comparisons : 499500 (for already sorted O(n^2))
 *   Randomized Quick Sort Comparisons    : ~11000 (O(n log n) expected)
 *   Detailed comparison and execution time shown.
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Counters for algorithmic analysis
long long comp_deterministic = 0;
long long comp_randomized = 0;
long long swaps_deterministic = 0;
long long swaps_randomized = 0;

/**
 * Standard Lomuto Partitioning Scheme
 * deterministic: Uses the last element arr[high] as pivot.
 */
int partitionDeterministic(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // deterministic pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {
        comp_deterministic++;
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
            swaps_deterministic++;
        }
    }
    swap(arr[i + 1], arr[high]);
    swaps_deterministic++;
    return i + 1;
}

/**
 * Deterministic Quick Sort
 * Worst-case Time Complexity: O(n^2) when array is already sorted/reverse-sorted
 * Average-case Time Complexity: O(n log n)
 * Space Complexity: O(log n) auxiliary stack space (O(n) in worst case)
 */
void quickSortDeterministic(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionDeterministic(arr, low, high);
        quickSortDeterministic(arr, low, pi - 1);
        quickSortDeterministic(arr, pi + 1, high);
    }
}

/**
 * Randomized Partitioning Scheme:
 * Randomly picks a pivot index between low and high, swaps it with arr[high],
 * and then executes standard partition.
 */
int partitionRandomized(vector<int>& arr, int low, int high) {
    // Generate random index between low and high
    int randomIndex = low + rand() % (high - low + 1);
    swap(arr[randomIndex], arr[high]);
    swaps_randomized++;

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        comp_randomized++;
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
            swaps_randomized++;
        }
    }
    swap(arr[i + 1], arr[high]);
    swaps_randomized++;
    return i + 1;
}

/**
 * Randomized Quick Sort
 * Avoids the O(n^2) worst-case trap on sorted/partially sorted inputs.
 * Expected Time Complexity: O(n log n) with high probability on any input!
 * Space Complexity: O(log n) expected stack depth
 */
void quickSortRandomized(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionRandomized(arr, low, high);
        quickSortRandomized(arr, low, pi - 1);
        quickSortRandomized(arr, pi + 1, high);
    }
}

int main() {
    // Seed random number generator
    srand(time(nullptr));

    cout << "========================================================\n";
    cout << "   DAA Lab Assignment 6: Quick Sort Analysis            \n";
    cout << "   (Deterministic vs Randomized Variant)                \n";
    cout << "========================================================\n";

    int n;
    cout << "Enter array size N (e.g. 1000 or 5000): ";
    cin >> n;

    if (n <= 0) {
        cout << "Array size must be positive.\n";
        return 0;
    }

    cout << "\nChoose Input Data Distribution:\n";
    cout << "  1. Already Sorted Array (Triggers Worst-Case in Deterministic)\n";
    cout << "  2. Reverse Sorted Array\n";
    cout << "  3. Random Unsorted Array\n";
    cout << "Enter choice (1, 2, or 3): ";
    int choice;
    cin >> choice;

    vector<int> original(n);
    if (choice == 1) {
        for (int i = 0; i < n; i++) original[i] = i + 1;
        cout << "Generated Already Sorted array of size " << n << ".\n";
    } else if (choice == 2) {
        for (int i = 0; i < n; i++) original[i] = n - i;
        cout << "Generated Reverse Sorted array of size " << n << ".\n";
    } else {
        for (int i = 0; i < n; i++) original[i] = rand() % 10000;
        cout << "Generated Random array of size " << n << ".\n";
    }

    // Copies for both sorts
    vector<int> arr1 = original;
    vector<int> arr2 = original;

    // Reset counters
    comp_deterministic = 0;
    swaps_deterministic = 0;
    comp_randomized = 0;
    swaps_randomized = 0;

    // 1. Run Deterministic Quick Sort
    auto start1 = high_resolution_clock::now();
    quickSortDeterministic(arr1, 0, n - 1);
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(end1 - start1).count();

    // 2. Run Randomized Quick Sort
    auto start2 = high_resolution_clock::now();
    quickSortRandomized(arr2, 0, n - 1);
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end2 - start2).count();

    // Verify sorting correctness
    bool correct1 = is_sorted(arr1.begin(), arr1.end());
    bool correct2 = is_sorted(arr2.begin(), arr2.end());

    cout << "\n================ Performance Analysis Table ================\n";
    cout << left << setw(28) << "Metric" 
         << setw(22) << "Deterministic" 
         << setw(22) << "Randomized" << "\n";
    cout << string(70, '-') << "\n";

    cout << left << setw(28) << "Pivot Strategy" 
         << setw(22) << "Fixed (arr[high])" 
         << setw(22) << "Random element" << "\n";

    cout << left << setw(28) << "Total Comparisons" 
         << setw(22) << comp_deterministic 
         << setw(22) << comp_randomized << "\n";

    cout << left << setw(28) << "Total Swaps" 
         << setw(22) << swaps_deterministic 
         << setw(22) << swaps_randomized << "\n";

    cout << left << setw(28) << "Execution Time (us)" 
         << setw(22) << duration1 
         << setw(22) << duration2 << "\n";

    cout << left << setw(28) << "Correctly Sorted?" 
         << setw(22) << (correct1 ? "Yes" : "No") 
         << setw(22) << (correct2 ? "Yes" : "No") << "\n";

    cout << string(70, '-') << "\n";

    if (choice == 1 || choice == 2) {
        cout << "\nObservation: Notice how on sorted/reverse data, Deterministic Quick Sort\n";
        cout << "performs ~ N^2/2 comparisons (" << comp_deterministic 
             << "), while Randomized Quick Sort\n";
        cout << "achieves optimal O(N log N) performance (" << comp_randomized 
             << " comparisons)!\n";
    }
    cout << "============================================================\n";

    return 0;
}
