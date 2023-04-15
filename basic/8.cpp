#include <bits/stdc++.h>

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
        cout << "File input is not found! \n Exit program\n";
        exit(-1);
    }

    int local_key_count = 0;
    int run_count = 0;
    int total_mem_so_far = 0;

    ofstream output;
    vector<string> data; 

    cout << "File " << input_name << " is being read!" << "\n";
    cout << "-------------------------------------------------------\n\n\n";
    cout << "-------------------------------------------------------\n";

    string sentence;
    while (getline(input, sentence) && (local_key_count < key_count)) {
        local_key_count ++;

        if (total_mem_so_far + sentence.size() + 1 < TOTAL_MEM) {
            total_mem_so_far += sentence.size() + 1; //coz strings are null terminated ig
            data.push_back(sentence);
        } else {        //we have obtained a run 

            auto t1 = std::chrono::high_resolution_clock::now();
            sort(data.begin(), data.end());
            auto t2 = std::chrono::high_resolution_clock::now();
            auto t3 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
            cout << "sort time measured:" << t3.count() * 1e-9 <<  "seconds.\n";

            run_count++;

            stringstream ss;
            ss << "temp.0." << run_count;
            cout << "Writing " << ss.str() << endl;
            output.open(ss.str(), ios::out | ios::trunc);

            t1 = std::chrono::high_resolution_clock::now();
            ostream_iterator<string> output_iterator(output, "\n");
            copy(begin(data) , end(data)-1 , output_iterator);
            output << data.back();

            t2 = std::chrono::high_resolution_clock::now();
            t3 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
            cout << "write to output file- time measured:" << t3.count()* 1e-9 << "seconds.\n";

            //now we have written run i on file run_i.txt 
            output.close();
            // data.clear();
            // data.push_back(sentence);

            // data.assign(1, sentence);

            data = vector<string>(1, sentence);
            total_mem_so_far = sentence.size() + 1;
        }
    }

    input.close();
    cout << "Read input is done!" << endl;
    cout << "-------------------------------------------------------\n\n\n";

    if (data.size() > 0) {          //when input file has been finished reading but data vector ko abhi tk kisi run_i.txt file mein unload nhi kiya
        sort(data.begin(), data.end());
        cout << "data size " << data.size() << "\n";

        run_count++;
        stringstream ss;
        ss << "temp.0." << run_count;
        cout << "Writing " << ss.str() << "\n";
        output.open(ss.str(), ios::out | ios::trunc);

        ostream_iterator<string> output_iterator(output, "\n");
        copy(begin(data) , end(data)-1 , output_iterator);
        output << data.back();
        output.close();
    }
    data = vector<string>();


    //now we have all the runs created on disk in txt files 

    return run_count;
}

void merge(int start_idx , int end_idx , int stage , int stage_count , int Total_memory , string outfile , bool outflag ){

    int runs_count = end_idx - start_idx + 1;

    ifstream input[runs_count];
    for (int i = 0; i < runs_count; i++) {
        input[i].open("temp." + to_string(stage)  + "." + to_string(start_idx + i + 1));
    }

    priority_queue<HeapNode, vector<HeapNode> > heap;   //std::vector is our container becoz priority_queue is container adapter

    vector<string> outvec;
    int totmem = 0;

    ofstream output;
    stringstream ss;
    if( !outflag ) ss << "temp." << stage + 1 << "." << stage_count;
    else ss << outfile;
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

    // int outcount = 0;
    int flag = 0;
    while (!heap.empty()) {
        string sentence = heap.top().sentence; 
        int index = heap.top().index;
        heap.pop();


        totmem += sentence.size();
        outvec.push_back(sentence);


        if(totmem >= Total_memory){
            if(flag == 0){
                        ostream_iterator<string> output_iterator(output, "\n");
                        copy(begin(outvec) , end(outvec)-1 , output_iterator);
                        output << outvec[outvec.size() - 1];
                        totmem = 0;
                        flag = 1;
            }
            else{
                        output << "\n";
                        ostream_iterator<string> output_iterator(output, "\n");
                        copy(begin(outvec) , end(outvec)-1 , output_iterator);
                        output << outvec[outvec.size() - 1];
                        totmem = 0;                       
            }

            // outvec.clear();
            // vector<string>().swap(outvec);
            outvec = vector<string>();            
        }

        // if(outcount == 0) output << sentence;
        // else output << endl << sentence;

        // outcount ++;
        
        if (!input[index].eof()) {
            getline(input[index], sentence);
            heap.push(HeapNode(sentence, index));
        }
    }

    if(outvec.size() > 0){
        if(flag == 1) output << "\n";
        ostream_iterator<string> output_iterator(output, "\n");
        copy(begin(outvec) , end(outvec)-1 , output_iterator);
        output << outvec[outvec.size() - 1];
    }
    outvec = vector<string>();            


    cout << "Merge done!" << endl << endl;
    cout << "-------------------------------------------------------\n\n\n";

    for (int i = 0; i < runs_count; i++) {
        input[i].close();
    }

    output.close();

}


void merge_noheap(int start_idx , int end_idx , int stage , int stage_count , int Total_memory , string outfile , bool outflag ){
    int runs_count = end_idx - start_idx + 1;

    if(runs_count == 1){
        string ss1 = "temp." + to_string(stage) + "." + to_string(start_idx + 1);

        if(outflag){
            rename(ss1.c_str() , outfile.c_str());
        }
        else{
            string ss2 = "temp." + to_string(stage+1) + "." + to_string(stage_count);
            rename(ss1.c_str() , ss2.c_str());
        }
        return;
    }

    ifstream input[runs_count];
    for (int i = 0; i < runs_count; i++) {
        input[i].open("temp." + to_string(stage) +  "." + to_string(start_idx + i + 1));
    }

    vector<string> outvec;
    vector<string> invec(runs_count);
    int totmem = 0;

    ofstream output;
    stringstream ss;
    if( !outflag ) ss << "temp." << stage + 1 << "." << stage_count;
    else ss << outfile;
    output.open(ss.str());

    for (int i = 0; i < runs_count; i++) {
        string sentence;
        if (getline(input[i], sentence)) {
            invec[i] = sentence;
        }
    }

    cout << "-------------------------------------------------------\n";
    cout << endl << "Merging from run" << start_idx << " to run" << end_idx << endl;
 
    string STRING_MAX(2048, '~');
    bool terminate = false;
    int flag = 0;
    while(!terminate)
    {
        auto min = min_element(begin(invec) , end(invec));
        int index = min - begin(invec);
        if(STRING_MAX.compare(*min) == 0){
            terminate = true;
            break;
        }

        totmem += (*min).size();
        outvec.push_back(*min);
        invec[index] = STRING_MAX;

        if(totmem >= Total_memory){
            if(flag == 0){
                        ostream_iterator<string> output_iterator(output, "\n");
                        copy(begin(outvec) , end(outvec)-1 , output_iterator);
                        output << outvec[outvec.size() - 1];
                        totmem = 0;
                        flag = 1;
            }
            else
            {
                        output << "\n";
                        ostream_iterator<string> output_iterator(output, "\n");
                        copy(begin(outvec) , end(outvec)-1 , output_iterator);
                        output << outvec[outvec.size() - 1];
                        totmem = 0;
            }
            outvec = vector<string>();
        }

        string sentence;
        if (getline(input[index], sentence)) {
            invec[index] = sentence;
        }
        
    }

    if(outvec.size() > 0){
        if(flag == 1) output << "\n";
        ostream_iterator<string> output_iterator(output, "\n");
        copy(begin(outvec) , end(outvec)-1 , output_iterator);
        output << outvec[outvec.size() - 1];
    }
    outvec = vector<string>();

    cout << "Merge done!" << endl << endl;
    cout << "-------------------------------------------------------\n\n\n";

    for (int i = 0; i < runs_count; i++) {
        input[i].close();
    }

    output.close();
}

int merge_runs(int num_runs , int max_fanout , string outfile , int stage , int totmem , int num_merges_allowed , int num_merges_done ){
    //starting mein i have files = temp.stage.0 to temp.stage.(num_runs-1)
    int start = 0;
    int withtin_stage_count = 1;

    if(num_merges_allowed == num_merges_done && num_merges_allowed != 0) return num_merges_allowed ;

    while(start < num_runs){
        if( (start + max_fanout - 1 ) < num_runs) {
            if ( (start + max_fanout == num_runs) && withtin_stage_count == 1 ) merge_noheap(start , start + max_fanout -1 , stage , withtin_stage_count , totmem , outfile , true );
            else merge_noheap(start , start + max_fanout -1 , stage , withtin_stage_count , totmem , outfile , false );
            num_merges_done ++;
            start += max_fanout;
            withtin_stage_count ++ ;
        }
        else{
            if(withtin_stage_count == 1) merge_noheap(start , num_runs - 1 , stage , withtin_stage_count , totmem , outfile , true);
            else merge_noheap(start , num_runs - 1 , stage , withtin_stage_count , totmem , outfile , false);
            num_merges_done ++;
            start = num_runs ;
            withtin_stage_count ++ ;
        }
        if(num_merges_allowed == num_merges_done && num_merges_allowed != 0) return num_merges_allowed ;

    }

    if ( withtin_stage_count == 2 ) return num_merges_done;
    else return (merge_runs(withtin_stage_count -1 , max_fanout , outfile , stage + 1, totmem , num_merges_allowed , num_merges_done)); 

}

int external_merge_sort_withstop(const char* input, const char* output, const long keycount , const int k, const int num_merges){

    int runs_count = generate_runs(input, 50000000*2 , keycount); //5*1e7 is 50 mb  
    int anss = merge_runs(runs_count , k , output , 0 , 50000000*2 , num_merges , 0  );


    return anss;
}
