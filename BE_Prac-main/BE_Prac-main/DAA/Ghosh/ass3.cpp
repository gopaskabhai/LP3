#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Structure to represent an item
struct Item
{
    int value, weight;
};

// Compare items according to value/weight ratio
bool compare(Item a, Item b)
{
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2; // sort in decreasing order
}

// Function to solve Fractional Knapsack Problem
double fractionalKnapsack(int W, vector<Item> &items)
{
    // Step 1: Sort items by value/weight ratio
    sort(items.begin(), items.end(), compare);

    double totalValue = 0.0; // total profit/value
    int currentWeight = 0;   // current weight in knapsack

    // Step 2: Traverse sorted items
    for (auto &item : items)
    {
        if (currentWeight + item.weight <= W)
        {
            // Take whole item
            currentWeight += item.weight;
            totalValue += item.value;
        }
        else
        {
            // Take only fractional part of item
            int remain = W - currentWeight;
            totalValue += item.value * ((double)remain / item.weight);
            break; // Knapsack is full
        }
    }

    return totalValue;
}

int main()
{
    int n, W;
    cout << "Enter number of items: ";
    cin >> n;

    vector<Item> items(n);
    cout << "Enter value and weight of each item:\n";
    for (int i = 0; i < n; i++)
        cin >> items[i].value >> items[i].weight;

    cout << "Enter knapsack capacity: ";
    cin >> W;

    double maxValue = fractionalKnapsack(W, items);
    cout << "Maximum value in Knapsack = " << maxValue << endl;

    return 0;
}
