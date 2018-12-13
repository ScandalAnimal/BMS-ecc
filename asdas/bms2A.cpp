/* 
 * File:   bms2A.cpp
 * Author: Maros Vasilisin, xvasil02
 * Date:   10.12.2018 
 */

#include <iostream>
#include <fstream>
#include "rs.c"

using namespace std;

// #define CODEWORD_DATA 223
// #define CODEWORD_PARITY 32

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

    ofstream output_stream;
    output_stream.open(output_file_name, ios::binary);

    if (!output_stream.is_open()) {
        cerr << "Cannot open output: " << output_file_name << endl;
        return EXIT_FAILURE;
    }

    input_stream.seekg(0, input_stream.end);
    input_length = input_stream.tellg();
    input_stream.seekg(0, input_stream.beg);

    printf("LENGTH: %d\n", input_length);

    int codewords = (input_length / CODEWORD_DATA) + 1;
    int mod = (input_length % CODEWORD_DATA);

    char **buffer = new char*[codewords];
    for (int i = 0; i < codewords; ++i) {
        buffer[i] = new char[CODEWORD_LENGTH];
    }

    char *output_buffer = new char[codewords];

    streamsize last_streamsize = 0;

    /* generate the Galois Field GF(2**mm) */
    generate_gf();
    gen_poly();

    int iterator = 0;
    while (!input_stream.eof()) {

        for (int i = 0; i < CODEWORD_LENGTH; i++) {
           buffer[iterator][i] = '\0';
        }

        input_stream.read(buffer[iterator], CODEWORD_DATA);
        last_streamsize = input_stream.gcount();
        for (int i = 0; i < CODEWORD_DATA; i++) {
            data[i] = buffer[iterator][i];
            int x = (int)data[i];
            if (x < 0) {
                data[i] += 256;
            }
            printf("%d ", data[i]);
        }
        encode_rs();

        /* put the transmitted codeword, made up of data plus parity, in recd[] */
        for (int i = 0; i < CODEWORD_LENGTH-CODEWORD_DATA; i++) {
            buffer[iterator][i] = bb[i];
        }
        for (int i = 0; i < CODEWORD_DATA; i++) {
            buffer[iterator][i+CODEWORD_LENGTH-CODEWORD_DATA] = data[i];
        }
        // for (int i = 0; i < CODEWORD_LENGTH; i++) {
           // buffer[iterator][i] = index_of[(int)buffer[iterator][i]];
        // }

        printf("\n");
        iterator++;
    }
    printf("READNUM: %d\n", last_streamsize);
    printf("******\n");


// /* if you want to test the program, corrupt some of the elements of recd[]
//    here. This can also be done easily in a debugger. */
// /* Again, lets say that a middle element is changed */
//   data[nn-nn/2] = 3 ;

// for (i=0; i<nn; i++)
//      recd[i] = index_of[recd[i]] ;          /* put recd[i] into index form */

// /* decode recv[] */
//   decode_rs() ;          recd[] is returned in polynomial form 


// printf("Results for Reed-Solomon code (n=%3d, k=%3d, t= %3d)\n\n",nn,kk,tt) ;
//   printf("  i  data[i]   recd[i](decoded)   (data, recd in polynomial form)\n");
//   for (i=0; i<nn-kk; i++) {
//     printf("%3d    %3d      %3d\n",i, bb[i], recd[i]) ;
// }
//   for (i=nn-kk; i<nn; i++) {
//     printf("%3d    %3d      %3d\n",i, data[i-nn+kk], recd[i]) ;
// }

    for (int i = 0; i < CODEWORD_LENGTH; i++){
        for (int j = 0; j < codewords; j++) {
            int x = (int)buffer[j][i];
            if (x < 0) {
                buffer[j][i] += 256;
            }
            output_buffer[j] = buffer[j][i];
            printf("%d ", output_buffer[j]);
        }
        printf("\n");
    }
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    for (int i = 0; i < CODEWORD_LENGTH; i++){
        for (int j = 0; j < codewords; j++) {
            // printf("%d ", buffer[j][i]);
            // int x = (int)buffer[j][i];
            // if (x < 0) {
            //     buffer[j][i] += 256;
            // }
            output_buffer[j] = buffer[j][i];
        }
        // if (i > CODEWORD_DATA) {

        //     for (int x = 0; x < codewords - 1; x++)
        //     {
        //         printf("##%d %02X ", i, output_buffer[x]);
        //     }
        //     output_stream.write(output_buffer, codewords - 1);
        // }
        // else {
            // for (int x = 0; x < codewords; x++)
            // {
            //     printf("%d %02X ", i, output_buffer[x]);
            // }
            output_stream.write(output_buffer, codewords);
        // }
        // printf("\n");
    }
        printf("MOD: %d\n", mod);
        printf("CODEWORDS: %d\n", codewords);

    // for (int i = 0; i < codewords; i++){
    //     for (int j = 0; j < CODEWORD_LENGTH; j++) {
    //         printf("%02X ", buffer[i][j]);
    //     }
    //     printf("*************\n");
    // }

    for (int i = 0; i < codewords; ++i) {
        delete[] buffer[i];
    }
    delete[] buffer;
    delete[] output_buffer;

    input_stream.close();
    output_stream.close();

    return EXIT_SUCCESS;

}

