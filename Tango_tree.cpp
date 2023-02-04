#include<bits/stdc++.h>
#include<chrono>
using namespace std;
using namespace std::chrono;


class timer{
    public:
        void start(void){ 
            flag_one = high_resolution_clock::now(); return;
        }
        void stop(void){ 
            flag_two = high_resolution_clock::now(); return;
        }
        void timeElapsed(void){ 
            auto duration = duration_cast<nanoseconds>(flag_two - flag_one);
            cout<<setprecision(2)<<fixed<<duration.count()/1000;
        }

    private:
        std::chrono::_V2::system_clock::time_point flag_one,flag_two;
};



class TangoTree{
    public:        
        TangoTree(const int size);        
        ~TangoTree(void);

        bool query(size_t key); 
        void display(void){ 
            explore(root);
            return;
        }
    
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
        void explore(Node *cur);
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

void tester(string &type, int size){
    timer clk; 
    clk.start(); 
    TangoTree tre(size); 
    clk.stop();
    cout<<"Access type: "<<type<<"\t\tSize: "<<size<<endl<<"\t\tBuild Time: ";
    clk.timeElapsed();
    cout<<"ns"<<endl<<"\t\t";
    
    if(type == "Sequential"){
        clk.start();
        for(int i=0;i<size;i++){
            tre.query(i);
        }
        clk.stop();
        cout<<"Query Time: ";
        clk.timeElapsed();
        cout<<"ns"<<endl;
        //tre.display();
    }
    
    if(type == "Reverse"){
        clk.start();
        for(int i=size-1;i>=0;i--){
            tre.query(i);
        }
        clk.stop();
        cout<<"Query Time: ";
        clk.timeElapsed();
        cout<<"ns"<<endl;
    }
    
    if(type == "Random"){
        vector<int> sequence;
        for(int i=0;i<size;i++){
            sequence.push_back(i);
        }
        random_shuffle(sequence.begin(),sequence.end()); 
        clk.start();
        for(int i=0;i<size;i++){ 
            tre.query(sequence[i]);
        }
        clk.stop();
        cout<<"Query Time: ";
        clk.timeElapsed();
        cout<<"ns"<<endl;
    }
    if(type == "One"){
        clk.start();
        int tmp = size;
        
        for(int i=0;i<size;i++){ 
            tre.query(tmp);
        }
        clk.stop();
        cout<<"Query Time: ";
        clk.timeElapsed();
        cout<<"ns"<<endl;
    }
    return;
}

void TangoTree::explore(TangoTree::Node *cur){
    
    if(!cur) return;
    
    cout<<"Parent of "<<cur->key<<" = ";
    
    if(cur->parent){
        cout<<cur->parent->key;
    }
    
    else{
        cout<<"NULL";
    }
    
    if(cur->isRoot) cout<<", "<<cur->key<<" is a Root";
    cout<<endl;
    
    explore(cur->left);
    
    explore(cur->right);
    return;
}

int main(){
    cout<<"Tango Tree:"<<endl;
    
    
    string access = "Sequential"; 
    tester(access,500);
    tester(access,50000);
    tester(access,5000000);
    access = "Reverse"; 
    tester(access,500);
    tester(access,50000);
    tester(access,5000000);
    access = "Random"; 
    tester(access,500);
    tester(access,50000);
    tester(access,5000000);
    access = "One"; 
    tester(access,500);
    tester(access,50000);
    tester(access,5000000);
    return 0;
}
