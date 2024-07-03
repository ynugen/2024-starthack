#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define WEIGHTS_AND_BIASES_DIR "./weights_and_biases.txt"
#define BUFFSIZE 8192


typedef struct {
    double ***weight_matrices;
    double **bias_arrays;

} weight_and_bias_t; 



weight_and_bias_t* readWeightsAndBiases();
bool needsConcatenation(char *finalToken, char *token);



int main(int argc, char *argv[]) {
    
    weight_and_bias_t *wab = readWeightsAndBiases();
    printf("post weights and bias read\n");

    // placeholder to get running
    //printf("First values is %f", wab->weight_matrices[0][0][0]);


    free(wab);
    return 0;
}


weight_and_bias_t* readWeightsAndBiases() {

    // ensure file can open properly
    FILE *fp = fopen(WEIGHTS_AND_BIASES_DIR , "r"); 
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // allocate memory to the struct
    weight_and_bias_t *wab = (weight_and_bias_t*)malloc(sizeof(weight_and_bias_t));
    if (wab == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char buffer[BUFFSIZE];
    size_t bytesRead;

    const char delim[] = ",\n"; // delimiters to split
    char *token; // array of tokens
    char finalToken[30];

    int bytesScanned = 0;
    bool firstIteration = true;

    // main loop to process entire file of weights and biases
    while ((bytesRead = fread(buffer, 1, BUFFSIZE, fp)) > 0) {

        bytesScanned = 0;
        // check if this is the first iteration
        if (firstIteration) {

            token = strtok(buffer, delim);
            bytesScanned += strlen(token) + 1;
            firstIteration = false;

        } else { // otherwise check if the first token in the buffer is fully intact
            
            token = strtok(buffer, delim);
            bytesScanned += strlen(token) + 1;

            // check if the strings need to be concatenated
            if (needsConcatenation(finalToken, token)) {
                strcat(finalToken, token);
                printf("concatenated = %s\n", finalToken);
            }
        }
        
        while (bytesScanned < bytesRead) {
            token = strtok(NULL, delim);
            bytesScanned += strlen(token) + 1; // +1 for each delimiter
            //printf("token = \"%s\", bytes scanned = %d\n", token, bytesScanned);
        }
        strcpy(finalToken, token);

        //fwrite(buffer, 1, bytesRead, stdout);
        /*if (count++ == 5) {
            break;
        }*/
    }
   
    fclose(fp);

    return wab;
}

bool needsConcatenation(char *finalToken, char *token) {

    printf("\nCHECKING CONCATENATION final is %s, new is %s\n", finalToken, token);

    if (strlen(finalToken) == 1 && finalToken[0] == '-') { // if previous token was '-' then needs to be concatenated
        return true;
    }

    if (token[1] == '.' || token[0] == '-') { // intact if either _.___ or -____
        return false;
    }

    return  true; // presume not intact
}
