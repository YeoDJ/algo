#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> arr;

    for (int i = 1; i <= (int)sqrt(n); i++)
        if (!(n % i)) {
            arr.push_back(i);
            if (n / i != i)
                arr.push_back(n / i);
        }

    sort(arr.begin(), arr.end());
    for (auto &&i : arr)
        cout << i << ' ';
    return 0;
}