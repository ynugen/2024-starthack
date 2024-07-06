#include "neural.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

/* ReLU function 
 *
 * Parameters:
 *     * nn : pointer to neural network struct
 *        a : input value
 */
double relu(const nnetwork_t *nn, double a) {
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
int softmax(const nnetwork_t *nn, double *a) {
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
    for (int i = 0; i < OUTPUT_DIMENSION; ++i) {
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
    
    while (getline(&line, &line_size, weights_and_biases) > 0) {
        /* Calculate line size */
        line[strcspn(line, "\n")] = '\0';

        /* Skip if line is a header. */
        if (strstr(line, "weight:") != NULL) {
            weight_read = 1;
            bias_read = 0;
            continue;
        } else if(strstr(line, "bias:") != NULL) {
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
                printf("Elements: %d\n", i);
                perror("nnetwork_read: Line too long");
                exit(EXIT_FAILURE);
            }

            /* Reset errno value */
            errno = 0;
            char *end_ptr;

            /* Convert str to double */
            double value = strtod(token, &end_ptr);
            if (errno || *end_ptr != '\0' || end_ptr == token) {
                fprintf(stderr, "nnetwork_read: Read error with weight or bias value: %s\n", token);
                exit(EXIT_FAILURE);
            }
            ptr = NULL;
            
            // if (weight_read && curr_weight_ptr < nn->weights + nn->total_weights) {
            //     *curr_weight_ptr = (double) value;
            //     curr_weight_ptr++;
            // } else if (bias_read && curr_bias_ptr < nn->biases + nn->total_biases) {
            //     *curr_bias_ptr = (double) value;
            //     curr_bias_ptr++;
            if(weight_read) {
                *curr_weight_ptr = (double) value;
                curr_weight_ptr++;
            } else if (bias_read) {
                *curr_bias_ptr = (double) value;
                curr_bias_ptr++;
            } else {
                printf("ITEM: %d WEIGHT READ: %d BIAS READ: %d\nCURR_WEIGHT_PTR: %p\nWEIGHTS + TOTAL WEIGHTS: %p\n", i, weight_read, bias_read, curr_weight_ptr, nn->weights + nn->total_weights);
                perror("nnetwork_read: Error in storing value");
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

/* Print neural network to file. Used for error checking in storing values */
void nnetwork_print(nnetwork_t *nn) {
    FILE *f;
    f = fopen("test.txt", "w");
    for (int i = 0; i <nn->total_weights; ++i) {
        /* Print headers */
        if (i == 0) {
            fprintf(f,"LAYER 1 WEIGHTS:\n");
        } else if (i == INPUT_DIMENSION *HIDDEN_DIMENSION_1) {
            fprintf(f,"\n");
            //return;
            fprintf(f,"LAYER 2 WEIGHTS:\n");
        } else if (i == HIDDEN_DIMENSION_1 * HIDDEN_DIMENSION_2) {
            fprintf(f,"\n");
            fprintf(f,"LAYER 3 WEIGHTS:\n");
        } else if (i == HIDDEN_DIMENSION_2 * HIDDEN_DIMENSION_3) {
            fprintf(f,"\n");
            fprintf(f,"LAYER 4 WEIGHTS:\n");
        } else if (i == HIDDEN_DIMENSION_3 * HIDDEN_DIMENSION_4) {
            fprintf(f,"\n");
            fprintf(f,"LAYER 5 WEIGHTS:\n");
        } else if (i == HIDDEN_DIMENSION_4 * HIDDEN_DIMENSION_5) {
            fprintf(f,"\n");
            fprintf(f,"LAYER 6 WEIGHTS:\n");
        } else if (i == HIDDEN_DIMENSION_5 * HIDDEN_DIMENSION_6) {
            fprintf(f,"\n");
            fprintf(f, "LAYER 7 WEIGHTS:\n");
        } else {
            fprintf(f, ",");
        }
        fprintf(f, "%1.19g", nn->weights[i]);
    }
    fprintf(f,"\n");

    for (int i = 0; i <nn->total_biases; ++i) {
        /* Print headers */
        if (i == 0) {
            fprintf(f,"LAYER 1 BIAS:\n");
        } else if (i == HIDDEN_DIMENSION_1) {
            fprintf(f,"\n");
            //break;
            fprintf(f,"LAYER 2 BIAS:\n");
        } else if (i == HIDDEN_DIMENSION_2) {
            fprintf(f,"LAYER 3 BIAS:\n");
        } else if (i == HIDDEN_DIMENSION_3) {
            fprintf(f,"LAYER 4 BIAS:\n");
        } else if (i == HIDDEN_DIMENSION_4) {
            fprintf(f,"LAYER 5 BIAS:\n");
        } else if (i == HIDDEN_DIMENSION_5) {
            fprintf(f,"LAYER 6 BIAS:\n");
        } else if (i == HIDDEN_DIMENSION_6) {
            fprintf(f,"LAYER 7 BIAS:\n");
        } else {
            fprintf(f, ",");
        }
        fprintf(f,"%g", nn->biases[i]);
    }
    fprintf(f,"\n");
    fclose(f);
}

/* Run the neural network with a given tensor */
int nnetwork_run(nnetwork_t *nn, FILE *tensor) {

    /* Allocate mem for line buffer */ 
    char *line = NULL;
    size_t line_size = 0;

    /* Allocate mem for input buffer */
    double *inputs = (double *)malloc(sizeof(double) * INPUT_DIMENSION);
    if (!inputs) {
        perror("nnetwork_run: MALLOC error");
        exit(EXIT_FAILURE);
    }

    /* Read in tensor to input buffer */
    while(getline(&line, &line_size, tensor) > 0) {
        line[strcspn(line, "\n")] = '\0'; // removing newline character
        
        char *token;
        char *ptr = line;
        int i = 0;
        while((token = strtok(ptr, ",")) != NULL) {
            if (i >= INPUT_DIMENSION) {
                perror("nnetwork_run: Too many elements");
                exit(EXIT_FAILURE);
            }

            errno = 0;
            char *end_ptr;

            /* Read and convert value to double */
            double value = strtod(token, &end_ptr);

            if(errno || *end_ptr != '\0' || end_ptr == token) {
                fprintf(stderr, "nnetwork_run: Read error with tensor value");
                exit(EXIT_FAILURE);
            }

            /* Store value in input buffer and increment */
            inputs[i] = value;
            i++;

            ptr = NULL;
        }
    }

    /* Store inputs in the respective neurons of output */
    memcpy(nn->outputs, inputs, sizeof(double) * nn->input);

     /* Compute the forward pass */ 
    double *in = nn->outputs;
    double *out = nn->outputs + nn->input;
    double *w = nn->weights;
    double *b = nn->biases;

    /* Process each hidden layer */ 
    int layer_sizes[HIDDEN_LAYERS] = {
        HIDDEN_DIMENSION_1,
        HIDDEN_DIMENSION_2,
        HIDDEN_DIMENSION_3,
        HIDDEN_DIMENSION_4,
        HIDDEN_DIMENSION_5,
        HIDDEN_DIMENSION_6
    };
    
    for (int layer = 0; layer < HIDDEN_LAYERS; ++layer) {
        int in_neurons = (layer == 0) ? nn->input : layer_sizes[layer - 1];
        int out_neurons = layer_sizes[layer];
        
        for (int j = 0; j < out_neurons; ++j) {
            double sum = 0.0;
            for (int k = 0; k < in_neurons; ++k) {
                sum += in[k] * (*w++);
            }
            sum += *b++;
            /* Apply activation function */ 
            out[j] = nn->activation_hidden(nn, sum);
        }
        
        /* Move input pointer to the output of the current layer */
        in = out;
        out += out_neurons;
    }

    int in_neurons = layer_sizes[HIDDEN_LAYERS - 1]; // last layer!!!! :)
    int out_neurons = nn->output;

    for (int j = 0; j < out_neurons; ++j) {
        double sum = 0.0;
        for (int k = 0; k < in_neurons; ++k) {
            sum += in[k] * (*w++);
        }
        sum += *b++;
        nn->outputs[j] = sum; // store the raw output before softmax
    }

    /* Apply softmax to the output layer */ 
    int argmax = nn->activation_output(nn, nn->outputs);

    free(inputs);
    free(line);


    return argmax;
}

/* Return char value from lookup table */
char lookup(int value) {
    if (value >= OUTPUT_DIMENSION) {
        perror("lookup: value out of output bounds");
        exit(EXIT_FAILURE);
    }
    return table[value];
}

void print_weights_and_biases(nnetwork_t *nn) {
    printf("weights:\n");
    for (int i = 0; i < nn->total_weights; ++i) {
        printf("%f ", nn->weights[i]);
    }
    printf("\nbiases:\n");
    for (int i = 0; i < nn->total_biases; ++i) {
        printf("%f ", nn->biases[i]);
    }
    printf("\n");
}