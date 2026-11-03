#include <bits/stdc++.h>
using namespace std;

struct Job {
    char id;
    int deadline;
    int profit;
};

bool comparison(Job a, Job b) {
    return (a.profit > b.profit);
}

void jobSequencing(Job jobs[], int n) {
    sort(jobs, jobs + n, comparison);
    
    int maxDeadline = 0;
    for (int i = 0; i < n; i++) {
        if (jobs[i].deadline > maxDeadline) {
            maxDeadline = jobs[i].deadline;
        }
    }
    
    int *slot = new int[maxDeadline];
    bool *scheduled = new bool[maxDeadline];
    
    for (int i = 0; i < maxDeadline; i++) {
        scheduled[i] = false;
    }
    
    int totalProfit = 0;
    int jobCount = 0;
    
    cout << "\nJob Scheduling Process:\n";
    cout << "------------------------\n";
    
    for (int i = 0; i < n; i++) {
        for (int j = min(maxDeadline, jobs[i].deadline) - 1; j >= 0; j--) {
            if (scheduled[j] == false) {
                slot[j] = i;
                scheduled[j] = true;
                totalProfit += jobs[i].profit;
                jobCount++;
                cout << "Job " << jobs[i].id << " scheduled at slot " << (j + 1) 
                    << " (Profit: " << jobs[i].profit << ")\n";
                break;
            }
        }
    }
    
    cout << "\n--- RESULTS ---\n";
    cout << "Total jobs scheduled: " << jobCount << "\n";
    cout << "Maximum profit: " << totalProfit << "\n";
    cout << "\nScheduled Jobs: ";
    for (int i = 0; i < maxDeadline; i++) {
        if (scheduled[i]) {
            cout << jobs[slot[i]].id << " ";
        }
    }
    cout << "\n";
    
    delete[] slot;
    delete[] scheduled;
}

int main() {
    int n;
    
    cout << "Enter number of jobs: ";
    cin >> n;
    
    Job *jobs = new Job[n];
    
    cout << "\nEnter job details (ID Deadline Profit):\n";
    for (int i = 0; i < n; i++) {
        cout << "Job " << (i + 1) << ": ";
        cin >> jobs[i].id >> jobs[i].deadline >> jobs[i].profit;
    }
    
    cout << "\nInput Jobs:\n";
    cout << "ID\tDeadline\tProfit\n";
    for (int i = 0; i < n; i++) {
        cout << jobs[i].id << "\t" << jobs[i].deadline << "\t\t" << jobs[i].profit << "\n";
    }
    
    jobSequencing(jobs, n);
    
    delete[] jobs;
    return 0;
}


/*
JOB SEQUENCING WITH DEADLINES - GREEDY METHOD
==============================================

Algorithm: Greedy Approach
Time Complexity: O(n²) where n is number of jobs
Space Complexity: O(n) for slot array

Problem Statement:
Given a set of jobs where each job has:
- A unique ID
- A deadline
- A profit if completed before deadline

Goal: Schedule jobs to maximize total profit
Constraint: Only one job can be scheduled at a time, each job takes 1 unit of time

Greedy Strategy:
1. Sort all jobs in descending order of profit
2. For each job, try to schedule it in the latest possible slot before its deadline
3. If a slot is found, schedule the job and add profit to total
4. If no slot is available before deadline, skip the job

How it works:
- Create a slot array to track which jobs are scheduled
- For each job (sorted by profit):
  - Check slots from (deadline-1) down to 0
  - Find the first free slot and schedule the job there
  - Mark slot as occupied

Why Greedy Works:
- We always choose the job with maximum profit first
- We schedule it as late as possible to leave room for other jobs
- This locally optimal choice leads to globally optimal solution

Algorithm Steps:
1. Sort jobs by profit (descending order)
2. Find maximum deadline to determine number of slots needed
3. Initialize slot array (all free initially)
4. For each job in sorted order:
   - Find latest free slot before its deadline
   - If found, schedule job in that slot
   - Update total profit
5. Return scheduled jobs and total profit

Example:
Jobs: [(A,2,100), (B,1,19), (C,2,27), (D,1,25), (E,3,15)]

Step 1: Sort by profit
[(A,2,100), (C,2,27), (D,1,25), (B,1,19), (E,3,15)]

Step 2: Schedule jobs
Slots: [_ _ _] (3 slots for max deadline 3)

Job A (deadline=2, profit=100): Schedule at slot 2 -> [_ A _]
Job C (deadline=2, profit=27): Schedule at slot 1 -> [C A _]
Job D (deadline=1, profit=25): Slot 1 taken, can't schedule
Job B (deadline=1, profit=19): Slot 1 taken, can't schedule
Job E (deadline=3, profit=15): Schedule at slot 3 -> [C A E]

Result: Jobs scheduled = C, A, E
Total Profit = 27 + 100 + 15 = 142

Time Complexity Analysis:
- Sorting: O(n log n)
- Finding max deadline: O(n)
- Scheduling loop: O(n) jobs × O(n) slot checks = O(n²)
- Overall: O(n²)

Space Complexity Analysis:
- Slot array: O(max_deadline)
- Scheduled array: O(max_deadline)
- Overall: O(n) in worst case when max_deadline = n

Advantages:
- Simple and intuitive approach
- Guaranteed to find optimal solution
- Works well for moderate input sizes
- Easy to implement and understand

Disadvantages:
- O(n²) time complexity can be slow for large inputs
- Requires sorting which adds overhead
- May waste time slots if deadlines are not continuous

Optimizations:
- Use Disjoint Set Union (DSU) to find free slots in O(log n)
- This improves overall complexity to O(n log n)

Real-world Applications:
- Task scheduling in operating systems
- Project deadline management
- Resource allocation with time constraints
- Production scheduling in manufacturing
- Assignment of tasks to workers with deadlines

Input Format:
First line: Number of jobs (n)
Next n lines: Job_ID Deadline Profit

Example Input:
5
A 2 100
B 1 19
C 2 27
D 1 25
E 3 15

Expected Output:
Job C scheduled at slot 1 (Profit: 27)
Job A scheduled at slot 2 (Profit: 100)
Job E scheduled at slot 3 (Profit: 15)

Total jobs scheduled: 3
Maximum profit: 142
Scheduled Jobs: C A E
*/
