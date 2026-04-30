#include "B.h"
#include <iostream>
#include <queue>
BPlus::BPlus(int d) {
    order = d;
    root=new Node(d,true,nullptr);
    root->pre_leaf=nullptr;
    root->next_leaf=nullptr;
}
void BPlus::split(Node* target) {
    while (target->keys.size() >= order) {
        int mid = target->keys.size() / 2;
        int upkey = target->keys[mid];
        Node* temp = new Node(order, target->is_leaf, target->parent);
        temp->keys.assign(target->keys.begin() + mid + 1, target->keys.end());
        target->keys.erase(target->keys.begin() + mid, target->keys.end());
        
        temp->child_ptrs.assign(target->child_ptrs.begin() + mid + 1, target->child_ptrs.end());
        target->child_ptrs.erase(target->child_ptrs.begin() + mid + 1, target->child_ptrs.end());
        for (Node* child : temp->child_ptrs)
            child->parent = temp;

        if (target->parent == nullptr) {
            Node* new_root = new Node(order, false, nullptr);
            
            new_root->keys.push_back(upkey);
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
            parent->child_ptrs.insert(parent->child_ptrs.begin() + idx + 1, temp);
            target = parent;
        }
    }
}
void BPlus::split(Node* target,bool leaf) {
    int mid = target->keys.size() / 2;
    int upkey = target->keys[mid];
    Node* temp = new Node(order, target->is_leaf, target->parent);
    temp->keys.assign(target->keys.begin() + mid,target->keys.end());
    temp->rids.assign(target->rids.begin() + mid,target->rids.end());
    target->keys.erase(target->keys.begin() + mid, target->keys.end());
    target->rids.erase(target->rids.begin() + mid, target->rids.end());
    temp->next_leaf =target->next_leaf;
    if (temp->next_leaf != nullptr) temp->next_leaf->pre_leaf = temp;
    target->next_leaf =temp;
    temp->pre_leaf=target;
    temp->parent =target->parent;
    if (target->parent == nullptr) {
        Node* new_root = new Node(order, false, nullptr);
        
        new_root->keys.push_back(upkey);
        new_root->child_ptrs.push_back(target);
        new_root->child_ptrs.push_back(temp);
        target->parent = new_root;
        temp->parent = new_root;
        root = new_root;
    } 
    else {
        Node* parent = target->parent;
        int idx = 0;
        while (idx < parent->keys.size() && parent->keys[idx] < upkey)
            idx++;
        parent->keys.insert(parent->keys.begin() + idx, upkey);
        parent->child_ptrs.insert(parent->child_ptrs.begin() + idx + 1, temp);
        split(parent);
    }
}
void BPlus::merge(Node* target){
    if(target!=root && target->keys.size()<(order-1)/2){
        Node*p=target->parent;
        if(p->child_ptrs[0]==target){
            target=p->child_ptrs[1];
        }
        int i=1;
        while(target!=p->child_ptrs[i]) i++;
        Node* l_s=p->child_ptrs[i-1];
        l_s->keys.push_back(p->keys[i-1]);
        p->keys.erase(p->keys.begin()+i-1);
        p->child_ptrs.erase(p->child_ptrs.begin()+i);
        l_s->keys.insert(l_s->keys.end(),target->keys.begin(),target->keys.end());
        l_s->child_ptrs.insert(l_s->child_ptrs.end(),target->child_ptrs.begin(),target->child_ptrs.end());
        for(auto q : target->child_ptrs){
            q->parent=l_s;
        }
        delete target;
    }
    if(root->keys.size()==0&&!root->is_leaf) {
        Node* old_root =root;
        root =root->child_ptrs[0];
        root->parent =nullptr;
        delete old_root;
    }
    return;
}
void BPlus::merge(Node* target,bool leaf){
    if(target!=root && target->keys.size()<(order-1)/2){
        Node*p=target->parent;
        if(p->child_ptrs[0]==target){
            target=p->child_ptrs[1];
        }
        int i=1;
        while(target!=p->child_ptrs[i]) i++;
        Node* l_s=p->child_ptrs[i-1];

        p->keys.erase(p->keys.begin()+i-1);
        p->child_ptrs.erase(p->child_ptrs.begin()+i);
        l_s->keys.insert(l_s->keys.end(),target->keys.begin(),target->keys.end());
        l_s->rids.insert(l_s->rids.end(), target->rids.begin(), target->rids.end());
        
        l_s->next_leaf=target->next_leaf;
        if (target->next_leaf!=nullptr) 
            target->next_leaf->pre_leaf=l_s;
        delete target;
    }
    if(root->keys.size()==0&&!root->is_leaf) {
        Node* old_root =root;
        root =root->child_ptrs[0];
        root->parent =nullptr;
        delete old_root;
    }
    return;
}
bool BPlus::rotate(Node* target){
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
        target->child_ptrs.insert(target->child_ptrs.begin(),l_s->child_ptrs.back());
        l_s->child_ptrs.pop_back();
        target->child_ptrs.front()->parent=target;
        return true;
    }
    else if(r_s!=nullptr && r_s->keys.size()>(order-1)/2){
        target->keys.push_back(p->keys[i]);
        p->keys[i]=r_s->keys.front();
        r_s->keys.erase(r_s->keys.begin());
        target->child_ptrs.push_back(r_s->child_ptrs.front());
        r_s->child_ptrs.erase(r_s->child_ptrs.begin());
        target->child_ptrs.back()->parent=target;
        return true;
    }
    return false;
}
bool BPlus::rotate(Node* target,bool leaf){
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
        target->keys.insert(target->keys.begin(),l_s->keys.back());
        target->rids.insert(target->rids.begin(),l_s->rids.back());
        l_s->keys.pop_back();
        l_s->rids.pop_back();
        p->keys[i-1]=target->keys.front();
        return true;
    }
    else if(r_s!=nullptr && r_s->keys.size()>(order-1)/2){
        target->keys.push_back(r_s->keys.front());
        target->rids.push_back(r_s->rids.front());
        r_s->keys.erase(r_s->keys.begin());
        r_s->rids.erase(r_s->rids.begin());
        p->keys[i]=r_s->keys.front();
        return true;
    }
    return false;
}
void BPlus::underflow(Node* target){
    if(target==root) return;
    if(target->keys.size()<(order-1)/2){
        Node* temp=target->parent;
        bool parent_is_root=target->parent==root;
        if(target->is_leaf){
            if(rotate(target,true)) return;
            merge(target,true);
        }
        else{
            if(rotate(target)) return;
            merge(target);
        }
        if(!parent_is_root) underflow(temp);
    }
}
int BPlus::inNode_find(int key,Node* curr_n){
    int left=0;
    int right=curr_n->keys.size()-1;
    int mid=(right+left)/2;
    while(left<=right){
        if(key==curr_n->keys[mid]) return mid + (curr_n->is_leaf ? 0:1);//노드내에 존재
        else if(key<curr_n->keys[mid]) right=mid-1;
        else left=mid+1;
        mid=(right+left)/2;
    }
    return left;//자식으로 이동해야함
}
std::pair<Node*,int> BPlus::inter_search(int key) {
    Node* now_check=root;
    while(!now_check->is_leaf){
        int idx=inNode_find(key,now_check);
        now_check = now_check->child_ptrs[idx];
    }
    return {now_check,inNode_find(key,now_check)};
}
void BPlus::insert(int key,int rid) {
    std::pair<Node*,int> target=inter_search(key);
    Node* now_leaf=target.first;
    int idx=target.second;
    if(!now_leaf->is_leaf){
        std::cout<<"삽입단계 고장남:"<<key<<"\n";
        return;
    }
    if(now_leaf->keys[idx]==key){
        std::cout<<"이미 있는 key:"<<key<<"\n";
        return;
    }
    now_leaf->keys.insert(now_leaf->keys.begin()+idx,key);
    now_leaf->rids.insert(now_leaf->rids.begin()+idx,rid);
    if(now_leaf->keys.size()>=order)
        split(now_leaf,true);
}
void BPlus::remove(int key){
    
    std::pair<Node*,int> target=inter_search(key);
    
    Node* now=target.first;
    int idx=target.second;
    
    if(idx<now->keys.size() && now->keys[idx]==key){
        now->keys.erase(now->keys.begin() + idx);
        now->rids.erase(now->rids.begin() + idx);
        underflow(now);
    }
    else
        std::cout<<"없는 key"<<key<<"\n";
}
int BPlus::search(int key){
    std::pair<Node*,int> result=inter_search(key);
    if(result.second<result.first->keys.size() && 
        result.first->keys[result.second]==key)
            return result.first->rids[result.second];
    return -1;
}
void BPlus::level_order() {
    if (root == nullptr) return;
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        int level = q.size();
        while (level--) {
            Node* curr = q.front();
            q.pop();
            std::cout << "[ ";
            for (int key : curr->keys) std::cout << key << " ";
            std::cout << "] ";
            if (!curr->is_leaf) 
                for (Node* child : curr->child_ptrs) q.push(child);
        }
        std::cout << "\n"; 
    }
}
bool BPlus::verify(){
    if(root==nullptr){
        std :: cout<<"null tree\n";
        return true;
    }
    struct Verify_Node {
        Node* node;
        int min;
        int max;
        int depth;
    };
    std::queue<Verify_Node> q;
    q.push({root, 0,1000000001, 0});//key is student number it 202XXXXXX integer.
    int leaf_depth = -1;
    int total_leaf = 0;
    while (!q.empty()) {
        Verify_Node curr = q.front();
        q.pop();
        Node* tnode = curr.node;
        // number of key
        int min_keys = tnode==root ? (int)!tnode->is_leaf : (order-1)/2;
        int max_keys = order-1;
        if (tnode->keys.size() < min_keys ) {
            std::cout << "key need more much\n";
            return false;
        }
        else if(tnode->keys.size() > max_keys){
            std::cout << "key need more less\n";
            return false;
        }
        // BST
        for (int i=0;i<tnode->keys.size();i++) {
            if (tnode->keys[i] < curr.min ) {
                std::cout << "key need more big\n";
                return false;
            }
            else if(tnode->keys[i] > curr.max){
                std::cout<<tnode->keys[i]<<" "<<curr.max<<"\n";
                std::cout << "key need more small\n";
                return false;
            }
            if (i > 0 && tnode->keys[i - 1] >= tnode->keys[i]) {
                std::cout << "innode keys not sorted\n";
                return false;
            }
        }
        //leaf node
        if (tnode->is_leaf) {
            total_leaf++;
            if (leaf_depth == -1) leaf_depth = curr.depth; 
            else if (leaf_depth != curr.depth) {
                std::cout << "all leaf must same depth\n";
                return false;
            }
            if (tnode->child_ptrs.size() != 0) {
                std::cout << "leaf has not child\n";
                return false;
            }
        } 
        //internal node
        else {
            if (tnode->child_ptrs.size()!=tnode->keys.size()+1) {
                std::cout<<"childs must keys+1\n";
                return false;
            }
            for (int i=0;i<tnode->child_ptrs.size();i++) {
                Node* child = tnode->child_ptrs[i];
                if (child == nullptr) {
                    std::cout << "child is null\n";
                    return false;
                }
                if (child->parent != tnode) {
                    std::cout << "child not match parent\n";
                    return false;
                }
                int next_min = (i==0) ? curr.min : tnode->keys[i-1];
                int next_max = (i==tnode->keys.size()) ? curr.max : tnode->keys[i]-1;
                q.push({child,next_min,next_max,curr.depth+1});
            }
        }
    }
    Node* leaf_start=root;
    
    while(!leaf_start->is_leaf) leaf_start=leaf_start->child_ptrs[0];
    if(leaf_start->pre_leaf!=nullptr){
        std::cout<<"left most Node must not have pointer\n";
        return false;
    }
    int check_leaf=1;   
    while(leaf_start->next_leaf!=nullptr){
        if (leaf_start->keys.back()>=leaf_start->next_leaf->keys.front()) {
                std::cout << "1not sorted\n";
                return false;
            }
        if (leaf_start->next_leaf->pre_leaf!=leaf_start) {
            std::cout << "pre_leaf pointer mismatch\n";
            return false;
        }
        leaf_start = leaf_start->next_leaf;
        check_leaf++; 
    }
    if(check_leaf!=total_leaf){
        std::cout<<"some Node not linked\n";
        return false;
    }
    while(leaf_start->pre_leaf!=nullptr){
        if (leaf_start->keys.front()<=leaf_start->pre_leaf->keys.back()) {
                std::cout << "2not sorted\n";
                return false;
            }
        if (leaf_start->pre_leaf->next_leaf!=leaf_start) {
            std::cout << "pre_leaf pointer mismatch\n";
            return false;
        }
        leaf_start = leaf_start->pre_leaf;
    }
    return true;
}