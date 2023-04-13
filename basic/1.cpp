#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <queue>
#include <math.h>
#include <chrono>


using namespace std;

struct HeapNode {
    string sentence;                                    //element present at the heap node 
    int index;                                       //index of run from where the element i.e. the string is taken 
    HeapNode(string a, int b): sentence(a), index(b) {}
    bool operator<(const HeapNode& rhs) const {       //In the priority queue, there is a predefined boolean function "operator<()" , we overload it
        return (sentence.compare(rhs.sentence) > 0);
    }
};


int generate_runs(string input_name, long long TOTAL_MEM, int key_count) {
    ifstream input; 
    input.open(input_name.c_str());

    if (!input.good()) {
        cout << "File input is not found!" << endl << "Exit program!" << endl;
        exit(-1);
    }

    int local_key_count = 0;
    // int input_size; 
    // input.seekg(0, input.end);
    // input_size = input.tellg();
    // input.seekg(0, input.beg);
    // cout << "-------------------------------------------------------\n";
    // cout << "The size of the file chosen is (in bytes): " << input_size << endl;

    int run_count = 0;
    int total_mem_so_far = 0;

    ofstream output;
    vector<string> data; data.clear();

    cout << "File " << input_name << " is being read!" << endl;
    cout << "-------------------------------------------------------\n\n\n";

    cout << "-------------------------------------------------------\n";
    while ( (!input.eof()) && (local_key_count < key_count) ) {
        string sentence;
        getline(input, sentence);   //jab tak newline encounter na ho , get the string input in sentence 
        local_key_count ++;

        if (total_mem_so_far + sentence.size() < TOTAL_MEM) {
            total_mem_so_far += sentence.size() + 1; //coz strings are null terminated ig
            data.push_back(sentence);
        } else {        //we have obtained a run 
            sort(data.begin(), data.end());

            run_count++;
            stringstream ss;
            ss << "temp.0." << run_count;
            cout << "Writing " << ss.str() << endl;
            output.open(ss.str());

            int data_size = data.size();
            for (int i = 0; i < data_size-1; i++) {
                output << data[i];
                output << endl;
            }

            if (data_size > 0) {
                output << data[data_size-1];
            }
            //now we have written run i on file run_i.txt 
            output.close();
            data.clear();
            total_mem_so_far = sentence.size();
            data.push_back(sentence);
        }
    }


    if (data.size() > 0) {          //when input file has been finished reading but data vector ko abhi tk kisi run_i.txt file mein unload nhi kiya
        sort(data.begin(), data.end());

        run_count++;
        stringstream ss;
        ss << "temp.0." << run_count;
        cout << "Writing " << ss.str() << endl;
        output.open(ss.str());

        int data_size = data.size();
        for (int i = 0; i < data_size-1; i++) {
            output << data[i];
            output << endl;
        }
        input.close();
        output << data[data_size-1];
        output.close();
    }

    //now we have all the runs created on disk in txt files 
    
    cout << "Read input is done!" << endl;
//    cout << "Entire process so far took a total of: " << float(clock() - begin_time) / CLOCKS_PER_SEC * 1000 << " msec." << endl;
    cout << "-------------------------------------------------------\n\n\n";

    return run_count;
}


void merge(int start_idx , int end_idx , int stage , int stage_count ){

    int runs_count = end_idx - start_idx + 1;

    ifstream input[runs_count];
    for (int i = 0; i < runs_count; i++) {
        stringstream ss;
        ss << "temp." << stage  << "." << start_idx + i + 1;
        input[i].open(ss.str());
    }

    priority_queue<HeapNode, vector<HeapNode> > heap;   //std::vector is our container becoz priority_queue is container adapter

    ofstream output;
    stringstream ss;
    ss << "temp." << stage + 1 << "." << stage_count;
    output.open(ss.str());

    for (int i = 0; i < runs_count; i++) {
        string sentence;
        if (!input[i].eof()) {
            getline(input[i], sentence);
            heap.push(HeapNode(sentence, i));
        }
    }

    cout << "-------------------------------------------------------\n";
    cout << endl << "Merging from run" << start_idx << " to run" << end_idx << endl;

    int outcount = 0;
    while (!heap.empty()) {
        string sentence = heap.top().sentence; 
        int index = heap.top().index;
        heap.pop();


        if(outcount == 0) output << sentence;
        else output << endl << sentence;

        outcount ++;
        
        if (!input[index].eof()) {
            getline(input[index], sentence);
            heap.push(HeapNode(sentence, index));
        }
    }

    cout << "Merge done!" << endl << endl;
    cout << "-------------------------------------------------------\n\n\n";

    for (int i = 0; i < runs_count; i++) {
        input[i].close();
    }

    output.close();

}


int merge_runs(int num_runs , int max_fanout , string outfile , int stage ){
    //starting mein i have files = temp.stage.0 to temp.stage.(num_runs-1)
    int start = 0;
    int withtin_stage_count = 1;
    int ans = 0;
    while(start < num_runs){
        if( (start + max_fanout - 1 ) < num_runs) {
            merge(start , start + max_fanout -1 , stage , withtin_stage_count);
            ans ++;
            start += max_fanout;
            withtin_stage_count ++ ;
        }
        else{
            merge(start , num_runs - 1 , stage , withtin_stage_count );
            ans ++;
            start = num_runs ;
            withtin_stage_count ++ ;
        }
    }

    if ( withtin_stage_count == 2 ) return ans;
    else return (ans + merge_runs(withtin_stage_count -1 , max_fanout , outfile , stage + 1)); 

}




int external_merge_sort_withstop(const char* input, const char* output, const long keycount , const int k, const int num_merges){

    int runs_count = generate_runs(input, 50000000*2 , keycount); //5*1e7 is 50 mb  
    int anss = merge_runs(runs_count , k , output , 0);


    return anss;
}
