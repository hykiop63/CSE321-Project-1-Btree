#include "B.h"
#include <iostream>
#include <queue>
BStar::BStar(int d) {
    order = d;
    split_count=0;
    merge_count=0;
    root=new Node(d,true,nullptr);
}
void BStar::split_root(Node* target){
    int k = (2*order-1)/3;
    if(target->keys.size() >=3*k+2) {
        split_count++;
        int upkey1 = target->keys[k];
        int uprid1 = target->rids[k];
        int upkey2 = target->keys[2*k+1];
        int uprid2 = target->rids[2*k+1];
        Node* m_n=new Node(order,target->is_leaf,nullptr);
        Node* r_n=new Node(order,target->is_leaf,nullptr);

        m_n->keys.assign(target->keys.begin()+k+1,target->keys.begin()+2*k+1);
        m_n->rids.assign(target->rids.begin()+k+1,target->rids.begin()+2*k+1);
        r_n->keys.assign(target->keys.begin()+2*k+2,target->keys.end());
        r_n->rids.assign(target->rids.begin()+2*k+2,target->rids.end());
        target->keys.erase(target->keys.begin()+k,target->keys.end());
        target->rids.erase(target->rids.begin()+k,target->rids.end());
        if (!target->is_leaf) {
            m_n->child_ptrs.assign(target->child_ptrs.begin()+k+1,target->child_ptrs.begin()+2*k+2);
            r_n->child_ptrs.assign(target->child_ptrs.begin()+2*k+2,target->child_ptrs.end());
            target->child_ptrs.erase(target->child_ptrs.begin()+k+1,target->child_ptrs.end());
            
            for (Node* child : m_n->child_ptrs) child->parent = m_n;
            for (Node* child : r_n->child_ptrs) child->parent = r_n;
        }
        Node* new_root = new Node(order, false, nullptr);
            
        new_root->keys.push_back(upkey1);
        new_root->keys.push_back(upkey2);
        new_root->rids.push_back(uprid1);
        new_root->rids.push_back(uprid2);

        new_root->child_ptrs.push_back(target);
        new_root->child_ptrs.push_back(m_n);
        new_root->child_ptrs.push_back(r_n);
        target->parent = new_root;
        m_n->parent = new_root;
        r_n->parent = new_root;
        root = new_root;
    }
}
void BStar::split(Node* target) {
    
    if (target->keys.size() >= order) {
        split_count++;
        Node*p=target->parent;
        Node*temp=new Node(order,target->is_leaf,p);
        if(p->child_ptrs[0]==target){
            target=p->child_ptrs[1];
        }
        int i=1;
        while(target!=p->child_ptrs[i]) i++;
        Node* l_s=p->child_ptrs[i-1];
        temp->keys.insert(temp->keys.end(),l_s->keys.begin(),l_s->keys.end());
        temp->rids.insert(temp->rids.end(),l_s->rids.begin(),l_s->rids.end());
        
        temp->keys.push_back(p->keys[i-1]);
        temp->rids.push_back(p->rids[i-1]);
        temp->keys.insert(temp->keys.end(),target->keys.begin(),target->keys.end());
        temp->rids.insert(temp->rids.end(),target->rids.begin(),target->rids.end());
        
        l_s->keys.clear();
        l_s->rids.clear();
        
        target->keys.clear();
        target->rids.clear();
        
        l_s->keys.insert(l_s->keys.end(),temp->keys.begin(),temp->keys.begin()+(2*order-1)/3);
        l_s->rids.insert(l_s->rids.end(),temp->rids.begin(),temp->rids.begin()+(2*order-1)/3);
        
        p->keys[i-1]=temp->keys[(2*order-1)/3];
        p->rids[i-1]=temp->rids[(2*order-1)/3];
        target->keys.insert(target->keys.end(),temp->keys.begin()+1+(2*order-1)/3,temp->keys.begin()+1+2*(2*order-1)/3);
        target->rids.insert(target->rids.end(),temp->rids.begin()+1+(2*order-1)/3,temp->rids.begin()+1+2*(2*order-1)/3);
        
        p->keys.insert(p->keys.begin()+i,temp->keys[1+2*(2*order-1)/3]);
        p->rids.insert(p->rids.begin()+i,temp->rids[1+2*(2*order-1)/3]);
        p->child_ptrs.insert(p->child_ptrs.begin()+i+1,temp);
        temp->keys.erase(temp->keys.begin(),temp->keys.begin()+2+2*(2*order-1)/3);
        temp->rids.erase(temp->rids.begin(),temp->rids.begin()+2+2*(2*order-1)/3);
        
        if (!temp->is_leaf) {
            temp->child_ptrs.insert(temp->child_ptrs.end(),l_s->child_ptrs.begin(),l_s->child_ptrs.end());
            temp->child_ptrs.insert(temp->child_ptrs.end(),target->child_ptrs.begin(),target->child_ptrs.end());
            l_s->child_ptrs.clear();
            target->child_ptrs.clear();
            l_s->child_ptrs.insert(l_s->child_ptrs.end(),temp->child_ptrs.begin(),temp->child_ptrs.begin()+1+(2*order-1)/3);
            target->child_ptrs.insert(target->child_ptrs.end(),temp->child_ptrs.begin()+1+(2*order-1)/3,temp->child_ptrs.begin()+2+2*(2*order-1)/3);
            temp->child_ptrs.erase(temp->child_ptrs.begin(),temp->child_ptrs.begin()+2+2*(2*order-1)/3);
            for (auto child : l_s->child_ptrs) child->parent=l_s;
            for (auto child : target->child_ptrs) child->parent=target;
            for (auto child : temp->child_ptrs) child->parent=temp;
        }
    }
}
void BStar::merge_3_to_3(Node* l_s,Node* target,Node* r_s,Node* p,int i){
    l_s->keys.push_back(p->keys[i-1]);
    l_s->rids.push_back(p->rids[i-1]);
    l_s->keys.insert(l_s->keys.end(),target->keys.begin(),target->keys.end());
    l_s->rids.insert(l_s->rids.end(),target->rids.begin(),target->rids.end());
    l_s->keys.push_back(p->keys[i]);
    l_s->rids.push_back(p->rids[i]);
    l_s->keys.insert(l_s->keys.end(),r_s->keys.begin(),r_s->keys.end());
    l_s->rids.insert(l_s->rids.end(),r_s->rids.begin(),r_s->rids.end());

    if(!target->is_leaf){
        l_s->child_ptrs.insert(l_s->child_ptrs.end(),target->child_ptrs.begin(),target->child_ptrs.end());
        l_s->child_ptrs.insert(l_s->child_ptrs.end(),r_s->child_ptrs.begin(),r_s->child_ptrs.end());
    }

    int total=l_s->keys.size();
    int part1=total/3;
    int part2=(total-part1-2)/2;

    target->keys.clear();
    target->rids.clear();
    r_s->keys.clear();
    r_s->rids.clear();

    r_s->keys.insert(r_s->keys.end(),l_s->keys.begin()+part1+part2+2,l_s->keys.end());
    r_s->rids.insert(r_s->rids.end(),l_s->rids.begin()+part1+part2+2,l_s->rids.end());

    target->keys.insert(target->keys.end(),l_s->keys.begin()+part1+1,l_s->keys.begin()+part1+part2+1);
    target->rids.insert(target->rids.end(),l_s->rids.begin()+part1+1,l_s->rids.begin()+part1+part2+1);

    p->keys[i-1]=l_s->keys[part1];
    p->rids[i-1]=l_s->rids[part1];
    p->keys[i]=l_s->keys[part1+part2+1];
    p->rids[i]=l_s->rids[part1+part2+1];

    l_s->keys.erase(l_s->keys.begin()+part1,l_s->keys.end());
    l_s->rids.erase(l_s->rids.begin()+part1,l_s->rids.end());

    if(!target->is_leaf){
        target->child_ptrs.clear();
        r_s->child_ptrs.clear();

        r_s->child_ptrs.insert(r_s->child_ptrs.end(),l_s->child_ptrs.begin()+part1+part2+2,l_s->child_ptrs.end());
        target->child_ptrs.insert(target->child_ptrs.end(),l_s->child_ptrs.begin()+part1+1,l_s->child_ptrs.begin()+part1+part2+2);
        l_s->child_ptrs.erase(l_s->child_ptrs.begin()+part1+1,l_s->child_ptrs.end());

        for(auto q:l_s->child_ptrs) q->parent=l_s;
        for(auto q:target->child_ptrs) q->parent=target;
        for(auto q:r_s->child_ptrs) q->parent=r_s;
    }
}
void BStar::merge_3_to_2(Node* l_s,Node* target,Node* r_s,Node* p,int i){
    l_s->keys.push_back(p->keys[i-1]);
    l_s->rids.push_back(p->rids[i-1]);
    l_s->keys.insert(l_s->keys.end(),target->keys.begin(),target->keys.end());
    l_s->rids.insert(l_s->rids.end(),target->rids.begin(),target->rids.end());
        
    l_s->keys.push_back(p->keys[i]);
    l_s->rids.push_back(p->rids[i]);
    l_s->keys.insert(l_s->keys.end(),r_s->keys.begin(),r_s->keys.end());
    l_s->rids.insert(l_s->rids.end(),r_s->rids.begin(),r_s->rids.end());
        
    p->keys.erase(p->keys.begin()+i-1);
    p->rids.erase(p->rids.begin()+i-1);
    p->child_ptrs.erase(p->child_ptrs.begin()+i);
    r_s->keys.clear();
    r_s->rids.clear();
        
    int mid=l_s->keys.size()/2;
    r_s->keys.insert(r_s->keys.begin(),l_s->keys.begin() + mid +1,l_s->keys.end());
    r_s->rids.insert(r_s->rids.begin(),l_s->rids.begin() + mid +1,l_s->rids.end());
       
    p->keys[i-1]=l_s->keys[mid];
    p->rids[i-1]=l_s->rids[mid];
    l_s->keys.erase(l_s->keys.begin() + mid,l_s->keys.end());
    l_s->rids.erase(l_s->rids.begin() + mid,l_s->rids.end());
        
    if(!target->is_leaf){
        l_s->child_ptrs.insert(l_s->child_ptrs.end(),target->child_ptrs.begin(),target->child_ptrs.end());
        l_s->child_ptrs.insert(l_s->child_ptrs.end(),r_s->child_ptrs.begin(),r_s->child_ptrs.end());
            
        r_s->child_ptrs.clear();
        r_s->child_ptrs.insert(r_s->child_ptrs.begin(),l_s->child_ptrs.begin() + mid +1,l_s->child_ptrs.end());
        l_s->child_ptrs.erase(l_s->child_ptrs.begin() + mid+1,l_s->child_ptrs.end());
        for(auto q : l_s->child_ptrs) q->parent=l_s;
        for(auto q : r_s->child_ptrs) q->parent=r_s;
    }
        
    delete target;
}
void BStar::merge_2_to_1(Node* l_s,Node* r_s,Node* p){
    l_s->keys.push_back(p->keys[0]);
    l_s->rids.push_back(p->rids[0]);
    
    l_s->keys.insert(l_s->keys.end(),r_s->keys.begin(),r_s->keys.end());
    l_s->rids.insert(l_s->rids.end(),r_s->rids.begin(),r_s->rids.end());
    
    if(!l_s->is_leaf){
        l_s->child_ptrs.insert(l_s->child_ptrs.end(),r_s->child_ptrs.begin(),r_s->child_ptrs.end());
        for(auto child:r_s->child_ptrs) child->parent=l_s;
    }
    
    root=l_s;
    root->parent=nullptr;
    
    delete r_s;
    delete p;
}
void BStar::merge(Node* target){

    if(target!=root && target->keys.size()<(2*order-1)/3){
        
        merge_count++;
        Node*p=target->parent;
        if(p==root&&p->child_ptrs.size()==2){
            merge_2_to_1(p->child_ptrs[0],p->child_ptrs[1],p);
            return;
        }
        if(p->child_ptrs[0]==target){
            target=p->child_ptrs[1];
        }
        else if(p->child_ptrs.back()==target){
            target=p->child_ptrs[p->child_ptrs.size()-2];
        }
        int i=1;
        while(target!=p->child_ptrs[i]) i++;
        Node* l_s=p->child_ptrs[i-1];
        Node* r_s=p->child_ptrs[i+1];
        if(target->keys.size()+l_s->keys.size()+r_s->keys.size()+1<=2*(order-1)){
            //std::cout<<target->keys.size()+l_s->keys.size()+r_s->keys.size()+1<<" merge pro\n";
            merge_3_to_2(l_s,target,r_s,p,i);
        }
            
        else
            merge_3_to_3(l_s,target,r_s,p,i);
    }
}
bool BStar::rotate(Node* target){
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
    if(l_s!=nullptr && l_s->keys.size()>(2*order-1)/3){// i means target==p->child_ptrs[i]
        while(l_s->keys.size()>target->keys.size()+1){
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
        }
        return true;
    }
    else if(r_s!=nullptr && r_s->keys.size()>(2*order-1)/3){
        while(r_s->keys.size()>target->keys.size()+1){
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
        }
        return true;
    }
    return false;
}
bool BStar::rotate_insert(Node* target){
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
                l_s=l_s->keys.size() > r_s->keys.size()?
                    nullptr : l_s;//l_s가 null인경우에만 r_s에서 가져옴
                r_s=l_s==nullptr?
                    r_s : nullptr;
                break;
            }
        }
    }
    if(l_s!=nullptr && l_s->keys.size()<order-1){// i means target==p->child_ptrs[i]
        while(target->keys.size()>l_s->keys.size()){
            l_s->keys.push_back(p->keys[i-1]);
            p->keys[i-1]=target->keys.front();
            target->keys.erase(target->keys.begin());
            l_s->rids.push_back(p->rids[i-1]);
            p->rids[i-1]=target->rids.front();
            target->rids.erase(target->rids.begin());
            if(!target->is_leaf){
                l_s->child_ptrs.push_back(target->child_ptrs.front());
                target->child_ptrs.erase(target->child_ptrs.begin());
                l_s->child_ptrs.back()->parent=l_s;
            }
        }
        return true;
    }
    else if(r_s!=nullptr && r_s->keys.size()<order-1){
        while(target->keys.size()>r_s->keys.size()){
            r_s->keys.insert(r_s->keys.begin(),p->keys[i]);
            p->keys[i]=target->keys.back();
            target->keys.pop_back();
            r_s->rids.insert(r_s->rids.begin(),p->rids[i]);
            p->rids[i]=target->rids.back();
            target->rids.pop_back();
            if(!target->is_leaf){
                r_s->child_ptrs.insert(r_s->child_ptrs.begin(),target->child_ptrs.back());
                target->child_ptrs.pop_back();
                r_s->child_ptrs.front()->parent=r_s;
            }
        }
        return true;
    }
    return false;
}
void BStar::underflow(Node* target){
    if(target==root) return;
    if(target->keys.size()<(2*order-1)/3){
        Node* temp=target->parent;
        bool parent_is_root=target->parent==root;
        if(rotate(target)) return;
        merge(target);
        if(!parent_is_root) underflow(temp);
    }
}
void BStar::overflow(Node* target){
    if(target==root){
        if (target->keys.size()>=3*((2*order-1)/3)+2){
            split_root(target);
        }
        return;
    }
    if(target->keys.size()>=order){
        if(rotate_insert(target)) return;
        Node* temp=target->parent;
        split(target);
        overflow(temp);
    }
}
int BStar::inNode_find(int key,Node* curr_n){
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
std::pair<Node*,int> BStar::inter_search(int key) {
    Node* now_check=root;
    while(true){
        int idx=inNode_find(key,now_check);
        if(idx<now_check->keys.size() && now_check->keys[idx]==key)
            return {now_check,idx};
        if(now_check->is_leaf) return {now_check, idx};
        now_check = now_check->child_ptrs[idx];
    }
}
void BStar::insert(int key,int rid) {
    std::pair<Node*,int> target=inter_search(key);
    Node* now_leaf=target.first;
    int idx=target.second;
    if(!now_leaf->is_leaf){
        std::cout<<"이미 있는 key:"<<key<<"\n";
        return;
    }
    now_leaf->keys.insert(now_leaf->keys.begin()+idx,key);
    now_leaf->rids.insert(now_leaf->rids.begin()+idx,rid);
    overflow(now_leaf);
}
void BStar::remove(int key){
    std::pair<Node*,int> target=inter_search(key);
    Node* now=target.first;
    int idx=target.second;
    if(now == nullptr){
        //std::cout<<"없는 key"<<key<<"\n";
        return;
    }
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
int BStar::search(int key){
    std::pair<Node*,int> result=inter_search(key);
    if(result.second<result.first->keys.size() && 
        result.first->keys[result.second]==key)
            return result.first->rids[result.second];
    return -1;
}
std::vector<int> BStar::range(int min_key,int max_key){
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
void BStar::level_order() {
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
bool BStar::verify(){
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
        int min_keys = tnode==root ? (int)!tnode->is_leaf : (2*order-2)/3;
        int max_keys = tnode==root ? 3*((2*order-1)/3)+2 : order-1;
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
                    std::cout << tnode->keys[0]<<"child not match parent\n";
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
void BStar::delete_all(Node* node) {
    if (node == nullptr) return;
    if (!node->is_leaf) for (Node* child : node->child_ptrs) delete_all(child);
    delete node; 
}
BStar::~BStar(){delete_all(root);}