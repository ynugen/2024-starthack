#include "neural.h"
#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv) {

    /* Check arg inputs */
    // for (int i = 0; i <argc; ++i) {
    //     printf("%d: %s\n", i, argv[i]);
    // }

    /* Open weights and biases file to read */
    FILE *f = fopen("weights_and_biases.txt", "r");
    assert(f);
    
    nnetwork_t *nn = nnetwork_read(f);
    fclose(f);

    /* Open tensor file to read */
    FILE *t = fopen("tensors/01out.txt","r");
    assert(t);

    nnetwork_run(nn, t);

    fclose(t);
    
    nnetwork_print(nn);
    nnetwork_free(nn);
    printf("HELLO\n");
    return 0;
}