class Solution {
  public:
    vector<int> maxOfSubarrays(vector<int>& arr, int k) {
        // code here
        int n = arr.size();
        vector<int> ans;
        priority_queue<pair<int,int>> pq;
        int i = 0;
        for(i=0;i<k;i++){
            pq.push({arr[i],i});
        }
        
        while(i<n+1){
            while(!pq.empty() && pq.top().second < i-k)
            pq.pop();
            ans.push_back(pq.top().first);
            pq.push({arr[i],i});
            i++;
        }
    return ans;
    }
};
