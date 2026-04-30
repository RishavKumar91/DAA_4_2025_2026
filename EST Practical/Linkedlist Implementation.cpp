#include <iostream>
using namespace std;

class Stack {
    struct Node {
        int data;
        Node* next;
    };
    Node* head;
    int count;

public:
    Stack() {
        head = NULL;
        count = 0;
    }

    void push(int x) {
        Node* temp = new Node();
        temp->data = x;
        temp->next = head;
        head = temp;
        count++;
    }

    void pop() {
        if (head == NULL) return;
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
    }

    int top() {
        if (head == NULL) return -1;
        return head->data;
    }

    int size() {
        return count;
    }

    bool empty() {
        return head == NULL;
    }
};

int main() {
    Stack s;
    s.push(10);
    s.push(20);
    s.push(30);

    cout << s.top() << endl;
    cout << s.size() << endl;

    s.pop();

    cout << s.top() << endl;
    cout << s.empty() << endl;

    return 0;
}
