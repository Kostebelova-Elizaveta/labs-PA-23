#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


void generate_matrix(const std::string& filename, int rows, int columns){
    ofstream file;
    file.open(filename);
    if (!file){
        cout << "Can't open file generate matrix\n";
        return;
    }
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            file << rand() % 100 << ' ';
        }
        file << std::endl;
    }
    file.close();
}

void read_matrix(vector<vector<int>>& matrix, string filename){
    ifstream file(filename);
    if (!file){
        cout << "Can't open file read matrix";
        return;
    }

    string line;
    string delimiter = " ";
    vector<int> row;


    while (getline(file, line)){
        row.clear();
        size_t pos;

        while ((pos = line.find(delimiter)) != std::string::npos) {
            row.push_back(stoi(line.substr(0, pos)));
            line.erase(0, pos + delimiter.length());
        }
        matrix.push_back(row);
    }

    file.close();
}

void read_matrices(vector<vector<int>>& matrix1, vector<vector<int>>& matrix2){
    read_matrix(matrix1, "/home/master/work/etu/PA/lab1/matrix_1.txt");
    read_matrix(matrix2, "/home/master/work/etu/PA/lab1/matrix_2.txt");
}

void multiply_matrices(vector<vector<int>>& matrix1, vector<vector<int>>& matrix2, vector<vector<int>>& matrix_res){
    int row1 = matrix1.size();
    int col1 = matrix1[0].size();
    int row2 = matrix2.size();
    int col2 = matrix2[0].size();



    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            for(int k = 0; k < col1; k++)
                matrix_res[i][j] += matrix1[i][k] * matrix2[k][j];
        }
    }
}

void write_result(vector<vector<int>> matrix_res, string filename){
    ofstream file;
    file.open("/home/master/work/etu/PA/lab1/" + filename);
    for (int i = 0; i < matrix_res.size(); i++){
        for(int j = 0; j < matrix_res[i].size(); j++){
            file << matrix_res[i][j] << ' ';
        }
        file << std::endl;
    }
    file.close();
}

int main(){
    vector<vector<int>> matrix1;
    vector<vector<int>> matrix2;

    read_matrices(matrix1, matrix2);

    vector<vector<int>> matrix_res(matrix1.size(), vector<int>(matrix2[0].size(), 0));

    int sum_pid;

    switch(sum_pid = fork()){
        case -1:
            exit(-1);

        case 0:
            multiply_matrices(matrix1, matrix2, matrix_res);

            int write_pid;
            switch(write_pid = fork()){
                case -1:
                    exit(-1);
                case 0:
                    write_result(matrix_res, "processes_res.txt");
                    exit(1);
                default:
                    wait(&write_pid);
            }
            exit(1);
        default:
            wait(&sum_pid);
    }
}

