#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>

#include <omp.h>

using namespace std;

int operator * (const vector<int>& tmp1, const vector<int>& tmp2){
    if (tmp1.size() != tmp2.size()){
        exit(-2);
    }
    int result = 0;
    for (int i = 0; i < tmp1.size(); i++)
        result += tmp1[i] * tmp2[i];
    return result;
}

int main(){
    int max_treads;
    int num_colums;
    int mod;
    int output;
    
    cin >> max_treads;
    cin >> num_colums;
    cin >> mod;
    cin >> output;

    if (max_treads < 1 || num_colums < 1 || mod < 1 || mod > 2)
        return -1;

    vector<vector<int>> my_matrix = vector<vector<int>> (num_colums);
    vector<int> my_vector = vector<int>(num_colums);
    vector<int> my_result_vector = vector<int>(num_colums);
    for (int i = 0; i < num_colums; i++)
        my_matrix[i] = vector<int> (num_colums);

    switch (mod)
    {
        case 1:
            for (int y = 0; y < num_colums; y++){
                for (int x = 0; x < num_colums; x++)
                    if (x == y)
                        my_matrix[y][x] = y + 1;
                    else
                        my_matrix[y][x] = 1;
                    my_vector[y] = 1;    
                }
            break;
        case 2:
            for (int y = 0; y < num_colums; y++){
                for (int x = 0; x < num_colums; x++)
                    if (x == y)
                        my_matrix[y][x] = 2;
                    else
                        my_matrix[y][x] = 1;
                my_vector[y] = 1;    
                }
            break;
        default:
            break;
    }

    cout << "-------------------------------------------" << endl;
    auto start = chrono::steady_clock::now();
    #pragma omp parallel num_threads(max_treads)
    {
        int current_thread = omp_get_thread_num();
        for (int i = current_thread*(num_colums/max_treads); i < (current_thread + 1 == max_treads ? 
                            num_colums : (current_thread + 1) * (num_colums/max_treads)); i++){
            my_result_vector[i] = my_vector * my_matrix[i];
        }
    }
    auto end = chrono::steady_clock::now();

    for (int y = 0; y < num_colums && output; y++){
        for (int x = 0; x < num_colums; x++)
            cout << my_matrix[y][x] << " ";
        cout << endl;
    }
        

    for (int i = 0; i < num_colums && output; i++)
        cout << my_result_vector[i] << " ";
    cout << endl;
    cout << "------------------------------------------" << endl;
    auto tmp = chrono::duration_cast<std::chrono::milliseconds>(end - start);;
    cout << tmp.count() << endl;
}


// pragma opm critical - для того, чтобы блок выполнялся только одним потоком
// #pragma omp parallel
//{
//}
