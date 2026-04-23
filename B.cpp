// B.cpp
#include "B.h"
#include <iostream>

// B 클래스 내부의 생성자 구현
B::B(int d) {
    order = d;
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