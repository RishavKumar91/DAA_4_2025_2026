class Solution {
  public:
    int spanningTree(int V, vector<vector<int>>& edges) {
        // code here
        vector<bool> visit(V,0);
        vector<list<pair<int,int>>> aj(V);
        for(int i =0;i<edges.size();i++){
            int u = edges[i][0];
            int v = edges[i][1];
            int w = edges[i][2];
            aj[u].push_back({v,w});
            aj[v].push_back({u,w});
        }
        int ans  = 0;
        priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;        
        q.push({0,0});
        while(!q.empty()){
            auto [wt,node] = q.top();
            q.pop();
            if(visit[node]) continue;
            visit[node] = 1;
            ans+=wt;
            for(auto x  : aj[node]){
                if(visit[x.first]) continue;
                q.push({x.second,x.first});
            }
        }
    return ans;
    }
};
