#include "1.cpp"

int main(){

    const char* input_file = "dataset/random.txt";
    const char* output_file = "./a2.txt" ;
    const long key_cnt = 1000000;
    const int k = 2;
    const int nummerges = 0;

    auto begin = std::chrono::high_resolution_clock::now();

    int ans = external_merge_sort_withstop(input_file , output_file , key_cnt , k , nummerges);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);


    cout << "final ans:" << ans << endl;
    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);

    return 0;
}
