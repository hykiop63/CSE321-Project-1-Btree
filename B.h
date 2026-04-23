#pragma once
#include<vector>
struct Node {
    std::vector<int> keys;
    std::vector<int> rids;
    std::vector<Node*> child_ptrs;
    Node(int order);
};
class B{
    private:
        int order;
        Node* root;
        void split(int key);
        void merge(int key);
    public:
        B(int d);
        void insert(int key);
        void remove(int key);
        void search(int key);


};

class BStar {
    // B*-트리 선언
};

class BPlus {
    // B+-트리 선언
};