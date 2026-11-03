// #include <iostream>
// using namespace std;

// int fibonacci_recursive(int n)
// {
//     if (n == 0)
//         return 0;
//     else if (n == 1)
//         return 1;
//     else
//         return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
// }

// int main()
// {
//     int n;
//     cout << "Enter the number of terms: ";
//     cin >> n;

//     cout << "Fibonacci Series (Recursive): ";
//     for (int i = 0; i < n; i++)
//     {
//         cout << fibonacci_recursive(i) << " ";
//     }
//     cout << endl;

//     return 0;
// }

//////// non recursive part

#include <iostream>
using namespace std;

void fibonacci_iterative(int n)
{
    int a = 0, b = 1, c;
    cout << "Fibonacci Series (Iterative): ";

    for (int i = 0; i < n; i++)
    {
        cout << a << " ";
        c = a + b;
        a = b;
        b = c;
    }
    cout << endl;
}

int main()
{
    int n;
    cout << "Enter the number of terms: ";
    cin >> n;

    fibonacci_iterative(n);

    return 0;
}
