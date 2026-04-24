// B.cpp
#include "B.h"
#include <iostream>

Node::Node(int order,Node* p){
    parent=p;
    is_leaf=false;
    keys.reserve(order);
    rids.reserve(order);
    child_ptrs.reserve(order+1);
}
// B 클래스 내부의 생성자 구현
B::B(int d) {
    order = d;
}
void B::split(int key){
    std::cout<<key<<"\n";
}
void B::merge(int key){
    std::cout<<key<<"\n";
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
    while(now_check!=nullptr){
        int idx=inNode_find(key,now_check);
        if(idx<now_check->keys.size() && now_check->keys[idx]==key)
            return {now_check,idx};
        if(now_check->is_leaf) break;
        now_check = now_check->child_ptrs[idx];
    }
}
// B 클래스 내부의 insert 함수 구현
void B::insert(int key,int rid) {
    std::cout << key << " 삽입 로직 실행\n";
    // 실제 B-Tree 노드 분할 및 삽입 로직 작성
    
}
void B::remove(int key){
    std::cout << key << " 검색 로직 실행\n";
}
int B::search(int key){
    std::pair<Node*,int> result=inter_search(key);
    return result.first==nullptr ? -1 : 
            result.first->rids[result.second];
}