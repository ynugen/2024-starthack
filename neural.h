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

#define MAX_LINE_ELEMENTS 225

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

    /* 2D array of weights for each layer */
    double **weights;

    /* 2D array of biases for each layer */
    double **biases;

    /* Output array for each neuron */
    double *outputs;
} nnetwork_t;

/* ReLU function */
double relu(nnetwork_t *nn, double a);

/* Softmax function */
double softmax(nnetwork_t *nn, double *a);

/* Initialise memory for new neural network */
nnetwork_t *nnetwork_init();

/* Initialise a neural network given file specifying weights and biases */
nnetwork_t *nnetwork_read(FILE *weights_and_biases);

/* Free memory used by a neural network */
void nnetwork_free(nnetwork_t* nn);

#endif