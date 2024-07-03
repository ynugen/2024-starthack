#include "neural.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

/* ReLU function 
 *
 * Parameters:
 *     * nn : pointer to neural network struct
 *        a : input value
 */
double relu(nnetwork_t *nn, double a) {
    assert(!isnan(a));

    if(a > 0) {
        return a;
    }
    return 0;
}

/* Softmax function 
 *
 * Adapted from: https://en.wikipedia.org/wiki/Softmax_function
 */
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

    nn->activation_hidden = relu;
    nn->activation_output = softmax;

    return nn;
}

/* Build neural network given file specifying weights and biases 
 *
 * Adapted from: https://stackoverflow.com/questions/62409573/what-is-the-fastest-way-to-read-several-lines-of-data-from-a-large-file 
 */
nnetwork_t *nnetwork_read(FILE *weights_and_biases) {
    nnetwork_t *nn = nnetwork_init();
    char *line = NULL;
    size_t line_size = 0;

    /* Set pointers */
    double *curr_weight_ptr = nn->weights;
    double *curr_bias_ptr = nn->biases;

    /* Set read state flags */
    int weight_read = 0;
    int bias_read = 0;
    
    while (getline(&line, &line_size, weights_and_biases) >0) {
        /* Calculate line size */
        line[strcspn(line, "\n")] = '\0';

        /* Skip if line is a header. */
        if (strstr(line, "weight:") != NULL) {
            weight_read = 1;
            bias_read = 0;
            continue;
        } else if(strstr(line, "bias:" != NULL)) {
            weight_read = 0;
            bias_read = 1;
            continue;
        }

        /* Process line into network*/
        char *token;
        char *ptr = line;
        int i = 0;
        while ((token = strtok(ptr, ",")) != NULL) {
            if (i >= MAX_LINE_ELEMENTS) {
                perror("nnetwork_read: Line too long");
                exit(EXIT_FAILURE);
            }

            /* Reset errno value */
            errno = 0;
            char *end_ptr;

            /* Convert str to double */
            double value = strtod(token, &end_ptr);
            if (errno || *end_ptr != '\0' || end_ptr == token) {
                fprintf(stderr, "nnetwork_read: Read error with value: %s\n", token);
                exit(EXIT_FAILURE);
            }
            ptr = NULL;
            
            if (weight_read && curr_weight_ptr < nn->weights + nn->total_weights) {
                *curr_weight_ptr++ = (double) value;
                weight_read = 0;
            } else if (bias_read && curr_bias_ptr < nn->biases + nn->total_biases) {
                *curr_bias_ptr++ = (double) value;  
            } else {
                perror("nnetwork_read: Error in reading value");
                exit(EXIT_FAILURE);
            }
            i++;
        }
    }
    free(line);
    return nn;
}

/* Free memory used by a neural network */
void nnetwork_free(nnetwork_t* nn) {
    /* Memory in nn allocated to single contiguous block */
    free(nn);
}