#pragma once
#include<vector>
#include<utility>
struct Node {
    Node* parent;
    bool is_leaf;
    std::vector<int> keys;
    std::vector<int> rids;
    std::vector<Node*> child_ptrs;
    Node(int order,bool leaf,Node* p);
};
class B{
    private:
        int order;
        Node* root;
        void split(Node* target);
        void merge(Node* target);
        bool rotate(Node* target);
        void underflow(Node* target);
        int inNode_find(int key,Node* curr_n);
        std::pair<Node*,int> inter_search(int key);
    public:
        B(int d);
        void insert(int key,int rid);
        void remove(int key);
        int search(int key);
        void level_order();
        bool verify();
        


};

class BStar {
    // B*-트리 선언
};

class BPlus {
    // B+-트리 선언
};