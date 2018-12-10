/* 
 * File:   bms2B.cpp
 * Author: Maros Vasilisin, xvasil02
 * Date:   10.12.2018 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <math.h>

using namespace std;

int main(int argc, char** argv) {
    
    if (argc != 2) {
        cerr << "Invalid format, correct: " << argv[0] << " file_name" << endl;
        return EXIT_FAILURE;
    }

    string input_file_name = argv[1];
    string output_file_name = input_file_name.substr(0, input_file_name.find_last_of('.')) + ".ok";
    
    ifstream input_stream;
    input_stream.open(input_file_name, ios::in);

    if (!input_stream.is_open()) {
        cerr << "Cannot open input: " << argv[0] << endl;
        return EXIT_FAILURE;
    }

    input_stream.close();    
    return EXIT_SUCCESS;
}

