#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include <cassert>
#include "B.h"
#include "test.h"
int main() {
    std::vector<Student> student_array;
    std::ifstream file("student.csv"); 

    if (!file.is_open()) {
        std::cerr << "CSV 파일을 열 수 없습니다.\n";
        return 1;
    }

    std::string line;
    
    std::getline(file, line); 
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Student temp_student;
        std::getline(ss, token, ',');
        temp_student.id = std::stoi(token); 
        std::getline(ss, temp_student.name, ',');
        std::getline(ss, temp_student.gender, ',');
        std::getline(ss, token, ',');
        temp_student.gpa = std::stof(token);
        std::getline(ss, token, ',');
        temp_student.height_weight = std::stof(token);
        student_array.push_back(temp_student);
    }

    file.close();
    std::cout << "총 로드된 데이터 수: " << student_array.size() << "건\n";
    if (!student_array.empty()) {
        std::cout << "[테스트] RID 0번의 학번(Key): " << student_array[0].id 
                  << ", 이름: " << student_array[0].name << "\n";
    }
    /*
    BStar Btree(3);
    for(int i=0;i<100000;i++){
        //std::cout<<"insert :"<<i<<" "<<student_array[i].id<<"\n";
        Btree.insert(student_array[i].id,i);
        //Btree.level_order();
        //assert(Btree.verify()==true); 
        
    }
    for(int i=0;i<100000;i++){
        std::cout<<"insert :"<<i<<" "<<student_array[i].id<<"\n";
        Btree.remove(student_array[i].id);
        assert(Btree.verify()==true); 
        //Btree.level_order();
    }*/
    int max_d=5001;
    insertion_parmeter_test(student_array,max_d);
    point_search_test(student_array,max_d);
    range_test(student_array,max_d);
    deletion_test(student_array,max_d);
    return 0;
}