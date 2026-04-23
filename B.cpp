// B.cpp
#include "B.h"
#include <iostream>

Node::Node(int order){
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
// B 클래스 내부의 insert 함수 구현
void B::insert(int key) {
    std::cout << key << " 삽입 로직 실행\n";
    // 실제 B-Tree 노드 분할 및 삽입 로직 작성
}
void B::remove(int key){
    std::cout << key << " 검색 로직 실행\n";
}
// B 클래스 내부의 search 함수 구현
void B::search(int key) {
    std::cout << key << " 검색 로직 실행\n";
    // 실제 검색 로직 작성
}