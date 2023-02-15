#include<bits/stdc++.h>
#include<chrono>
using namespace std;
using namespace std::chrono;


class TangoTree{
    public:        
        TangoTree(const int size);        
        ~TangoTree(void);

        bool query(size_t key); 
        
    
    private:
        struct Node{             
            size_t key, depth, minDepth;
            bool isRoot;
            Node *left, *right, *parent;
            Node(size_t key, size_t depth = 0, size_t minDepth = 0, bool isRoot = 0){
                this->key = key;
                left = right = parent = 0;
                this->isRoot = isRoot;
                this->depth = depth;
                this->minDepth = minDepth;
            }
        };

        Node *root; 
        Node *treeFor(size_t low, size_t high, size_t depth, bool isRoot); 
        void rotate(Node *cur); 
        void splay(Node *cur, Node *top);
        void switchPath(Node *cur);
        void expose(Node *cur);
        Node *refParent(Node *cur, int num); 
};

TangoTree::Node *TangoTree::treeFor(size_t low, size_t high, size_t depth, bool isRoot){
    if(low == high)  return nullptr;
    size_t mid = low + (high - low)/2;

    Node *tmp = new Node {mid, depth, depth, isRoot};

    tmp->left = treeFor(low, mid, depth + 1, false);

    tmp->right = treeFor(mid + 1, high, depth + 1, true);  
    if(tmp->left) tmp->left->parent = tmp;
    if(tmp->right) tmp->right->parent = tmp;
    return tmp;
}

TangoTree::TangoTree(const int size){
    root = treeFor(0, size, 0,  true);
    return;
}

TangoTree::~TangoTree(void){
    while(root != nullptr){
        if(root->left == nullptr){
            Node *next = root->right;
            delete root; 
            root = next;
            
        }

        else{
            Node *child = root->left;
            root->left = child->right;
            child->right = root;
            root = child;
        }
    }
    return;
}

void TangoTree::rotate(Node *cur){
    Node *tmp = cur->parent;

    if(tmp->isRoot){
        tmp->isRoot = false;
        cur->isRoot = true;
    }
    if(root == tmp) root = cur;
    if(tmp->parent){
        int cnt =  (tmp->parent->right == tmp);

        if(cnt == 0){
            tmp->parent->left = cur;
        }
        else{
            tmp->parent->right = cur;
        }
    }

    cur->parent = tmp->parent;
    int cnt = (tmp->right == cur);
    if(cnt == 0){
        
        tmp->left = cur->right;
        if(cur->right){
            cur->right->parent = tmp;
        }
        cur->right = tmp;
        tmp->parent = cur;
    }
    
    else{
        
        tmp->right = cur->left;
        if(cur->left){
            cur->left->parent = tmp;
        }
        cur->left = tmp;
        tmp->parent = cur;
    }
    
    cur->minDepth = tmp->minDepth;
    tmp->minDepth = tmp->depth;
    if(tmp->left){ 
        tmp->minDepth = min(tmp->minDepth, tmp->left->minDepth);
    }
    if(tmp->right){
        tmp->minDepth = min(tmp->minDepth, tmp->right->minDepth);
    }
    return;
}

void TangoTree::splay(Node *cur, Node *top = 0){
    
    while(!(cur->isRoot || cur->parent == top)){
        Node *tmp = cur->parent;
        
        if(!(tmp->isRoot || tmp->parent == top)){
            Node *tmp2 = tmp->parent;
            if((tmp2->left == tmp && tmp->left == cur) || (tmp2->right == tmp && tmp->right == cur)){
                rotate(tmp);
            }
            else{
                rotate(cur);
            }
        }
        
        rotate(cur);
    }
    return;
}

TangoTree::Node *TangoTree::refParent(TangoTree::Node *cur, int cnt){
    
    Node *tmp = NULL;
    if(cnt == 0){
        tmp = cur->left;
    }
    else{
        tmp = cur->right;
    }
    
    while(1){
        if(cnt == 0){
            
            if(tmp->right && tmp->right->minDepth < cur->depth){
                tmp = tmp->right;
            }
            
            else if(tmp->left && tmp->left->minDepth < cur->depth){
                tmp = tmp->left;
            }
            
            else{
                break;
            }
        }
        else{
            
            if(tmp->left && tmp->left->minDepth < cur->depth){
                tmp = tmp->left;
            }
            
            else if(tmp->right && tmp->right->minDepth < cur->depth){
                tmp = tmp->right;
            }
            
            else{
                break;
            }
        }
    }
    return tmp;
}

void TangoTree::switchPath(Node *cur){
    
    if(cur->left){
        
        if(cur->left->minDepth > cur->depth){
            cur->left->isRoot = !(cur->left->isRoot);
        }
        else{
            Node *tmp = refParent(cur, 0);
            splay(tmp, cur);
            if(tmp->right){
                tmp->right->isRoot = !(tmp->right->isRoot);
            }
        }
    }
    
    if(cur->right){
        
        if(cur->right->minDepth > cur->depth){
            cur->right->isRoot = !(cur->right->isRoot);
        }
        else{
            Node *tmp2 = refParent(cur, 1);
            splay(tmp2, cur);
            if(tmp2->left){
                tmp2->left->isRoot = !(tmp2->left->isRoot);
            }
        }
    }
}

void TangoTree::expose(Node *cur){
    
    Node *tmp = cur;
    
    while(tmp->parent){
        Node *tmp2 = tmp->parent;
        
        if(tmp->isRoot){
            splay(tmp2);
            switchPath(tmp2);
        }
        tmp = tmp2;
    }
    splay(cur);
    return;
}

bool TangoTree::query(size_t key){ 
    
    Node *curr = root;
    Node *prev = root;
    
    while(curr && curr->key != key){
        prev = curr;
        if(key < curr->key){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    
    if(!curr){
        expose(prev);
        return false;
    }
    
    expose(curr);
    return true;
}