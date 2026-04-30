#include"test.h"
#include"B.h"
#include<cassert>
#include<chrono>
#include<utility>
#include<algorithm>
#include<random>
using Time=std::chrono::duration<double, std::micro>;
int getRandom(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}
void B_test1(const std::vector<Student>& v,const int order){
    B Btree(order);
    for(int i=0;i<100000;i++){
        //std::cout<<"insert :"<<i<<" "<<v[i].id<<"\n";
        Btree.insert(v[i].id,i);
        assert(Btree.verify()==true); 
        //Btree.level_order();
    }
    for(int i=0;i<100000;i++){
        if(v[Btree.search(v[i].id)].id!=v[i].id)
            std::cout<<"mismatch:"<<i<<"\n";
    }
    for(int i=10;i<100000;i++){
        //std::cout<<"delete :"<<i<<" "<<v[i].id<<"\n";
        Btree.remove(v[i].id);
        assert(Btree.verify()==true); 
        
    }
}
void B_test2(const std::vector<Student>& v,const int order){
    B Btree(order);
    for(int j=0;j<1000;j++){
        for(int i=j*100;i<(j+1)*100;i++){
            //std::cout<<"insert :"<<i<<" "<<v[i].id<<"\n";
            Btree.insert(v[i].id,i);
            assert(Btree.verify()==true); 
            //Btree.level_order();
        }
        for(int i=j*100;i<(j+1)*100;i++){
            if(v[Btree.search(v[i].id)].id!=v[i].id)
                std::cout<<"mismatch:"<<i<<"\n";
        }
        for(int i=j*100;i<(j+1)*100;i++){
            //std::cout<<"delete :"<<i<<" "<<v[i].id<<"\n";
            Btree.remove(v[i].id);
            assert(Btree.verify()==true); 
            
        }
    }
}
void insertion(const std::vector<Student>& v,int order){
    B Btree(order);
    for(int i=0;i<100000;i++){
        //std::cout<<"insert :"<<i<<" "<<v[i].id<<"\n";
        Btree.insert(v[i].id,i);
        assert(Btree.verify()==true); 
        //Btree.level_order();
    }
}
void insertion_parmeter_test(const std::vector<Student>& v){
    std::vector<std::pair<Time,int>> total;
    for(int d=1;d<=100000;d++){
        auto start = std::chrono::high_resolution_clock::now();
        insertion(v,d);
        auto end = std::chrono::high_resolution_clock::now();
        total.push_back({end-start,d});
    }
    std:: sort(total.begin(),total.end(),[](const std::pair<Time,int>&a,const std::pair<Time,int>&b){
        return a.first.count()<b.first.count();
    });
    std::cout<<total[0].first.count()<<" "<<total[0].second<<"\n";
    std::cout<<total.back().first.count()<<" "<<total.back().second<<"\n";
}
void point_search_test(const std::vector<Student>& v){//same d.
    std::vector<int> target(10000);
    for(int i=0;i<10000;i++)
        target[i]=getRandom(0,99999);
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
        
}