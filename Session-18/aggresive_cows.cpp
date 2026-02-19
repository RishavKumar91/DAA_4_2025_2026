class Solution {
public:

    bool canPlace(vector<int>& stalls, int k, int mid) {
        int cow = 1;
        int last = stalls[0];
        
        for (int i = 1; i < stalls.size(); i++) {
            if (stalls[i] - last >= mid) {
                cow++;
                last = stalls[i];
            }
            if (cow >= k)
                return true;
        }
        
        return false;
    }

    int aggressiveCows(vector<int> &stalls, int k) {
        sort(stalls.begin(), stalls.end());
        int n = stalls.size();

        int left = 1;
        int right = stalls[n-1] - stalls[0];
        int ans = 0;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (canPlace(stalls, k, mid)) {
                ans = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return ans;
    }
};
