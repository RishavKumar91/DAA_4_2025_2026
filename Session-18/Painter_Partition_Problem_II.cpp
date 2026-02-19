class Solution {
  public:

    bool canPaint(vector<int>& arr, int k, int mid) {
        int painters = 1;
        int sum = 0;

        for(int i = 0; i < arr.size(); i++) {
            
            if(sum + arr[i] <= mid) {
                sum += arr[i];
            }
            else {
                painters++;
                sum = arr[i];
            }
        }

        return painters <= k;
    }

    int minTime(vector<int>& arr, int k) {

        int low = *max_element(arr.begin(), arr.end());
        int high = 0;

        for(int i = 0; i < arr.size(); i++)
            high += arr[i];

        int ans = high;

        while(low <= high) {

            int mid = low + (high - low) / 2;

            if(canPaint(arr, k, mid)) {
                ans = mid;
                high = mid - 1; 
            }
            else {
                low = mid + 1;
            }
        }

        return ans;
    }
};
