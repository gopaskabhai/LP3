#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// Structure to store job details
struct Job
{
    char id;      // Job ID
    int deadline; // Deadline of job
    int profit;   // Profit if job is done before or on deadline
};

// Compare function to sort jobs by decreasing profit
bool compare(Job a, Job b)
{
    return a.profit > b.profit;
}

// Function to find maximum deadline among all jobs
int findMaxDeadline(vector<Job> &jobs)
{
    int maxDeadline = 0;
    for (auto &job : jobs)
        maxDeadline = max(maxDeadline, job.deadline);
    return maxDeadline;
}

// Function to schedule jobs for maximum profit
void jobSequencing(vector<Job> &jobs)
{
    // Step 1: Sort jobs in decreasing order of profit
    sort(jobs.begin(), jobs.end(), compare);

    // Step 2: Find maximum deadline
    int maxDeadline = findMaxDeadline(jobs);

    // Step 3: Create slots to store scheduled jobs
    vector<char> schedule(maxDeadline + 1, '-'); // '-' means empty slot

    int totalProfit = 0;

    // Step 4: Assign jobs greedily
    for (auto &job : jobs)
    {
        // Try to find a free slot for this job before its deadline
        for (int j = job.deadline; j > 0; j--)
        {
            if (schedule[j] == '-')
            {
                schedule[j] = job.id;
                totalProfit += job.profit;
                break;
            }
        }
    }

    // Display results
    cout << "Job sequence: ";
    for (int i = 1; i <= maxDeadline; i++)
    {
        if (schedule[i] != '-')
            cout << schedule[i] << " ";
    }
    cout << endl;

    cout << "Total Profit: " << totalProfit << endl;
}

int main()
{
    int n;
    cout << "Enter number of jobs: ";
    cin >> n;

    vector<Job> jobs(n);
    cout << "Enter job details (JobID Deadline Profit):\n";
    for (int i = 0; i < n; i++)
        cin >> jobs[i].id >> jobs[i].deadline >> jobs[i].profit;

    jobSequencing(jobs);

    return 0;
}
