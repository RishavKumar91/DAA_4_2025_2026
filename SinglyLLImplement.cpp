#include<iostream>
using namespace std;
class Node{
    public:
    int val;
    Node * next;
    Node(int val){
        this->val = val; this->next = NULL;
    }
};
Node * head = NULL;
void display(){
    Node * tmp = head;
    while(tmp!=NULL){
        cout<<tmp->val<<" ";
        tmp = tmp->next;
    }
    cout<<endl;
}
void insert(int x){
    if(head==NULL) head = new Node(x);
    else{
        Node * Add = new Node(x);
        Node * tmp = head;
        while(tmp->next!=NULL){
            tmp = tmp->next;
        }
        tmp->next = Add;
    }
}
void deletee(int x){
    if(head->val== x) head = head->next;
    else{
        Node * tmp = head;
        while(tmp->next->val!=x){
            tmp = tmp->next;
        }
        tmp->next = tmp->next->next;
    }
}
int main(){
    insert(87);
    insert(80);
    insert(5);
    display();
    deletee(5); deletee(87);
    display();
}