/*
 * NEURAL - Interface of module neural network
 * 
 * Implementation of network adapted from https://github.com/codeplea/genann
 * 
 */

#ifndef NEURAL_H
#define NEURAL_H

#include <stdio.h>

#define HIDDEN_LAYERS 6
#define INPUT_DIMENSION 225
#define HIDDEN_DIMENSION_1 98
#define HIDDEN_DIMENSION_2 65
#define HIDDEN_DIMENSION_3 50
#define HIDDEN_DIMENSION_4 30
#define HIDDEN_DIMENSION_5 25
#define HIDDEN_DIMENSION_6 40
#define OUTPUT_DIMENSION 52

#define MAX_LINE_ELEMENTS 22050

/* Lookup table to convert probability to character */
static const char table[OUTPUT_DIMENSION] = {
    'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e', 'F', 'f', 'G', 'g', 'H', 'h',
    'I', 'i', 'J', 'j', 'K', 'k', 'L', 'l', 'M', 'm', 'N', 'n', 'O', 'o', 'P', 'p',
    'Q', 'q', 'R', 'r', 'S', 's', 'T', 't', 'U', 'u', 'V', 'v', 'W', 'w', 'X', 'x',
    'Y', 'y', 'Z', 'z'
};

struct nnetwork;

/* Activation functions */
typedef double (*hidden_actfun)(const struct nnetwork* nn, double a);
typedef int (*output_actfun)(const struct nnetwork* nn, double *a);

/* Describes architecture of neural network*/
typedef struct nnetwork {
    /* Number of input & output units, hidden layers, and hidden neurons */
    int input, hidden_layers, hidden_neurons, output;

    /* Activation function between input and hidden layers. Default: relu */
    hidden_actfun activation_hidden;

    /* Activation function used for output layer. Default: softmax */
    output_actfun activation_output;

    /* Total number of weights */
    int total_weights;

    /* Total number of bias */
    int total_biases;

    /* Total number of neurons */
    int total_neurons;

    /* Flattened array of weights for each layer */
    double *weights;

    /* Flattened array of biases for each layer */
    double *biases;

    /* Output array for input values and output neuron */
    double *outputs;
} nnetwork_t;

/* ReLU function */
double relu(const nnetwork_t *nn, double a);

/* Softmax function */
int softmax(const nnetwork_t *nn, double *a);

/* Initialise memory for new neural network */
nnetwork_t *nnetwork_init();

/* Initialise a neural network given file specifying weights and biases */
nnetwork_t *nnetwork_read(FILE *weights_and_biases);

/* Free memory used by a neural network */
void nnetwork_free(nnetwork_t* nn);

/* Print neural network. Used for error checking in storing values */
void nnetwork_print(nnetwork_t *nn);

/* Run the neural network with a given tensor.
 * Returns argmax of outputs.
 */
int nnetwork_run(nnetwork_t *nn, FILE *tensor);

/* Return char value from lookup table */
char lookup(int value);

#endif