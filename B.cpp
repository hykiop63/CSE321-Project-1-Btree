#include "B.h"
#include <iostream>

Node::Node(int order,bool leaf,Node* p){
    parent=p;
    is_leaf=leaf;
    keys.reserve(order);
    rids.reserve(order);
    child_ptrs.reserve(order+1);
}
B::B(int d) {
    order = d;
    root=new Node(d,true,nullptr);
}
void B::split(Node* target) {
    while (target->keys.size() >= order) {
        int mid = target->keys.size() / 2;
        int upkey = target->keys[mid];
        int uprid = target->rids[mid];
        Node* temp = new Node(order, target->is_leaf, target->parent);
        temp->keys.assign(target->keys.begin() + mid + 1, target->keys.end());
        temp->rids.assign(target->rids.begin() + mid + 1, target->rids.end());
        target->keys.erase(target->keys.begin() + mid, target->keys.end());
        target->rids.erase(target->rids.begin() + mid, target->rids.end());

        if (!target->is_leaf) {
            temp->child_ptrs.assign(target->child_ptrs.begin() + mid + 1, target->child_ptrs.end());
            target->child_ptrs.erase(target->child_ptrs.begin() + mid + 1, target->child_ptrs.end());
            
            for (Node* child : temp->child_ptrs)
                child->parent = temp;
        }

        if (target->parent == nullptr) {
            Node* new_root = new Node(order, false, nullptr);
            
            new_root->keys.push_back(upkey);
            new_root->rids.push_back(uprid);
            new_root->child_ptrs.push_back(target);
            new_root->child_ptrs.push_back(temp);
            target->parent = new_root;
            temp->parent = new_root;
            root = new_root;
            break;
        } 
        else {
            Node* parent = target->parent;
            int idx = 0;
            while (idx < parent->keys.size() && parent->keys[idx] < upkey)
                idx++;
            parent->keys.insert(parent->keys.begin() + idx, upkey);
            parent->rids.insert(parent->rids.begin() + idx, uprid);
            parent->child_ptrs.insert(parent->child_ptrs.begin() + idx + 1, temp);

            target = parent;
        }
    }
}
void B::merge(Node* target){
    while(target!=root && target->keys.size()<(order-1)/2){
        Node*p=target->parent;
        if(p->child_ptrs[0]==target){
            target=p->child_ptrs[1];
        }
        int i=1;
        while(target!=p->child_ptrs[i]) i++;
        Node* l_s=p->child_ptrs[i-1];
        l_s->keys.push_back(p->keys[i-1]);
        l_s->rids.push_back(p->rids[i-1]);
        p->keys.erase(p->keys.begin()+i-1);
        p->rids.erase(p->rids.begin()+i-1);
        p->child_ptrs.erase(p->child_ptrs.begin()+i);
        l_s->keys.insert(l_s->keys.end(),target->keys.begin(),target->keys.end());
        l_s->rids.insert(l_s->rids.end(),target->rids.begin(),target->rids.end());
        l_s->child_ptrs.insert(l_s->child_ptrs.end(),target->child_ptrs.begin(),target->child_ptrs.end());
        for(auto q : target->child_ptrs){
            q->parent=l_s;
        }
        delete target;
        target=p;
    }
    if (root->keys.size()==0&&!root->is_leaf) {
        Node* old_root =root;
        root =root->child_ptrs[0];
        root->parent =nullptr;
        delete old_root;
    }
    return;
}
bool B::rotate(Node* target){
    if(target->parent==nullptr) return false;
    Node* p=target->parent;
    Node* r_s=p->child_ptrs[0];
    Node* l_s=p->child_ptrs[0];
    int i;
    if(p->child_ptrs[0]==target){//r_s만 존재
        r_s=p->child_ptrs[1];
        l_s=nullptr;
        i=0;
    }
    else if(p->child_ptrs.back()==target){//l_s만 존재
        r_s=nullptr;
        l_s=p->child_ptrs[p->child_ptrs.size()-2];
        i=p->child_ptrs.size()-1;
    }
    else{
        for(i=1;i<p->child_ptrs.size();i++){
            if(p->child_ptrs[i]==target){
                r_s=p->child_ptrs[i+1];
                l_s=p->child_ptrs[i-1];
                l_s=l_s->keys.size() < r_s->keys.size()?
                    nullptr : l_s;//l_s가 null인경우에만 r_s에서 가져옴
                r_s=l_s==nullptr?
                    r_s : nullptr;
                break;
            }
        }
    }
    if(l_s!=nullptr && l_s->keys.size()>(order-1)/2){// i means target==p->child_ptrs[i]
        target->keys.insert(target->keys.begin(),p->keys[i-1]);
        p->keys[i-1]=l_s->keys.back();
        l_s->keys.pop_back();
        target->rids.insert(target->rids.begin(),p->rids[i-1]);
        p->rids[i-1]=l_s->rids.back();
        l_s->rids.pop_back();
        if(!target->is_leaf){
            target->child_ptrs.insert(target->child_ptrs.begin(),l_s->child_ptrs.back());
            l_s->child_ptrs.pop_back();
            target->child_ptrs.front()->parent=target;
        }
        return true;
    }
    else if(r_s!=nullptr && r_s->keys.size()>(order-1)/2){
        target->keys.push_back(p->keys[i]);
        p->keys[i]=r_s->keys.front();
        r_s->keys.erase(r_s->keys.begin());
        target->rids.push_back(p->rids[i]);
        p->rids[i]=r_s->rids.front();
        r_s->rids.erase(r_s->rids.begin());
        if(!target->is_leaf){
            target->child_ptrs.push_back(r_s->child_ptrs.front());
            r_s->child_ptrs.erase(r_s->child_ptrs.begin());
            target->child_ptrs.back()->parent=target;
        }
        return true;
    }
    return false;
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
std::pair<Node*,int> B::inter_search(int key) {
    std::cout << key << " 검색 로직 실행\n";
    Node* now_check=root;
    while(true){
        int idx=inNode_find(key,now_check);
        if(idx<now_check->keys.size() && now_check->keys[idx]==key)
            return {now_check,idx};
        if(now_check->is_leaf) return {now_check, idx};
        now_check = now_check->child_ptrs[idx];
    }
}
void B::insert(int key,int rid) {
    std::pair<Node*,int> target=inter_search(key);
    Node* now_leaf=target.first;
    int idx=target.second;
    if(!now_leaf->is_leaf){
        std::cout<<"이미 있는 key:"<<key<<"\n";
        return;
    }
    now_leaf->keys.insert(now_leaf->keys.begin()+idx,key);
    now_leaf->rids.insert(now_leaf->rids.begin()+idx,rid);
    split(now_leaf);
}
void B::remove(int key){
    std::cout << key << " 검색 로직 실행\n";
    std::pair<Node*,int> target=inter_search(key);
    Node* now=target.first;
    int idx=target.second;
    if(idx<now->keys.size() && now->keys[idx]==key){
        if(now->is_leaf){
            now->keys.erase(now->keys.begin() + idx);
            now->rids.erase(now->rids.begin() + idx);
        }
        Node* pre=now->child_ptrs[idx];
        Node* succ=now->child_ptrs[idx+1];
        while(!pre->is_leaf) pre=pre->child_ptrs.back();
        while(!succ->is_leaf) succ=succ->child_ptrs.front();
        if(pre->keys.size()<succ->keys.size()){
            now->keys[idx]=succ->keys.front();
            now->rids[idx]=succ->rids.front();
            succ->keys.erase(succ->keys.begin());
            succ->rids.erase(succ->rids.begin());
            if(!rotate(succ))
                merge(succ);
        }
        else{
            now->keys[idx]=pre->keys.front();
            now->rids[idx]=pre->rids.front();
            pre->keys.erase(pre->keys.begin());
            pre->rids.erase(pre->rids.begin());
            if(!rotate(pre))
                merge(pre);
        } 
    }
    else{
        std::cout<<"없는 key"<<key<<"\n";
        return;
    }
}
int B::search(int key){
    std::pair<Node*,int> result=inter_search(key);
    if(result.second<result.first->keys.size() && 
        result.first->keys[result.second]==key)
            return result.first->rids[result.second];
    return -1;
}