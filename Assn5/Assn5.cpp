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
    static const int HASH_SIZE = 1024;
    Node* arr[HASH_SIZE];

public:
    HashMap() {
        for (int i = 0; i < HASH_SIZE; i++) {
            arr[i] = nullptr;
        }
    }

    int hash(int v) {
        return (v * 8321648 + 213846) % HASH_SIZE;
    }

    void insert(int k, int v) {
        Node* node = new Node(k, v);
        int idx = hash(k);
        if (arr[idx] == nullptr) {
            arr[idx] = node;
            return;
        }

        Node* now = arr[idx];
        while (now->next) {
            now = now->next;
        }

        now->next = node;
        node->prev = now;
    }

    void erase(int k) {
        int idx = hash(k);
        if (arr[idx] == nullptr) {
            cout << "No Value" << endl;
            return;
        }

        Node* now = arr[idx];
        while (now) {
            if (now->key == k) {
                if (now->prev == nullptr) {
                    arr[idx] = now->next;
                    if (now->next) {
                        now->next->prev = nullptr;
                    }
                }
                else {
                    now->prev->next = now->next;
                    if (now->next) {
                        now->next->prev = now->prev;
                    }
                }

                delete now;
                return;
            }

            now = now->next;
        }

        cout << "No Value" << endl;
    }

    void showAll() {
        for (int i = 0; i < HASH_SIZE; i++) {
            if (arr[i]) {
                Node* now = arr[i];
                while (now) {
                    cout << "key: " << now->key << " value: " << now->value << endl;
                    now = now->next;
                }
            }
        }
    }

    int find(int k) {
        int idx = hash(k);
        if (arr[idx] == nullptr) {
            cout << "No Value" << endl;
            return 0;
        }
        
        Node* now = arr[idx];
        while (now) {
            if (now->key == k) {
                return now->value;
            }

            now = now->next;
        }

        cout << "No Value" << endl;
        return 0;
    }
};

int main()
{
    HashMap* hm = new HashMap();
    hm->insert(5, 10);
    hm->insert(10, 15);
    hm->insert(20, 25);
    hm->insert(30, 35);
    hm->erase(20);

    hm->showAll();
    cout << hm->find(10) << endl;;
    cout << hm->find(20) << endl;
}

