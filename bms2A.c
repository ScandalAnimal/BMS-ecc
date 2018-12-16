/* 
 * File:   bms2A.cpp
 * Author: Maros Vasilisin, xvasil02
 * Date:   16.12.2018 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ecc.h"

#define DATA_LENGTH 104
#define BLOCK_LENGTH (DATA_LENGTH + NPAR)


int main (int argc, char *argv[]) {
    
    // kontrola argumentov
    if (argc != 2) {
        fprintf(stderr, "Invalid format, correct: %s file_name\n", argv[0]);
        return -1;
    }

    // ziskanie output suboru zo vstupneho
    char* input_file_name = argv[1];
    char output_file_name[strlen(input_file_name) + 4];
    strcpy(output_file_name, input_file_name);
    strcat(output_file_name, ".out");
 
    // otvorenie oboch suborov, input len read, output je write binary
    FILE* input_file;  
    if ((input_file = fopen (input_file_name, "r")) == NULL) {
        fprintf(stderr, "Cannot open input: %s\n", input_file_name);
        return -1;
    }

    FILE* output_file;  
    if ((output_file = fopen (output_file_name, "wb")) == NULL) {
        fprintf(stderr, "Cannot open output: %s\n", output_file_name);
        return -1;
    }

    // vypocet velkosti vstupu
    fseek(input_file, 0L, SEEK_END);
    int input_size = ftell(input_file);
    fseek(input_file, 0L, SEEK_SET);
    
    // pocet kodovych slov co vieme spravit zo vstupu
    int codewords = (input_size / DATA_LENGTH) + 1;

    unsigned char **output_matrix = malloc(BLOCK_LENGTH * sizeof(unsigned char *));
    if (output_matrix == NULL) {
        fprintf(stderr, "Allocation error");
        return -1;
    }
    for (int i = 0; i < BLOCK_LENGTH; i++) {
        output_matrix[i] = (unsigned char*)malloc(codewords * sizeof(unsigned char));
        if (output_matrix[i] == NULL) {
            fprintf(stderr, "Allocation error");
            return -1;       
        }
    }
    
    initialize_ecc ();

    unsigned char buffer[DATA_LENGTH + 1];
    unsigned char encoded[BLOCK_LENGTH];

    int bytes_read = 0;
    int encoded_length = 0;
    for (int i = 0; i < codewords; i++) {
        
        for (int x = 0; x < DATA_LENGTH + 1; x++) {
            buffer[x] = '\0';
        }     
        for (int x = 0; x < BLOCK_LENGTH; x++) {
            encoded[x] = '\0';
        }     

        // nacitame si kodove slovo, ulozime do matice po stlpcoch
        bytes_read = fread(buffer, 1, DATA_LENGTH, input_file);
        if (bytes_read != 0) {
            // zakodujeme si kodove slovo paritnymi bytami
            encode_data (buffer, bytes_read, encoded);
            encoded_length = bytes_read + NPAR;
            for (int j = 0; j < encoded_length; j++) {
                output_matrix[j][i] = encoded[j];
            }
        }
    }
    
    // z matice citame po riadkoch, posledny riadok orezeme aby tam neboli znaky naviac
    for (int i = 0; i < BLOCK_LENGTH; i++) {
        
        if (i < encoded_length) {
            fwrite (output_matrix[i], sizeof(unsigned char), codewords, output_file);            
        }
        else {
            fwrite (output_matrix[i], sizeof(unsigned char), codewords - 1, output_file);        
        }
    }

    fclose(input_file);
    fclose(output_file);

    free(output_matrix);
    
    return 0;
}
