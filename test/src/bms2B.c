/* 
 * File:   bms2B.cpp
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

    // ziskanie vystupneho suboru
    char* input_file_name = argv[1];
    char output_file_name[strlen(input_file_name) + 3];
    strcpy(output_file_name, input_file_name);
    strcat(output_file_name, ".ok");
  
    FILE* input_file;  
    if ((input_file = fopen (input_file_name, "rb")) == NULL) {
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

    // pocet kolko kodovych slov mame na vstupe
    int codewords = (input_size / BLOCK_LENGTH) + 1;

    unsigned char **output_matrix = malloc(codewords * sizeof(unsigned char *));
    if (output_matrix == NULL) {
        fprintf(stderr, "Allocation error");
        return -1;
    }
    for (int i = 0; i < codewords; i++) {
        output_matrix[i] = (unsigned char*)malloc(BLOCK_LENGTH * sizeof(unsigned char));
        if (output_matrix[i] == NULL) {
            fprintf(stderr, "Allocation error");
            return -1;       
        }
    }

    initialize_ecc();

    unsigned char input_buffer[codewords];

    int bytes_read = 0;
    int blocks = input_size % BLOCK_LENGTH;
    for (int i = 0; i < BLOCK_LENGTH; i++) {       
        
        for (int j = 0; j < codewords; j++) {
           input_buffer[j] = '\0';
        }

        // citame vstup po kodovych slovach, ukladame do matice po stlpcoch
        // mame kontrolu na posledne kodove slovo, obsahuje menej znakov ako ostatne
        if (i < blocks) {
            bytes_read = fread (input_buffer, 1, codewords, input_file);            
        }
        else {
            bytes_read = fread (input_buffer, 1, codewords - 1, input_file);
        }

        if (bytes_read != 0) {
            for (int j = 0; j < bytes_read; j++) {
                output_matrix[j][i] = input_buffer[j];
            }
        }
    }

    // prechadazame maticu po riadkoch, kazdy z nich dekodujeme, vypisujeme na vystup
    // posledny riadok ma menej znakov
    for (int i = 0; i < codewords; i++) {
        
        int decode_size = 0;
        if (i == (codewords - 1)) {
            decode_size = blocks;
        }
        else {
            decode_size = BLOCK_LENGTH;
        }
        decode_data(output_matrix[i], decode_size);            


        if (check_syndrome () != 0) {
            correct_errors_erasures (output_matrix[i], decode_size, 0, NULL);
        }
        fwrite (output_matrix[i] , sizeof(char), decode_size-NPAR, output_file);
    }


    fclose(input_file);
    fclose(output_file);

    free(output_matrix);
    
    return 0;
}