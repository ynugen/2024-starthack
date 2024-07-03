#include "neural.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

/* ReLU function */
double relu(nnetwork_t *nn, double a) {
    assert(!isnan(a));

    if(a > 0) {
        return a;
    }
    return 0;
}

/* Softmax function */
int softmax(nnetwork_t *nn, double *a) {
    int argmax = 0;
    double max = 0.0;
    double exp_sum = 0.0;

    /* Find max value of ouput values */
    for (int i = 1; i < OUTPUT_DIMENSION; ++i) {
        if (a[i] > max) {
            max = a[i];
            argmax = i;
        }
    }

    /* Compute sum and exponentiation */
    for (int i = 0; i < OUTPUT_DIMENSION; ++i) {
        double output = exp(a[i] - max);
        nn->outputs[i] = output;
        exp_sum += output;
    }

    /* Normalise values */
    for (int i = 0; i < OUTPUT_DIMENSION) {
        nn->outputs[i] /= exp_sum;
    }

    return argmax;
}

/* Initialise memory for new neural network */
nnetwork_t *nnetwork_init() {
    
    /* Define neural network architecture given in the README */
    const int total_neurons = INPUT_DIMENSION + HIDDEN_DIMENSION_1 + HIDDEN_DIMENSION_2 +
                            HIDDEN_DIMENSION_3 + HIDDEN_DIMENSION_4 + HIDDEN_DIMENSION_5 +
                            HIDDEN_DIMENSION_6 + OUTPUT_DIMENSION;

    const int total_biases = HIDDEN_DIMENSION_1 + HIDDEN_DIMENSION_2 + HIDDEN_DIMENSION_3 +
                            HIDDEN_DIMENSION_4 + HIDDEN_DIMENSION_5 + HIDDEN_DIMENSION_6 +
                            OUTPUT_DIMENSION;

    const int total_weights = (INPUT_DIMENSION * HIDDEN_DIMENSION_1) + (HIDDEN_DIMENSION_1 * HIDDEN_DIMENSION_2) +
                            (HIDDEN_DIMENSION_2 * HIDDEN_DIMENSION_3) + (HIDDEN_DIMENSION_3 * HIDDEN_DIMENSION_4) +
                            (HIDDEN_DIMENSION_5 * HIDDEN_DIMENSION_6) + (HIDDEN_DIMENSION_6 * OUTPUT_DIMENSION);
    

    /* Allocate memory for nnetwork_t */
    const int size = sizeof(nnetwork_t) + sizeof(double) * (total_neurons + total_weights + total_biases);
    nnetwork_t *nn = malloc(size);
    if (!nn) {
        perror("nnetwork_init: MALLOC error");
        exit(EXIT_FAILURE);
    }

    nn->input = INPUT_DIMENSION;
    nn->hidden_layers = HIDDEN_LAYERS;
    nn->hidden_neurons = HIDDEN_DIMENSION_1 + HIDDEN_DIMENSION_2 + HIDDEN_DIMENSION_3 +
                        HIDDEN_DIMENSION_4 + HIDDEN_DIMENSION_5 + HIDDEN_DIMENSION_6;
    nn->output = OUTPUT_DIMENSION;

    nn->total_weights = total_weights;
    nn->total_biases = total_biases;
    nn->total_neurons = total_neurons;

    /* Set pointers */
    nn->weights = (double*)((char*)nn + sizeof(nnetwork_t));
    nn->biases = nn->weights + nn->total_weights;
    nn->outputs = nn->biases + nn->total_biases;

    nn->activation_hidden;
}

/* Initialise a neural network given file specifying weights and biases */
nnetwork_t *nnetwork_read(FILE *weights_and_biases);

/* Free memory used by a neural network */
void nnetwork_free(nnetwork_t* nn);