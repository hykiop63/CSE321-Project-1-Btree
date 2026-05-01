#pragma once
#include<vector>
#include<utility>
struct Node {
    Node* parent;
    bool is_leaf;
    std::vector<int> keys;
    std::vector<int> rids;
    std::vector<Node*> child_ptrs;
    Node* pre_leaf=nullptr;
    Node* next_leaf=nullptr;
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
    private:
        int order;
        Node* root;
        void split_root(Node* target);
        void split(Node* target);
        void merge_3_to_2(Node* l_s,Node* target,Node* r_s,Node* p,int i);
        void merge_3_to_3(Node* l_s,Node* target,Node* r_s,Node* p,int i);
        void merge_2_to_1(Node* l_s,Node* r_s,Node* p);
        void merge(Node* target);
        bool rotate(Node* target);
        bool rotate_insert(Node* target);
        void underflow(Node* target);
        void overflow(Node* target);
        int inNode_find(int key,Node* curr_n);
        std::pair<Node*,int> inter_search(int key);
    public:
        BStar(int d);
        void insert(int key,int rid);
        void remove(int key);
        int search(int key);
        void level_order();
        bool verify();
};

class BPlus {
    private:
        int order;
        Node* root;
        void split(Node* target);
        void split(Node* target,bool leaf);
        void merge(Node* target);
        void merge(Node* target,bool leaf);
        bool rotate(Node* target);
        bool rotate(Node* target,bool leaf);
        void underflow(Node* target);
        int inNode_find(int key,Node* curr_n);
        std::pair<Node*,int> inter_search(int key);
    public:
        BPlus(int d);
        void insert(int key,int rid);
        void remove(int key);
        int search(int key);
        void level_order();
        bool verify();
    // B+-트리 선언
};