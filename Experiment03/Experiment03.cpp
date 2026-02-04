#include <bits/stdc++.h>
using namespace std;

void maximumStableWindows() {
    int n, present = 0, absent = 0, final = 0;
    cin >> n;

    vector<char> entries(n);
    unordered_map<int, int> mapp;
    mapp[0] = -1;

    for (char &t: entries) cin >> t;
    for (int i = 0; i < n; i++) {
        if (entries[i] == 'P') present++;
        else absent++;
        if (!mapp.count(present - absent)) mapp[present - absent] = i;
        if (present == absent) final = max(final, present + absent);
        else {
            int idx = mapp[present - absent];
            final = max(final, i - idx);
        }
    }
    cout << final << endl;
}
int main() {
    maximumStableWindows();
    return 0;
}
