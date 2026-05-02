#include "B.h"
#include <iostream>
#include <queue>
Node::Node(int order,bool leaf,Node* p){
    parent=p;
    is_leaf=leaf;
    keys.reserve(order);
    rids.reserve(order);
    child_ptrs.reserve(order+1);
}
B::B(int d) {
    order = d;
    split_count=0;
    merge_count=0;
    root=new Node(d,true,nullptr);
}
void B::split(Node* target) {
    while (target->keys.size() >= order) {
        split_count++;
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
    merge_count++;
    if(target!=root && target->keys.size()<(order-1)/2){
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
    }
    if(root->keys.size()==0&&!root->is_leaf) {
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
void B::underflow(Node* target){
    if(target==root) return;
    if(target->keys.size()<(order-1)/2){
        Node* temp=target->parent;
        bool parent_is_root=target->parent==root;
        if(rotate(target)) return;
        merge(target);
        if(!parent_is_root) underflow(temp);
    }
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
    
    std::pair<Node*,int> target=inter_search(key);
    
    Node* now=target.first;
    int idx=target.second;
    
    if(idx<now->keys.size() && now->keys[idx]==key){
        if(now->is_leaf){
            now->keys.erase(now->keys.begin() + idx);
            now->rids.erase(now->rids.begin() + idx);
            underflow(now);
            return;
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
            underflow(succ);
        }
        else{
            now->keys[idx]=pre->keys.back();
            now->rids[idx]=pre->rids.back();
            pre->keys.pop_back();
            pre->rids.pop_back();
            underflow(pre);
        } 
    }
    else{
        //std::cout<<"없는 key"<<key<<"\n";
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
std::vector<int> B::range(int min_key,int max_key){
    std::vector<int> result;
    if (root == nullptr) return result;
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* curr=q.front();
        q.pop();
        int idx=inNode_find(min_key,curr);
        int last=curr->child_ptrs.size();
        for(int i=idx;i<curr->rids.size();i++){
            if(curr->keys[i]>max_key){
                last=i+1;
                break;
            } 
            result.push_back(curr->rids[i]);
            if(curr->keys[i]==max_key){
                last=i+1;
                break;
            } 
            
        }
        if(!curr->is_leaf){
            if(curr->keys.size()>idx&&curr->keys[idx]==min_key)idx++;
            for(;idx<last;idx++)
                q.push(curr->child_ptrs[idx]);
        }
    }
    return result;
}
void B::level_order() {
    if (root == nullptr) return;
    std::queue<Node*> q;
    total_nodes=0;
    total_elements=0;
    q.push(root);
    while (!q.empty()) {
        int level = q.size();
        while (level--) {
            Node* curr = q.front();
            q.pop();
            total_nodes++;
            total_elements+=curr->keys.size();
            /*
            std::cout << "[ ";
            for (int key : curr->keys) std::cout << key << " ";
            std::cout << "] ";*/
            if (!curr->is_leaf) 
                for (Node* child : curr->child_ptrs) q.push(child);
        }
        //std::cout << "\n"; 
    }
}
bool B::verify(){
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
        for (int i = 0; i < tnode->keys.size(); i++) {
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
            if (tnode->child_ptrs.size() != tnode->keys.size() + 1) {
                std::cout << "childs must keys+1\n";
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
                int next_min = (i==0) ? curr.min : tnode->keys[i-1] + 1;
                int next_max = (i==tnode->keys.size()) ? curr.max : tnode->keys[i]-1;
                q.push({child, next_min, next_max, curr.depth+1});
            }
        }
    }
    return true;
}
void B::delete_all(Node* node) {
    if (node == nullptr) return;
    if (!node->is_leaf) for (Node* child : node->child_ptrs) delete_all(child);
    delete node; 
}
B::~B(){delete_all(root);}
