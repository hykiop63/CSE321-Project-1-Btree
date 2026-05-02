#include<vector>
#include<iostream>
struct Student {
    int id;  
    std::string name;    
    std::string gender;   
    float gpa;            
    float height_weight; 
};
struct MetricResult{
    long long time;
    int splits;
    double util;
};
void B_test1(const std::vector<Student>& v,const int order);
MetricResult insertion(const std::vector<Student>& v,int order);
void insertion_parmeter_test(const std::vector<Student>& v);
void point_search_test(const std::vector<Student>& v);
void range_test(const std::vector<Student>& v);
void deletion_test(const std::vector<Student>& v);
