#include<bits/stdc++.h>
using namespace std;
int heap[100];
int heapSize=0;

void heapifyDown(int i ){
  int small = i;
  int lft = 2*i+1;
  int rgt = 2*i+2;     
  if(lft<heapSize && heap[small] > heap[lft]) 
    small= lft;
  if(rgt<heapSize && heap[small]>heap[rgt]) 
    small =rgt;
  if(small!=i) {
    swap(heap[i],heap[small]);
    heapifyDown(small);
    }
}

void heapifyUp(int i) {
    while(i>0 && heap[(i-1)/2] > heap[i]) {
        swap(heap[(i-1)/2] , heap[i]);
        i =(i-1)/2;
    }
}

void insert(int val) {
  if(heapSize ==100) {
    cout<<"overflow" ;
    return ;
  }
    heap[heapSize] = val;
    heapSize++;
    heapifyUp(heapSize-1);
}

void deletee() {
  if(heapSize ==0) {
		cout<<"no element";
    return ;
  }
  heap[0] = heap[heapSize-1];
  heapSize--;
  heapifyDown(0);
}
void deleteValue(int val) {
    int index = -1;
    for(int i = 0; i < heapSize; i++) {
        if(heap[i] == val) {
            index = i;
            break;
        }
    }
    if(index == -1) {
        cout << "Element not found\n";
        return;
    }
    heap[index] = heap[heapSize - 1];
    heapSize--;
    heapifyUp(index);
    heapifyDown(index);

    cout << "Deleted: " << val << endl;
}

int main() {
  insert(1);
  insert(14);
  insert(166);
  deletee();
  insert(98);
  for(int i =0;i<heapSize;i++) 
    cout<<heap[i]<< " ";
    deleteValue(14);
}