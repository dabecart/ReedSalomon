/***************************************************************************************************
 * @file ReedSolomon.h
 * @brief The bulk of the Reed Solomon Algorithm.
 *
 * @version   1.0
 * @date      2024-07-23
 * @author    @dabecart
 *
 * @license
 * This project is licensed under the MIT License - see the LICENSE file for details.
 **************************************************************************************************/

#ifndef REED_Solomon_h
#define REED_Solomon_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CommonDefines.h"

// You can specify the type of the numbers used by the algorithm. This will be related to MODULUS as
// the type you select has to be able to represent at least (MODULUS-1)^2. There are no negative 
// values, so unsigned values can be used.
typedef unsigned short ModInt;

/***************************************************************************************************
 * ALGORITHM SELECTION
 **************************************************************************************************/
// Different algorithms to calculate the module of a fraction.
// If MOD_USE_ARRAY is selected, be sure to modify nResults in modFrac(). You can use Wolfram 
// Mathematica to get the values:
// In[1] := Table[ModularInverse[x, MODULE], {x, 1, MODULE-1}]

// #define MOD_USE_NAIVE
// #define MOD_USE_EUCLID
#define MOD_USE_ARRAY       

/***************************************************************************************************
 * FUNCTIONS
 **************************************************************************************************/
AlgorithmReturn verifyMessage(int* rx, int* ry, int len, int pointsPerLagrange);

void addErrorCorrectionFields(int* x, int* y, int numPoints, int* xx, int* yy);

#endif