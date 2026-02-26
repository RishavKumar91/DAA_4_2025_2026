class Solution {
  public:
  
    int find(int x, vector<int> &parent) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x], parent);
    }
  
    int kruskalsMST(int V, vector<vector<int>> &edges) {
        
        sort(edges.begin(), edges.end(), [](vector<int> &a, vector<int> &b){
            return a[2] < b[2];
        });
        
        vector<int> parent(V);
        for(int i = 0; i < V; i++)
            parent[i] = i;
        
        int ans = 0;
        
        for(auto &e : edges) {
            int u = find(e[0], parent);
            int v = find(e[1], parent);
            
            if(u != v) {
                abs += e[2];
                parent[u] = v;
            }
        }
        
        return ans;
    }
};
