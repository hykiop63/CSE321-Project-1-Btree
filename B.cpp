#include "B.h"
#include <iostream>

Node::Node(int order,bool leaf,Node* p){
    parent=p;
    is_leaf=leaf;
    keys.reserve(order);
    rids.reserve(order);
    child_ptrs.reserve(order+1);
}
// B 클래스 내부의 생성자 구현
B::B(int d) {
    order = d;
    root=new Node(d,true,nullptr);
}
void B::split(Node* target) {
    while (target->keys.size() >= order) {
        int mid = target->keys.size() / 2;
        int upkey = target->keys[mid];
        int uprid = target->rids[mid];
        Node* temp = new Node(order, target->is_leaf, target->parent);
        temp->keys.assign(target->keys.begin() + mid + 1, target->keys.end());
        temp->rids.assign(target->rids.begin() + mid + 1, target->rids.end());
        target->keys.erase(target->keys.begin() + mid, target->keys.end());
        target->rids.erase(target->rids.begin() + mid, target->rids.end());

        if (!target->is_leaf) {
            temp->child_ptrs.assign(target->child_ptrs.begin() + mid + 1, target->child_ptrs.end());
            target->child_ptrs.erase(target->child_ptrs.begin() + mid + 1, target->child_ptrs.end());
            
            for (Node* child : temp->child_ptrs)
                child->parent = temp;
        }

        if (target->parent == nullptr) {
            Node* new_root = new Node(order, false, nullptr);
            
            new_root->keys.push_back(upkey);
            new_root->rids.push_back(uprid);
            new_root->child_ptrs.push_back(target);
            new_root->child_ptrs.push_back(temp);
            target->parent = new_root;
            temp->parent = new_root;
            root = new_root;
            break;
        } 
        else {
            Node* parent = target->parent;
            int idx = 0;
            while (idx < parent->keys.size() && parent->keys[idx] < upkey)
                idx++;
            parent->keys.insert(parent->keys.begin() + idx, upkey);
            parent->rids.insert(parent->rids.begin() + idx, uprid);
            parent->child_ptrs.insert(parent->child_ptrs.begin() + idx + 1, temp);

            target = parent;
        }
    }
}
int B::inNode_find(int key,Node* curr_n){
    int left=0;
    int right=curr_n->keys.size()-1;
    int mid=(right+left)/2;
    while(left<=right){
        if(key==curr_n->keys[mid]) return mid;//노드내에 존재
        else if(key<curr_n->keys[mid]) right=mid-1;
        else left=mid+1;
        mid=(right+left)/2;
    }
    return left;//자식으로 이동해야함
}
// B 클래스 내부의 search 함수 구현
std::pair<Node*,int> B::inter_search(int key) {
    std::cout << key << " 검색 로직 실행\n";
    Node* now_check=root;
    while(true){
        int idx=inNode_find(key,now_check);
        if(idx<now_check->keys.size() && now_check->keys[idx]==key)
            return {now_check,idx};
        if(now_check->is_leaf) return {now_check, idx};
        now_check = now_check->child_ptrs[idx];
    }
}
// B 클래스 내부의 insert 함수 구현
void B::insert(int key,int rid) {
    std::cout << key << " 삽입 로직 실행\n";
    // 실제 B-Tree 노드 분할 및 삽입 로직 작성
    std::pair<Node*,int> target=inter_search(key);
    Node* now_leaf=target.first;
    int idx=target.second;
    if(!now_leaf->is_leaf){
        std::cout<<"이미 있는 key:"<<key<<"\n";
        return;
    }
    now_leaf->keys.insert(now_leaf->keys.begin()+idx,key);
    now_leaf->rids.insert(now_leaf->rids.begin()+idx,rid);
    split(now_leaf);
}
void B::remove(int key){
    std::cout << key << " 검색 로직 실행\n";
    std::pair<Node*,int> target=inter_search(key);
    Node* now_leaf=target.first;
    int idx=target.second;
    if(idx<now_leaf->keys.size() && now_leaf->keys[idx]==key){
        now_leaf->keys.erase(now_leaf->keys.begin() + idx);
        now_leaf->rids.erase(now_leaf->rids.begin() + idx);
    }
    else{
        std::cout<<"없는 key"<<key<<"\n";
        return;
    }
}
int B::search(int key){
    std::pair<Node*,int> result=inter_search(key);
    if(result.second<result.first->keys.size() && 
        result.first->keys[result.second]==key)
            return result.first->rids[result.second];
    return -1;
}