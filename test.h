#include<vector>
#include<iostream>
struct Student {
    int id;  
    std::string name;    
    std::string gender;   
    float gpa;            
    float height_weight; 
};
void B_test1(const std::vector<Student>& v,const int order);
void insertion(const std::vector<Student>& v,int order);
void insertion_parmeter_test(const std::vector<Student>& v);
