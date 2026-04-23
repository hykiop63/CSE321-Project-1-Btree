#pragma once

class B {
    private:
        int order;

    
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