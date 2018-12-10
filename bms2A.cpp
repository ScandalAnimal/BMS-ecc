/* 
 * File:   bms2A.cpp
 * Author: Maros Vasilisin, xvasil02
 * Date:   10.12.2018 
 */

#include <iostream>
#include <fstream>

using namespace std;

// #define CODEWORD_DATA 223
// #define CODEWORD_PARITY 32
#define CODEWORD_DATA 15
#define CODEWORD_PARITY 3

int main(int argc, char** argv) {

    if (argc != 2) {
        cerr << "Invalid format, correct: " << argv[0] << " file_name" << endl;
        return EXIT_FAILURE;
    }

    string input_file_name = argv[1];
    string output_file_name = input_file_name.substr(0, input_file_name.find_last_of('.')) + ".out";

  	ifstream input_stream;
  	input_stream.open(input_file_name, ios::binary);
    int input_length = 0;

    if (!input_stream.is_open()) {
        cerr << "Cannot open input: " << argv[0] << endl;
        return EXIT_FAILURE;
    }

    input_stream.seekg(0, input_stream.end);
    input_length = input_stream.tellg();
    input_stream.seekg(0, input_stream.beg);

    printf("LENGTH: %d\n", input_length);

    int codewords = (input_length / CODEWORD_DATA) + 1;

    char **buffer = new char*[codewords];
    for (int i = 0; i < codewords; ++i) {
        buffer[i] = new char[CODEWORD_DATA];
    }

    int iterator = 0;
    while (!input_stream.eof()) {

        for (int i = 0; i < CODEWORD_DATA; i++) {
           buffer[iterator][i] = '\0';
        }

        input_stream.read(buffer[iterator], CODEWORD_DATA);
        for (int i = 0; i < CODEWORD_DATA; i++) {
            printf("%d ", buffer[iterator][i]);
        }
        printf("\n");
        iterator++;
    }






    
    printf("******\n");

    for (int i = 0; i < CODEWORD_DATA; i++){
        for (int j = 0; j < codewords; j++) {
            printf("%d ", buffer[j][i]);
        }
        printf("\n");
    }

    for (int i = 0; i < codewords; ++i) {
        delete[] buffer[i];
    }
    delete[] buffer;

    input_stream.close();

    return EXIT_SUCCESS;

}

