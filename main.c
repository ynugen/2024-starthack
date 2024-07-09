#include "neural.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <stdbool.h>



#define WEIGHT_BIAS_PATH 1
#define TENSOR_DIR_PATH 2

int compare(const struct dirent **a, const struct dirent **b) {
    return alphasort(a, b);
}

int main(int argc, char** argv) {

    /* Check arg inputs */
    char* weights_biases = (argc > WEIGHT_BIAS_PATH) ? argv[WEIGHT_BIAS_PATH] :"weights_and_biases.txt";
    char* tensor_dir = (argc >TENSOR_DIR_PATH) ? argv[TENSOR_DIR_PATH] : "tensors";
    // printf("%s\n%s\n", weights_biases, tensor_dir);

    /* Open weights and biases file to read */
    FILE *f = fopen(weights_biases, "r");
    assert(f);
    
    nnetwork_t *nn = nnetwork_read(f);
    fclose(f);

    /* Open single tensor file to read */
    FILE *t = fopen("tensors/01out.txt","r");
    assert(t);
    nnetwork_run(nn, t);
    fclose(t);    
    nnetwork_print(nn);

    /* Create output file */
    FILE *results = fopen("results.csv", "w");
    /* Add header to file */
    fprintf(results, "image_number, guess\n");

    /* Read and run neural network on all files in give input tensor dir path */
    //DIR *dp = opendir(tensor_dir);
    struct dirent **nameList;
    int n = scandir(tensor_dir, &nameList, NULL, alphasort);
    if (n < 0) {
            perror("main: Unable to open directory");
            exit(EXIT_FAILURE);
        }

    //struct dirent *tensor_file;
    //while ((tensor_file = readdir(dp))) {

    for (int i = 0; i < n; i++) {
        //if (i == 4) break;
        /* Skip current and parent dir */
        if (!strcmp(nameList[i]->d_name, ".") || !strcmp(nameList[i]->d_name, "..")) {
            // i should probs add a free here later (check valgrind)
            continue;
        }
        //printf("HELLO\n");
       
        // printf("FILE: %s\n", tensor_file->d_name);
        char tensor_path[1024];
        snprintf(tensor_path, sizeof(tensor_path), "%s/%s", tensor_dir, nameList[i]->d_name);
        //printf("HELLO2\n");
        FILE *t = fopen(tensor_path, "r");
        if (t == NULL) {
            perror("main: Unable to open file");
            exit(EXIT_FAILURE);
        }

        int idx = nnetwork_run(nn, t);

        //printf("FILE NAME: %s\n",nameList[i]->d_name);
        //printf("IDX: %d\n", idx);
        fprintf(results, "%d, %c\n", i - 1, lookup(idx));
        fclose(t);
    }
    nnetwork_free(nn);
    fclose(results);
    //closedir(dp);

    
    // printf("HELLO\n");
    return 0;
}

/* QDX main.c code */
// #define WEIGHTS_AND_BIASES_DIR "./weights_and_biases.txt"
// #define BUFFSIZE 8192


// typedef struct {
//     double ***weight_matrices;
//     double **bias_arrays;

// } weight_and_bias_t; 



// weight_and_bias_t* readWeightsAndBiases();
// bool needsConcatenation(char *finalToken, char *token);



// int main(int argc, char *argv[]) {
    
//     weight_and_bias_t *wab = readWeightsAndBiases();
//     printf("post weights and bias read\n");

//     // placeholder to get running
//     //printf("First values is %f", wab->weight_matrices[0][0][0]);


//     free(wab);
//     return 0;
// }


// weight_and_bias_t* readWeightsAndBiases() {

//     // ensure file can open properly
//     FILE *fp = fopen(WEIGHTS_AND_BIASES_DIR , "r"); 
//     if (fp == NULL) {
//         perror("Error opening file");
//         exit(1);
//     }

//     // allocate memory to the struct
//     weight_and_bias_t *wab = (weight_and_bias_t*)malloc(sizeof(weight_and_bias_t));
//     if (wab == NULL) {
//         fprintf(stderr, "Memory allocation failed\n");
//         exit(EXIT_FAILURE);
//     }

//     int count = 0;
//     char buffer[BUFFSIZE];
//     size_t bytesRead;

//     const char delim[] = ",\n"; // delimiters to split
//     char *token; // array of tokens
//     char finalToken[30];

//     int bytesScanned = 0;
//     bool firstIteration = true;

//     // main loop to process entire file of weights and biases
//     while ((bytesRead = fread(buffer, 1, BUFFSIZE, fp)) > 0) {

//         bytesScanned = 0;
//         // check if this is the first iteration
//         if (firstIteration) {

//             token = strtok(buffer, delim);
//             bytesScanned += strlen(token) + 1;
//             firstIteration = false;

//         } else { // otherwise check if the first token in the buffer is fully intact
            
//             token = strtok(buffer, delim);
//             bytesScanned += strlen(token) + 1;

//             // check if the strings need to be concatenated
//             if (needsConcatenation(finalToken, token)) {
//                 strcat(finalToken, token);
//                 printf("concatenated = %s\n", finalToken);
//             }
//         }
        
//         while (bytesScanned < bytesRead) {
//             token = strtok(NULL, delim);
//             bytesScanned += strlen(token) + 1; // +1 for each delimiter
//             //printf("token = \"%s\", bytes scanned = %d\n", token, bytesScanned);
//         }
//         strcpy(finalToken, token);

//         //fwrite(buffer, 1, bytesRead, stdout);
//         /*if (count++ == 5) {
//             break;
//         }*/
//     }
   
//     fclose(fp);

//     return wab;
// }

// bool needsConcatenation(char *finalToken, char *token) {

//     printf("\nCHECKING CONCATENATION final is %s, new is %s\n", finalToken, token);

//     if (strlen(finalToken) == 1 && finalToken[0] == '-') { // if previous token was '-' then needs to be concatenated
//         return true;
//     }

//     if (token[1] == '.' || token[0] == '-') { // intact if either _.___ or -____
//         return false;
//     }

//     return  true; // presume not intact
// }
