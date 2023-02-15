#include<bits/stdc++.h>
#include<chrono>
#include "Tango_tree.cpp"
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
        auto timeElapsed(void){ 
            auto duration = duration_cast<nanoseconds>(flag_two - flag_one);
            return duration.count()/1000;
        }

    private:
        std::chrono::_V2::system_clock::time_point flag_one,flag_two;
};

void tester(string &type, int size, string output_path){
    ofstream cout(output_path);

    timer clk; 
    clk.start(); 
    TangoTree tre(size); 
    clk.stop();
    cout<<"Access type: "<<type<<"\t\tSize: "<<size<<endl<<"\t\tBuild Time: ";
    cout<<setprecision(2)<<fixed<<clk.timeElapsed();
    cout<<"ns"<<endl<<"\t\t";
    
    if(type == "Sequential"){
        clk.start();
        for(int i=0;i<size;i++){
            tre.query(i);
        }
        clk.stop();
        cout<<"Query Time: ";
        cout<<setprecision(2)<<fixed<<clk.timeElapsed();
        cout<<"ns"<<endl;
    }
    
    if(type == "Reverse"){
        clk.start();
        for(int i=size-1;i>=0;i--){
            tre.query(i);
        }
        clk.stop();
        cout<<"Query Time: ";
        cout<<setprecision(2)<<fixed<<clk.timeElapsed();
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
        cout<<setprecision(2)<<fixed<<clk.timeElapsed();
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
        cout<<setprecision(2)<<fixed<<clk.timeElapsed();
        cout<<"ns"<<endl;
    }
    return;
}

int main(){
    for (int i = 1; i <= 12; i++) {
        string input_path = "tests/" + to_string(i) + "/input.txt";
        string output_path = "tests/" + to_string(i) + "/output.txt";
        cout << input_path;
        ifstream cin(input_path);
        
        string access = "";
        int size = 0;
        cin >> access >> size;
        
        tester(access, size, output_path);
    } 

}