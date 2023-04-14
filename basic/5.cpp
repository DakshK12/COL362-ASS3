// #include <iostream>
// #include <stdio.h>
// #include <string>
// #include <vector>
// #include <fstream>
// #include <sstream>
// #include <algorithm>
// #include <ctime>
// #include <queue>
// #include <math.h>
// #include <chrono>

#include <bits/stdc++.h>

#define BLOCK_SIZE 1 << 22
#define MEM_AVAIL 1 << 30
#define MAX_STRING_LEN 1 << 10

using namespace std;

int tokenise(char* s, char delimiter, int max_tokens, vector<string> &tokens){
    int i = 0;
    char* token = strtok(s, &delimiter);
    while(token != NULL && i < max_tokens){
        tokens.push_back(token);
        token = strtok(NULL, &delimiter);
        i++;
    }
    return i;
}

struct HeapNode {
    string sentence;                                    //element present at the heap node 
    int index;                                       //index of run from where the element i.e. the string is taken 
    HeapNode(string a, int b): sentence(a), index(b) {}
    bool operator<(const HeapNode& rhs) const {       //In the priority queue, there is a predefined boolean function "operator<()" , we overload it
        return (sentence.compare(rhs.sentence) > 0);
    }
};


int generate_runs(string input_name, long long TOTAL_MEM, int key_count) {
    cout << "Total memory available: " << TOTAL_MEM << endl;
    ifstream input; 
    input.open(input_name.c_str());

    if (!input.good()) {
        cout << "File input is not found!" << endl << "Exit program!" << endl;
        exit(-1);
    }

    int remaining_key_count = key_count;
    // int input_size; 
    // input.seekg(0, input.end);
    // input_size = input.tellg();
    // input.seekg(0, input.beg);
    // cout << "-------------------------------------------------------\n";
    // cout << "The size of the file chosen is (in bytes): " << input_size << endl;

    int run_count = 0;
    int total_mem_so_far = 0;

    ofstream output;
    vector<string> data; data.clear(); // why clear here?

    cout << "File " << input_name << " is being read!" << endl;
    cout << "-------------------------------------------------------\n\n\n";

    cout << "-------------------------------------------------------\n";

    string overflow_string = "";
    string overflow_token = "";
    while ( (!input.eof()) && (remaining_key_count > 0) ) {
        char block[BLOCK_SIZE];
        input.read(block, BLOCK_SIZE);

        int read_size = input.gcount();
        // cout << "read size = " << read_size << endl;

        // char* pos = strrchr(block, '\n');
        // if (pos != NULL && pos != &block[read_size-1]) {
        //     *pos = '\0';
        //     pos++;
        //     overflow_string = pos;
        // } else {
        //     overflow_string = "";
        // }

        vector<string> tokens;
        tokenise(block, '\n', remaining_key_count, tokens);
        if (tokens.empty()) {
            break;
        }

        // cout << "overflow token = " << overflow_token << endl;
        tokens[0] = overflow_token + tokens[0];

        if (block[read_size-1] != '\n') {
            overflow_token = tokens[tokens.size()-1];
            tokens.pop_back();
        } else {
            overflow_token = "";
        }

        remaining_key_count -= tokens.size();

        // cout << "first token = " << tokens[0] << endl;
        // cout << "last token = " << tokens[tokens.size()-1] << endl;

        for ( string sentence : tokens ) {
            if (total_mem_so_far + sentence.size() < TOTAL_MEM) {
                total_mem_so_far += sentence.size() + 1; //coz strings are null terminated ig
                data.push_back(sentence);
            } else {        //we have obtained a run 
                sort(data.begin(), data.end());

                run_count++;

                string to_write = "";
                for (string s : data) {
                    to_write += s;
                    to_write += "\n";
                }

                string filename = "temp.0." + to_string(run_count);
                cout << "Writing " << filename << endl;
                output.open(filename, ios::out | ios::trunc);
                output.write(to_write.c_str(), to_write.size());
                //now we have written run i on file run_i.txt 
                output.close();

                data.clear();
                total_mem_so_far = sentence.size();
                data.push_back(sentence);
            }
        } 
    }


    if (data.size() > 0) {          //when input file has been finished reading but data vector ko abhi tk kisi run_i.txt file mein unload nhi kiya
        sort(data.begin(), data.end());

        run_count++;

                string to_write = "";
                for (string s : data) {
                    to_write += s;
                    to_write += "\n";
                }

                string filename = "temp.0." + to_string(run_count);
                cout << "Writing " << filename << endl;
                output.open(filename, ios::out | ios::trunc);
                output.write(to_write.c_str(), to_write.size());
                //now we have written run i on file run_i.txt 
                output.close();
    }

    //now we have all the runs created on disk in txt files 
    
    input.close();
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

    for (int i = 0; i < runs_count; i++) {
        string sentence;
        if (!input[i].eof()) {
            getline(input[i], sentence);
            heap.push(HeapNode(sentence, i));
        }
    }

    cout << "-------------------------------------------------------\n";
    cout << endl << "Merging from run" << start_idx << " to run" << end_idx << endl;

    string to_write = "";

    int outcount = 0;
    while (!heap.empty()) {
        string sentence = heap.top().sentence; 
        int index = heap.top().index;
        heap.pop();


        if(outcount == 0) to_write += sentence;
        else to_write += "\n" + sentence;

        outcount ++;
        
        if (!input[index].eof()) {
            getline(input[index], sentence);
            heap.push(HeapNode(sentence, index));
        }
    }

    cout << "Merge done!" << endl << endl;
    cout << "-------------------------------------------------------\n\n\n";

    ofstream output;
    stringstream ss;
    ss << "temp." << stage + 1 << "." << stage_count;
    output.open(ss.str());
    output.write(to_write.c_str(), to_write.size());
    output.close();

    for (int i = 0; i < runs_count; i++) {
        input[i].close();
    }    

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
    int runs_count = generate_runs(input, MEM_AVAIL >> 3, keycount); 
    int anss = merge_runs(runs_count , k , output , 0);
    return anss;
}
