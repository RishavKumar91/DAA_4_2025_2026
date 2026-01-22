#include <bits/stdc++.h>
using namespace std;
int opt = 0;

void complexRec(int n) {


   if (n <= 2) {
       return;
   }


   int p = n;
   while (p > 0) {
       vector<int> temp(n);
       for (int i = 0; i < n; i++) {
           temp[i] = i ^ p;
           opt++;
       }
       p >>= 1;
   }


   vector<int> small(n);
   for (int i = 0; i < n; i++) {opt++;
       small[i] = i * i;
   }


   if (n % 3 == 0) {
       reverse(small.begin(), small.end());opt++;
   } else {
       reverse(small.begin(), small.end());opt++;
   }


   complexRec(n / 2);
   complexRec(n / 2);
   complexRec(n / 2);
}
int main(){
    int n ;
    cout<<"Number for which you want to run function for: ";
    cin>>n;
    complexRec(n);
    cout<<opt;
    
}