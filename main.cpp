#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include "B.h"
struct Student {
    int id;               // Key (학번)
    std::string name;     // 이름
    std::string gender;   // 성별
    float gpa;            // 평점
    float height_weight;  // 키/몸무게
};
int main() {
    // 10만 개의 레코드를 담을 In-memory array
    // 이 배열의 '인덱스'가 곧 트리에 저장될 'RID'가 됩니다.
    std::vector<Student> student_array;
    
    // 파일 열기 (csv 파일이 실행 파일과 같은 디렉토리에 있어야 합니다)
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

    return 0;
}