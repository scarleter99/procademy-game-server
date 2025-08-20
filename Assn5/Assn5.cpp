#include <iostream>

using namespace std;

class Node {
public:
    int key;
    int value;
    Node* prev = nullptr;
    Node* next = nullptr;

public:
    Node(int k, int v)
        : key(k),
        value(v)
        {}
};

class HashMap
{
public:
    Node* arr[1024];

public:
    int hash(int v) {
        return v;
    }

    void insert(int k, int v) {
        Node* node = new Node(k, v);
        int idx = hash(k);
        if (arr[idx] == nullptr) {
            arr[idx] = node;
        }
        else {
            Node* now = arr[idx];
            while (now->next) {
                now = now->next;
            }

            now->next = node;
        }
    }

    void erase(int k) {
        int idx = hash(k);
        if (arr[idx] == nullptr) {
            cout << "No Value" << endl;
        }
        else {
            Node* now = arr[idx];
            while (now) {
                if (now->key == k) {
                    if (now->prev) {
                        now->prev->next = now->next;
                    }
                    if (now->next) {
                        now->next->prev = now->prev;
                    }

                    delete now;
                    return;
                }

                now = now->next;
            }

            cout << "No Value" << endl;
        }
    }
};

int main()
{
    
}

