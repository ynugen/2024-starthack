/*
 * NEURAL - Interface of module neural network
 * 
 * Implementation of network struct adapted from https://github.com/codeplea/genann
 * 
 */

#ifndef NEURAL_H
#define NEURAL_H

#include <stdio.h>

#define MAX_LAYERS 7
#define INPUT_DIMENSION 225
#define HIDDEN_DIMENSION_1 98
#define HIDDEN_DIMENSION_2 65
#define HIDDEN_DIMENSION_3 50
#define HIDDEN_DIMENSION_4 30
#define HIDDEN_DIMENSION_5 25
#define HIDDEN_DIMENSION_6 40
#define OUTPUT_DIMENSION 52

struct nnetwork;

/* Activation function */
typedef double (*actfun)(const struct nnetwork* nn, double a);

/* Describes architecture of neural network*/
typedef struct nnetwork {
    /* Number of input & output units, hidden layers, and hidden neurons */
    int input, hidden_layers, hidden, output;

    /* Activation function between input and hidden layers. Default: ReLU */
    actfun activation_hidden;

    /* Activation function used for output layer. Default: Sigmoid */
    actfun activation_output;

    /* Total number of weights */
    int total_weights;

    /* Total number of bias */
    int total_biases;

    /* Total number of neurons */
    int total_neurons;

    /* 2D array of weights for each layer */
    double **weight;

    /* 2D array of biases for each layer */
    double **biases;

    /* Input and output array for each neuron*/
    double *output;
} nnetwork_t;

/* Initialise a neural network given file specifying weights and biases */


#endif