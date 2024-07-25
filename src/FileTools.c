/***************************************************************************************************
 * @file FileTools.c
 * @brief Tools to run the ReedSolomon algorithm on files. 
 *
 * @version   1.0
 * @date      2024-07-23
 * @author    @dabecart
 *
 * @license
 * This project is licensed under the MIT License - see the LICENSE file for details.
 **************************************************************************************************/

#include "FileTools.h"

/***************************************************************************************************
 * FILE REPARATION
 **************************************************************************************************/

void createRecuperationFile(const char* filename, const char* out){
    FILE* inputFile = fopen(filename, "rb");
    if (inputFile == NULL) {
        printf("File %s. ", filename);
        fflush(stdout);
        perror("Error message");
        exit(-1);
    }

    FILE* outputFile = fopen(out, "wb");
    if (outputFile == NULL) {
        printf("File %s. ", out);
        fflush(stdout);
        perror("Error creating output file");
        fclose(inputFile);
        exit(-1);
    }

    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    int x[NUM_POINTS_SAMPLE];
    int y[NUM_POINTS_SAMPLE];

    for(int i = 0; i < NUM_POINTS_SAMPLE; i++)    x[i] = i;

    long filePosition = 0;
    while(filePosition < fileSize){
        printLoadingBar(filePosition, fileSize);
        memset(y, 0, sizeof(y));

        for(int i = 0; i < NUM_POINTS_SAMPLE; i++){
            unsigned char read = fgetc(inputFile);

            if(read == EOF) break;

            y[i] = read; 
        }

        int xx[RS_MAX_POLY_DEGREE];
        int yy[RS_MAX_POLY_DEGREE + 1];
        addErrorCorrectionFields(x, y, NUM_POINTS_SAMPLE, xx, yy);

        for(int j = NUM_POINTS_SAMPLE; j < RS_MAX_POLY_DEGREE+1; j++){
            unsigned char putData = yy[j] & 0xFF;
            int writeResult = fputc(putData, outputFile);
            if(writeResult == EOF){
                printf("\nThe program is not writing properly. %d => %d\n", putData, writeResult);
                fclose(inputFile);
                fclose(outputFile);
                exit(-1);
            }
        }

        filePosition += NUM_POINTS_SAMPLE;
        fseek(inputFile, filePosition, SEEK_SET);
    }
    printLoadingBar(fileSize, fileSize);

    if(filePosition >= fileSize){
        printf("\nFile completely error proofed! %s -> %s\n", filename, out);
    }else{
        printf("\nFile wasn't completely processed!\n");
    }

    fclose(inputFile);
    fclose(outputFile);
}

void recuperateFile(const char* inputFilename, const char* recuperationFilename, const char* out){
    FILE* inputFile = fopen(inputFilename, "rb");
    if (inputFile == NULL) {
        printf("File %s. ", inputFilename);
        fflush(stdout);
        perror("Error opening input file");
        exit(-1);
    }

    FILE* recFile = fopen(recuperationFilename, "rb");
    if (recFile == NULL) {
        printf("File %s. ", recuperationFilename);
        fflush(stdout);
        perror("Error opening the recuperation file");
        fclose(inputFile);
        exit(-1);
    }

    FILE* outputFile = fopen(out, "wb");
    if (outputFile == NULL) {
        printf("File %s. ", out);
        fflush(stdout);
        perror("Error creating output file");
        fclose(inputFile);
        fclose(recFile);
        exit(-1);
    }

    fseek(inputFile, 0, SEEK_END);
    long inputFilesize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    fseek(recFile, 0, SEEK_END);
    long recFilesize = ftell(recFile);
    fseek(recFile, 0, SEEK_SET);

    int x[RS_MAX_POLY_DEGREE];
    int y[RS_MAX_POLY_DEGREE+1];

    for(int i = 0; i < RS_MAX_POLY_DEGREE; i++)    x[i] = i;

    long filePosition = 0;
    long correctionPosition = 0;
    long blocksCorrected = 0;
    long totalBlocks = 0;
    while(filePosition < inputFilesize && correctionPosition < recFilesize){
        printLoadingBar(filePosition, inputFilesize);
        memset(y, 0, sizeof(y));

        for(int i = 0; i < NUM_POINTS_SAMPLE; i++){
            unsigned char read = fgetc(inputFile);

            if(read == EOF) break;

            y[i] = read; 
        }

        for(int i = NUM_POINTS_SAMPLE; i < RS_MAX_POLY_DEGREE+1; i++){
            unsigned char read = fgetc(recFile);

            if(read == EOF) break;

            y[i] = read; 
        }

        AlgorithmReturn success = verifyMessage(x, y, RS_MAX_POLY_DEGREE, NUM_POINTS_SAMPLE);
        if(success < 0){
            printf("\nError fixing the file at: 0x%08lX. Correction file position: 0x%08lX.\nData: ", filePosition, correctionPosition);
            for(int i = 0; i < RS_MAX_POLY_DEGREE+1; i++){
                if(i==NUM_POINTS_SAMPLE) printf(" - ");
                printf("%02X", y[i]);
            }
            printf("\n");
        }else{
            blocksCorrected++;
        }
        totalBlocks++;

        // Save the corrected data.
        for(int j = 0; j < NUM_POINTS_SAMPLE; j++){
            char putData = y[j] & 0xFF;
            fputc(putData, outputFile);
        }

        filePosition += NUM_POINTS_SAMPLE;
        fseek(inputFile, filePosition, SEEK_SET);
        correctionPosition += EXTRA_POINTS+1;
        fseek(inputFile, filePosition, SEEK_SET);
    }
    printLoadingBar(inputFilesize, inputFilesize);

    if(filePosition >= inputFilesize && correctionPosition >= recFilesize){
        printf("\nCorrection completed! %ld of %ld blocks OK! (%s, %s) -> %s\n", 
            blocksCorrected, totalBlocks, inputFilename, recuperationFilename, out);
    }else{
        printf("\nThe files were misaligned or an external error happened!\n");
        printf("Input: %ld/%ld, Correction: %ld/%ld\n", 
            filePosition, inputFilesize, correctionPosition, recFilesize);
    }

    fclose(inputFile);
    fclose(recFile);
    fclose(outputFile);
}