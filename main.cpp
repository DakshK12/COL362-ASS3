#include "1.cpp"

int main(){

    const char* input_file = "./t1.txt";
    const char* output_file = "./a2.txt" ;
    const long key_cnt = 1000;
    const int k = 2;
    const int nummerges = 0;

    int ans = external_merge_sort_withstop(input_file , output_file , key_cnt , k , nummerges);

    return 0;
}