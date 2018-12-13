/* 
 * File:   bms2B.cpp
 * Author: Maros Vasilisin, xvasil02
 * Date:   10.12.2018 
 */

#include <iostream>
#include <fstream>
#include "rs.c"

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
    int input_length = 0;

    if (!input_stream.is_open()) {
        cerr << "Cannot open input: " << argv[0] << endl;
        return EXIT_FAILURE;
    }

    input_stream.seekg(0, input_stream.end);
    input_length = input_stream.tellg();
    input_stream.seekg(0, input_stream.beg);

    ofstream output_stream;
    output_stream.open(output_file_name, ios::binary);

    if (!output_stream.is_open()) {
        cerr << "Cannot open output: " << output_file_name << endl;
        return EXIT_FAILURE;
    }

    printf("LENGTH: %d\n", input_length);

    int codewords = (input_length / CODEWORD_LENGTH) + 1;
    int mod = (input_length % CODEWORD_LENGTH);

    char **buffer = new char*[codewords];
    for (int i = 0; i < codewords; ++i) {
        buffer[i] = new char[CODEWORD_LENGTH + 1];
    }

    char *input_buffer = new char[codewords];
    char *output_buffer = new char[CODEWORD_LENGTH];

    int iterator = 0;
    while (!input_stream.eof()) {

        for (int i = 0; i < codewords; i++) {
           input_buffer[i] = '\0';
        }

        if (iterator < mod) {
            input_stream.read(input_buffer, codewords);
        }
        else {
             input_stream.read(input_buffer, codewords - 1);   
        }
        streamsize readNum = input_stream.gcount();
        // printf("READNUM: %d\n", readNum);
        if (readNum != 0) {
            for (int i = 0; i < codewords; i++) {

                buffer[i][iterator] = input_buffer[i];
                // printf("%02X ", buffer[i][iterator]);

            }
        }
        // printf("***\n");

        iterator++;
    }
    
    for (int i = 0; i < codewords; i++){
        for (int j = 0; j < CODEWORD_LENGTH; j++) {
            printf("%02X ", buffer[i][j]);
        }
        printf("*************\n");
    }

        // printf("^^^^^^^^^^^^\n");

    generate_gf();
    // printf("Look-up tables for GF(2**%2d)\n",mm) ;
    // printf("  i   alpha_to[i]  index_of[i]\n") ;
    // for (int i=0; i<=CODEWORD_LENGTH; i++)
    //     printf("%3d      %3d          %3d\n",i,alpha_to[i],index_of[i]) ;
    // printf("\n\n") ;

    gen_poly();

    for (int i = 0; i < codewords; i++){
        for (int j = 0; j < CODEWORD_LENGTH; j++) {
            // printf("%d ", buffer[i][j]);
            recd[j] = buffer[i][j];
            int x = (int)recd[j];
            if (x < 0) {
                recd[j] += 256;
            }
//            // printf("(%d %02X# ", j, recd[j]) ;
//                // if (j % 5 == 0) {
 //                  // printf("\n");
//                // }
//            printf("%02X ", recd[j]);
            // printf("%d ", recd[j]);
            // output_buffer[j] = buffer[j][i];
        }
        // printf("\n");

        for (int x = 0; x < CODEWORD_LENGTH; x++) {
            recd[x] = index_of[recd[x]] ;          /* put recd[i] into index form */
        }
        // printf("RECD:\n");
        // for (int j = 0; j < CODEWORD_LENGTH; j++) {
        //     // printf("%d ", buffer[i][j]);
        //     printf("%d ", recd[j]);
        //     // output_buffer[j] = buffer[j][i];
        // }
        decode_rs();
        // for (int x=0; x<CODEWORD_LENGTH-CODEWORD_DATA; x++) {
            // printf("%3d    %3d      %3d\n",x, bb[x], recd[x]) ;
        // }
        if (i == codewords - 1) {
            int it = 0;
            for (int x=CODEWORD_LENGTH-CODEWORD_DATA; x<CODEWORD_DATA + 2; x++) {
               // printf("(%d %d)# ", x, recd[x]) ;
               //  if (x % 5 == 0) {
               //      printf("\n");
               //  }
                            // printf("%02X ", recd[x]);

                output_buffer[it] = recd[x];
                it++;
            } 
            output_stream.write(output_buffer, it);           
        }
        else {
            int it = 0;
            // for (int x=CODEWORD_LENGTH-CODEWORD_DATA; x<CODEWORD_DATA + CODEWORD_PARITY + 1; x++) {
            for (int x=CODEWORD_LENGTH-CODEWORD_DATA; x<CODEWORD_LENGTH; x++) {
                
                            // printf("%02X ", recd[x]);

                output_buffer[it] = recd[x];
                // printf("(%d %d)# ", it, output_buffer[it]) ;
                // if (x % 5 == 0) {
                //     printf("\n");
                // }
                it++;
            }   
            output_stream.write(output_buffer, it);
        }

        // output_stream.write(output_buffer, codewords);
//        printf("codewords: %d\n", codewords);
//        printf("\n\n");

    }



    for (int i = 0; i < codewords; ++i) {
        // delete[] buffer[i];
    }
    // delete[] buffer;
    // delete[] input_buffer;

//    printf("mod %d\n", mod);
    input_stream.close(); 
    output_stream.close();   
    return EXIT_SUCCESS;
}

