#include "neural.h"
#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv) {

    /* Open file to read */
    FILE *f = fopen("weights_and_biases.txt", "r");
    assert(f);
    
    nnetwork_t *nn = nnetwork_read(f);
    fclose(f);
    
    nnetwork_print(nn);
    nnetwork_free(nn);
}