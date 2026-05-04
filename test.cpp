#include"test.h"
#include"B.h"
#include<cassert>
#include<chrono>
#include<utility>
#include <fstream>
#include<algorithm>
#include<random>
using Time=std::chrono::duration<double, std::milli>;

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
template <typename TreeClass>MetricResult insertion(const std::vector<Student>& v,int order){
    TreeClass Btree(order);
    
    auto start=std::chrono::high_resolution_clock::now();
    for(int i=0;i<100000;i++){
        Btree.insert(v[i].id,i);
    }
    auto end=std::chrono::high_resolution_clock::now();
    Btree.level_order();
    assert(Btree.verify()==true); 
    MetricResult result;
    result.time=Time(end-start).count();
    result.splits=Btree.split_count; 
    result.util=(double)Btree.total_elements/((double)Btree.total_nodes*(double)order);   
    
    return result;
}
void insertion_parmeter_test(const std::vector<Student>& v,int max_d){
    std::ofstream file("insert.csv");
    file << "order,"
         << "B_time,B_split,B_util,"
         << "BPlus_time,BPlus_split,BPlus_util,"
         << "BStar_time,BStar_split,BStar_util\n";
    for(int d=3;d<max_d;d++){
        MetricResult b = insertion<B>(v, d);
        //std::cout<<"b\n";
        MetricResult bpuls = insertion<BPlus>(v, d);
        //std::cout<<"bp\n";
        MetricResult bstar = insertion<BStar>(v, d);
        //std::cout<<"bs\n";
        file << d << ","
             << b.time << "," << b.splits << "," << b.util << ","
             << bpuls.time << "," << bpuls.splits << "," << bpuls.util << ","
             << bstar.time << "," << bstar.splits << "," << bstar.util << "\n";
        std::cout<<"Order "<<d<<" Done.\n";
    }
    file.close();
    std::cout<<"CSV for insertion\n";
}
void point_search_test(const std::vector<Student>& v,int max_d){//same d.
    std::ofstream file("point_search.csv");
    file << "order,"
         << "B_time,"
         << "BPlus_time,"
         << "BStar_time\n";
    std::vector<int> target(100000);
    for(int i=0;i<100000;i++)
        target[i]=getRandom(0,99999);
    for(int d=3;d<max_d;d++){
        std::cout<<d<<"\n";
        file << d << ",";
        B Btree(d);
        BPlus Bplus(d);
        BStar Bstar(d);
        for(int i=0;i<100000;i++){
            Btree.insert(v[i].id,i);
            Bplus.insert(v[i].id,i);
            Bstar.insert(v[i].id,i);
        }
        auto start = std::chrono::high_resolution_clock::now();
        for(int j=0;j<100000;j++)
            if(target[j]!=Btree.search(v[target[j]].id)){
                std::cout<<"rid:"<<target[j]<<" "<<Btree.search(v[target[j]].id)<<"\n";
                std::cout<<"error";
                file.close();
                return;
            }
        auto end = std::chrono::high_resolution_clock::now();
        file << (long long)((end-start).count()/100000) << ",";
        start = std::chrono::high_resolution_clock::now();
        for(int i=0;i<1;i++)
            for(int j=0;j<100000;j++)
                if(target[j]!=Bplus.search(v[target[j]].id)){
                    std::cout<<"rid:"<<target[j]<<" "<<Btree.search(v[target[j]].id)<<"\n";
                    std::cout<<"error";
                    file.close();
                    return;
                }
        end = std::chrono::high_resolution_clock::now();
        file << (long long)((end-start).count()/100000) << ",";
        start = std::chrono::high_resolution_clock::now();
        for(int i=0;i<1;i++)
            for(int j=0;j<100000;j++)
                if(target[j]!=Bstar.search(v[target[j]].id)){
                    std::cout<<"rid:"<<target[j]<<" "<<Btree.search(v[target[j]].id)<<"\n";
                    std::cout<<"error";
                    file.close();
                    return;
                }
        end = std::chrono::high_resolution_clock::now();
        file << (long long)((end-start).count()/100000) << "\n";
    }
        
}
void range_test(const std::vector<Student>& v,int max_d){//same d.
    std::ofstream file("range.csv");
    file << "order,"
         << "avg length,"
         << "B_time,"
         << "BPlus_time,"
         << "BStar_time\n";
    std::vector<int> target_min(1000);
    std::vector<int> target_max(1000);
    for(int i=0;i<1000;i++){
        target_min[i]=202000000+getRandom(0,699999);
        target_max[i]=getRandom(target_min[i],202699999);
    }   
    for(int d=3;d<max_d;d++){
        std::cout<<d<<"\n";
        file << d << ",";
        B Btree(d);
        BPlus Bplus(d);
        BStar Bstar(d);
        for(int i=0;i<100000;i++){
            Btree.insert(v[i].id,i);
            Bplus.insert(v[i].id,i);
            Bstar.insert(v[i].id,i);
        }
        int length=0;
        for(int j=0;j<1000;j++){
            length+=target_max[j]-target_min[j];
        }
        file << length/1000 << ",";
        length=0;
        auto start = std::chrono::high_resolution_clock::now();
        for(int j=0;j<1000;j++)
            length+=Btree.range(target_min[j],target_max[j]).size();
        auto end = std::chrono::high_resolution_clock::now();
        file << (long long)((end-start).count()/length) << ",";
        length=0;
        start = std::chrono::high_resolution_clock::now();
        for(int j=0;j<1000;j++)
            length+=Bplus.range(target_min[j],target_max[j]).size();
        end = std::chrono::high_resolution_clock::now();
        file << (long long)((end-start).count()/length) << ",";
        length=0;
        start = std::chrono::high_resolution_clock::now();
        for(int j=0;j<1000;j++)
            length+=Bstar.range(target_min[j],target_max[j]).size();
        end = std::chrono::high_resolution_clock::now();
        file << (long long)((end-start).count()/length) << "\n";
    }
}
void deletion_test(const std::vector<Student>& v,int max_d){
    std::ofstream file("deletion.csv");
    file << "order,"
         << "B_time,B_merge,"
         << "BPlus_time,BPlus_merge,"
         << "BStar_time,BStar_merge\n";
    std::vector<int> target(100000);
    for(int i=0;i<10000;i++)
        target[i]=getRandom(0,99999);
    for(int d=4;d<max_d;d++){
        std::cout<<d<<"\n";
        file << d << ",";
        B Btree(d);
        BPlus Bplus(d);
        BStar Bstar(d);
        for(int i=0;i<100000;i++){
            Btree.insert(v[i].id,i);
            Bplus.insert(v[i].id,i);
            Bstar.insert(v[i].id,i);
        }
        auto start = std::chrono::high_resolution_clock::now();
        for(int j=0;j<10000;j++)
            Btree.remove(v[target[j]].id);
        auto end = std::chrono::high_resolution_clock::now();
        assert(Btree.verify()==true);
        file << (long long)((end-start).count()/10000) << "," <<Btree.merge_count << ",";
        start = std::chrono::high_resolution_clock::now();
        for(int j=0;j<10000;j++)
            Bplus.remove(v[target[j]].id);
        end = std::chrono::high_resolution_clock::now();
        assert(Bplus.verify()==true);
        file << (long long)((end-start).count()/10000) << "," <<Bplus.merge_count << ",";
        start = std::chrono::high_resolution_clock::now();
        for(int j=0;j<10000;j++){
            Bstar.remove(v[target[j]].id);
        }
        end = std::chrono::high_resolution_clock::now();
        assert(Bstar.verify()==true);
        file << (long long)((end-start).count()/10000) << "," <<Bstar.merge_count << "\n";
    }    
}