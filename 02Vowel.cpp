#include<iostream>
using namespace std;
int main(){
    string s;
    getline(cin,s);
    int i=0,count = 0;
    while(s[i]!='\0'){
        if(s[i]=='a'||s[i]=='e'||s[i]=='o'||s[i]=='i'||s[i]=='u')
            count++;
        i++;
    }
    cout<<count;
}
#include<iostream>
using namespace std;
int main(){
    int n ,count = 0;
    cin>>n;

    char str[n];
    // cin>>str; // its disadvantage is that it will not take input after a dash
    for(int i =0;i<n;i++){   // which is fixed by this as this doesnt take input of dash
        cin>>str[i];
    }
    int i = 0;
    while(str[i]!='\0'){
        if(str[i]=='a'||str[i]=='e'||str[i]=='o'||str[i]=='i'||str[i]=='u')
                 count++;
    i++;
    }
    cout<<count<<str;
}