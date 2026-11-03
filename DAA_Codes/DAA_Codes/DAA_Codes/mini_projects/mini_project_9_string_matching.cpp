/**
 * Mini Project 9: Naive vs Rabin-Karp String Matching Algorithm
 * 
 * Problem Statement:
 * Implement the Naive string matching algorithm and Rabin-Karp algorithm for
 * string matching. Observe difference in working of both the algorithms for the same input.
 * 
 * Features:
 * - Exact tracking of character comparisons vs hash comparisons
 * - Detection of spurious hits (hash collision where substring doesn't match)
 * - Side-by-side verification and step tracing
 * 
 * ============================================================================
 * EXAMPLE INPUT / OUTPUT TO TEST & VERIFY:
 * ============================================================================
 * Input:
 *   Text:    AABAACAADAABAABA
 *   Pattern: AABA
 * 
 * Expected Matches:
 *   Found at shift index: 0, 9, 12
 * 
 * Performance Analysis:
 *   Naive Algorithm Character Comparisons      : ~25
 *   Rabin-Karp Rolling Hash Comparisons        : 13
 *   Rabin-Karp Spurious Hits                   : 0
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Base (number of characters in alphabet) and large prime modulus for Rabin-Karp
const int RADIX = 256;
const int PRIME_MOD = 101; // Small prime to demonstrate collisions, or 1e9+7 for production

struct MatchResult {
    vector<int> matchIndices;
    int characterComparisons = 0;
    int hashComparisons = 0;
    int spuriousHits = 0; // Hash matches but characters differ
};

/**
 * Naive (Brute Force) String Matching Algorithm
 * 
 * Slides the pattern P[0..m-1] over text T[0..n-1] one character at a time.
 * For each candidate shift s, compares characters from left to right.
 * 
 * Time Complexity:
 *   Best Case: O(n) (mismatches immediately on first character)
 *   Worst Case: O(m * (n - m + 1)) (e.g. text "AAAAAAAA" and pattern "AAAAB")
 * Space Complexity: O(1)
 */
MatchResult naiveStringMatch(const string& text, const string& pattern) {
    MatchResult res;
    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m) {
            res.characterComparisons++;
            if (text[i + j] != pattern[j]) {
                break;
            }
            j++;
        }
        if (j == m) {
            res.matchIndices.push_back(i);
        }
    }
    return res;
}

/**
 * Rabin-Karp Algorithm using Rolling Hash:
 * 
 * 1. Computes hash of pattern P: h(P) = (P[0]*d^(m-1) + ... + P[m-1]) mod q
 * 2. Computes hash of first window of text T[0..m-1]
 * 3. Slides window one position forward:
 *      h(T[i+1]) = (d * (h(T[i]) - T[i] * h_factor) + T[i+m]) mod q
 * 4. Only performs full string comparison when window hash matches pattern hash!
 * 
 * Time Complexity:
 *   Average Case: O(n + m)
 *   Worst Case: O(n * m) (occurs when every window produces hash collision)
 * Space Complexity: O(1)
 */
MatchResult rabinKarpMatch(const string& text, const string& pattern, int q = PRIME_MOD) {
    MatchResult res;
    int n = text.length();
    int m = pattern.length();

    if (m > n || m == 0) return res;

    int d = RADIX;
    int p_hash = 0; // Hash value for pattern
    int t_hash = 0; // Hash value for current text window
    int h = 1;      // d^(m-1) % q

    // Calculate h = pow(d, m - 1) % q
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    // Calculate initial hash of pattern and first text window
    for (int i = 0; i < m; i++) {
        p_hash = (d * p_hash + pattern[i]) % q;
        t_hash = (d * t_hash + text[i]) % q;
    }

    // Slide the pattern over text
    for (int i = 0; i <= n - m; i++) {
        res.hashComparisons++;

        // If the hash values match, then only check for characters one by one
        if (p_hash == t_hash) {
            bool identical = true;
            for (int j = 0; j < m; j++) {
                res.characterComparisons++;
                if (text[i + j] != pattern[j]) {
                    identical = false;
                    break;
                }
            }

            if (identical) {
                res.matchIndices.push_back(i);
            } else {
                res.spuriousHits++; // Hash matched but strings differed!
            }
        }

        // Calculate rolling hash for next window
        if (i < n - m) {
            t_hash = (d * (t_hash - text[i] * h) + text[i + m]) % q;

            // Handle negative modulo in C++
            if (t_hash < 0) {
                t_hash = (t_hash + q);
            }
        }
    }

    return res;
}

int main() {
    cout << "========================================================\n";
    cout << "  Mini Project 9: Naive vs Rabin-Karp String Matching    \n";
    cout << "========================================================\n";

    string text, pattern;
    cout << "Enter Text string (or press Enter for sample 'AABAACAADAABAABA'):\n";
    getline(cin, text);
    if (text.empty()) text = "AABAACAADAABAABA";

    cout << "Enter Pattern string (or press Enter for sample 'AABA'):\n";
    getline(cin, pattern);
    if (pattern.empty()) pattern = "AABA";

    cout << "\nText    : \"" << text << "\" (Length = " << text.length() << ")\n";
    cout << "Pattern : \"" << pattern << "\" (Length = " << pattern.length() << ")\n";

    // Run both algorithms
    MatchResult naiveRes = naiveStringMatch(text, pattern);
    MatchResult rkRes = rabinKarpMatch(text, pattern);

    cout << "\n================ Execution Results ================\n";
    cout << "1. Naive Algorithm:\n";
    cout << "   - Pattern found at indices      : ";
    if (naiveRes.matchIndices.empty()) cout << "None";
    for (int idx : naiveRes.matchIndices) cout << idx << " ";
    cout << "\n   - Character Comparisons Made    : " << naiveRes.characterComparisons << "\n";

    cout << "\n2. Rabin-Karp Algorithm:\n";
    cout << "   - Pattern found at indices      : ";
    if (rkRes.matchIndices.empty()) cout << "None";
    for (int idx : rkRes.matchIndices) cout << idx << " ";
    cout << "\n   - Hash Window Comparisons Made : " << rkRes.hashComparisons << "\n";
    cout << "   - Character Comparisons (on match): " << rkRes.characterComparisons << "\n";
    cout << "   - Spurious Hits (Hash Collisions) : " << rkRes.spuriousHits << "\n";

    cout << "\n================ Comparative Analysis ================\n";
    cout << left << setw(32) << "Evaluation Factor"
         << setw(20) << "Naive Algorithm"
         << setw(20) << "Rabin-Karp" << "\n";
    cout << string(68, '-') << "\n";

    cout << left << setw(32) << "Average Time Complexity"
         << setw(20) << "O(N * M)"
         << setw(20) << "O(N + M)" << "\n";

    cout << left << setw(32) << "Worst Time Complexity"
         << setw(20) << "O(N * M)"
         << setw(20) << "O(N * M)" << "\n";

    cout << left << setw(32) << "Pre-processing Time"
         << setw(20) << "O(1)"
         << setw(20) << "O(M) (Hash calculation)" << "\n";

    cout << left << setw(32) << "Multiple Pattern Support"
         << setw(20) << "Inefficient"
         << setw(20) << "Highly Effective (Bloom)" << "\n";

    cout << string(68, '-') << "\n";
    cout << "Key Observation: Rabin-Karp uses Rolling Hash in O(1) time per shift,\n";
    cout << "avoiding character comparisons whenever the window hash does not match.\n";
    cout << "========================================================\n";

    return 0;
}
