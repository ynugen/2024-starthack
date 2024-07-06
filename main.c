#include "neural.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>


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
        //if (i == 3) break;
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
        // printf("FILE NAME: %s\n",nameList[i]->d_name);
        // printf("IDX: %d\n", idx);
        fprintf(results, "%d, %c\n", idx, lookup(idx));
        fclose(t);
    }
    nnetwork_free(nn);
    fclose(results);
    //closedir(dp);

    
    // printf("HELLO\n");
    return 0;
}