#include <stdio.h>
#include <stdlib.h>


#define WEIGHTS_AND_BIASES_DIR "./weights_and_biases.txt"
#define BUFFSIZE 1024


typedef struct {
    double ***weight_matrices;
    double **bias_arrays;

} weight_and_bias_t; 



weight_and_bias_t* readWeightsAndBiases();



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

    char buffer[BUFFSIZE];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

        printf("%s", buffer);
        if (++count == 3) {
            break;
        }
    }
    fclose(fp);

    return wab;
}

