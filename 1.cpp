#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>


using namespace std;


int generate_runs(string input_name, int TOTAL_MEM, int key_count) {
    ifstream input; 
    input.open(input_name.c_str());

    if (!input.good()) {
        cout << "File input is not found!" << endl << "Exit program!" << endl;
        exit(-1);
    }

    int local_key_count = 0;
    int input_size; 
    input.seekg(0, input.end);
    input_size = input.tellg();
    input.seekg(0, input.beg);
    cout << "-------------------------------------------------------\n";
    cout << "The size of the file chosen is (in bytes): " << input_size << endl;

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




int external_merge_sort_withstop(const char* input, const char* output, const long keycount , const int k, const int num_merges){

    int runs_count = generate_runs(input, 1000, keycount);

    return 0;
}