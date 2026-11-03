
/*
    JOB SEQUENCING WITH DEADLINES USING A GREEDY METHOD

    Theory:
    Each job takes exactly one unit of time. A job earns its profit only when
    it is completed on or before its deadline. The objective is to select and
    arrange jobs so that the total profit is maximum.

    Greedy idea:
    1. Sort all jobs in decreasing order of profit.
    2. Visit the jobs in that order.
    3. For each job, place it in the latest free time slot not greater than
       its deadline. Using the latest possible slot leaves earlier slots
       available for jobs with smaller deadlines.
    4. Add the profit only when a free slot is found.

    Why this works:
    The most profitable available job is considered first. Placing it as late
    as possible preserves earlier slots and therefore gives the remaining
    jobs the best chance of meeting their deadlines.

    Time complexity:
    - Sorting: O(n log n)
    - Slot search: O(n * d), where d is the maximum deadline
    - Overall: O(n log n + n*d)

    Input format:
    First enter n, the number of jobs. Then enter n lines containing:
        Job_ID Deadline Profit

    Example input:
        5
        A 2 100
        B 1 19
        C 2 27
        D 1 25
        E 3 15

    Example result:
        Scheduled Jobs: C A E
        Maximum Profit: 142

    How to compile and run on Windows (using g++):
        g++ JobSequencing.cpp -o JobSequencing.exe
        .\JobSequencing.exe

    Then type the input interactively, or save the example input in input.txt
    and run:
        .\JobSequencing.exe < input.txt
*/

#include <iostream>
#include <algorithm>
using namespace std;

struct Job {
    char id;
    int deadline;
    int profit;
};

// Comparator used by sort: higher-profit jobs are considered first.
bool compare(Job a, Job b) {
    return a.profit > b.profit;
}

int main() {
    int n;

    cout << "Enter number of jobs: ";
    cin >> n;

    Job jobs[100];

    for (int i = 0; i < n; i++) {
        cout << "Enter Job ID, Deadline and Profit: ";
        cin >> jobs[i].id >> jobs[i].deadline >> jobs[i].profit;
    }

    // Greedy choice: process the most profitable jobs first.
    sort(jobs, jobs + n, compare);

    int maxDeadline = 0;

    for (int i = 0; i < n; i++) {
        maxDeadline = max(maxDeadline, jobs[i].deadline);
    }

    char slot[100];
    bool occupied[100] = {false};

    int totalProfit = 0;

    // Put each job as late as possible before its deadline.
    // This preserves earlier slots for jobs with tighter deadlines.
    for (int i = 0; i < n; i++) {
        for (int j = min(jobs[i].deadline, maxDeadline); j >= 1; j--) {
            if (!occupied[j]) {
                slot[j] = jobs[i].id;
                occupied[j] = true;
                totalProfit += jobs[i].profit;
                break;
            }
        }
    }

    cout << "\nScheduled Jobs: ";

    for (int i = 1; i <= maxDeadline; i++) {
        if (occupied[i]) {
            cout << slot[i] << " ";
        }
    }

    cout << "\nMaximum Profit: " << totalProfit << endl;

    return 0;
}


// g++ job_sequencing.cpp -o job_sequencing
// ./job_sequencing

// g++ fractional_knapsack.cpp -o fractional_knapsack
// ./fractional_knapsack
