#include<iostream>
using namespace std;
class DoublyLL{
    public:
    int val;
    DoublyLL * prev;
    DoublyLL * next;
    DoublyLL(int val){
        this->val = val;
        this->prev = NULL;
        this->next = NULL;
    }
};
DoublyLL * head = NULL;
void insert(int x){
    if(head == NULL) {
        head = new DoublyLL(x);
    }
    else{ DoublyLL * Add = new DoublyLL(x);
        DoublyLL * tmp = head;
        while(tmp->next!=NULL){
            tmp=tmp->next;
        }
        tmp->next = Add; Add->prev = tmp;
    }
}
void deletee(int x){
    if(head == NULL) return;
    if(head->val == x){
        DoublyLL* del = head;
        head = head->next;
        if(head) head->prev = NULL;
        delete del;
        return;
    }
    DoublyLL* tmp = head;
    while(tmp && tmp->val != x){
        tmp = tmp->next;
    }
    if(tmp == NULL) return;
    if(tmp->next)
        tmp->next->prev = tmp->prev;
    if(tmp->prev)
        tmp->prev->next = tmp->next;
    delete tmp;
}

void display(){
    DoublyLL * tmp = head;
    while(tmp!=NULL){
        cout<<tmp->val<<" ";
            tmp=tmp->next;
        }
}
int main(){
    insert(4); insert(68); insert(89); insert(99);
    deletee(89);
    display();
}