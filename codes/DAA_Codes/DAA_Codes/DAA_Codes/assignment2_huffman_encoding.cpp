/**
 * Assignment 2: Huffman Encoding using Greedy Strategy
 * 
 * Problem Statement:
 * Write a program to implement Huffman Encoding using a greedy strategy.
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Enter number of distinct characters: 6
 *   Enter character and its frequency for each:
 *   a 5
 *   b 9
 *   c 12
 *   d 13
 *   e 16
 *   f 45
 * 
 * Expected Output:
 *   Character Huffman Codes:
 *   f: 0
 *   c: 100
 *   d: 101
 *   a: 1100
 *   b: 1101
 *   e: 111
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>

using namespace std;

// Structure for a node in the Huffman Tree
struct MinHeapNode {
    char data;               // Input character ('$' for internal non-leaf nodes)
    unsigned freq;           // Frequency of occurrence
    MinHeapNode *left, *right; // Left and right child pointers

    MinHeapNode(char data, unsigned freq) {
        this->left = this->right = nullptr;
        this->data = data;
        this->freq = freq;
    }
};

// Comparison functor for min-heap priority queue
struct Compare {
    bool operator()(MinHeapNode* l, MinHeapNode* r) {
        // Lower frequency has higher priority (min-heap)
        return (l->freq > r->freq);
    }
};

/**
 * Traverses the Huffman Tree from root to leaves and prints/stores codes
 * - Append '0' when moving to the left child
 * - Append '1' when moving to the right child
 */
void printCodes(MinHeapNode* root, string str, map<char, string>& huffmanCode) {
    if (!root) return;

    // Leaf node contains an actual character
    if (!root->left && !root->right) {
        huffmanCode[root->data] = (str.empty() ? "0" : str);
        return;
    }

    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

/**
 * Builds the Huffman Tree using a greedy strategy (Min-Priority Queue):
 * 1. Create a leaf node for each character and insert it into min-heap.
 * 2. Extract two nodes with the lowest frequencies.
 * 3. Create a new internal node with frequency = sum of both extracted nodes.
 * 4. Insert the new node back into the min-heap.
 * 5. Repeat until only one node (the root) remains.
 * 
 * Time Complexity: O(n log n) where n is the number of distinct characters
 * Space Complexity: O(n) for the tree and priority queue
 */
void buildHuffmanTree(const vector<char>& data, const vector<int>& freq, int n) {
    // Min-heap priority queue
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, Compare> minHeap;

    // Step 1: Create a leaf node for each unique character
    for (int i = 0; i < n; ++i) {
        minHeap.push(new MinHeapNode(data[i], freq[i]));
    }

    // Step 2-4: Iterate until min heap contains only root node
    while (minHeap.size() > 1) {
        // Extract the two lowest frequency nodes
        MinHeapNode *left = minHeap.top();
        minHeap.pop();

        MinHeapNode *right = minHeap.top();
        minHeap.pop();

        // Create new internal node with '$' as dummy char and sum of frequencies
        MinHeapNode *top = new MinHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        // Add back to the priority queue
        minHeap.push(top);
    }

    // The remaining node is the root of the Huffman Tree
    MinHeapNode* root = minHeap.top();

    // Map to hold generated codes
    map<char, string> huffmanCode;
    printCodes(root, "", huffmanCode);

    // Display the generated variable-length prefix codes
    cout << "\n================ Huffman Codes ================\n";
    cout << "Char\tFrequency\tHuffman Code\tBit Length\n";
    cout << "-----------------------------------------------\n";
    for (int i = 0; i < n; i++) {
        cout << "'" << data[i] << "'\t" 
             << freq[i] << "\t\t" 
             << huffmanCode[data[i]] << "\t\t"
             << huffmanCode[data[i]].length() << "\n";
    }
    cout << "===============================================\n";
}

int main() {
    cout << "========================================================\n";
    cout << "   DAA Lab Assignment 2: Huffman Encoding (Greedy)      \n";
    cout << "========================================================\n";

    int n;
    cout << "Enter number of distinct characters: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of characters. Must be >= 1.\n";
        return 0;
    }

    vector<char> chars(n);
    vector<int> freq(n);

    cout << "Enter character and its frequency for each (" << n << " lines):\n";
    for (int i = 0; i < n; i++) {
        cout << "Item " << i + 1 << " (e.g. 'a 5'): ";
        cin >> chars[i] >> freq[i];
    }

    buildHuffmanTree(chars, freq, n);

    return 0;
}
