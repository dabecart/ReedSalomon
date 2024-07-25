/***************************************************************************************************
 * @file main.c
 * @brief Launch point for the program.
 *
 * @version   1.0
 * @date      2024-07-23
 * @author    @dabecart
 *
 * @license
 * This project is licensed under the MIT License - see the LICENSE file for details.
 **************************************************************************************************/

#include "SimulationTools.h"
#include "FileTools.h"

/***************************************************************************************************
 * MAIN
 **************************************************************************************************/
int main(){
    // Runs 100000 random cases with up to EXTRA_POINTS errors and prints the results of the algo.
    testBench(10000, 1, EXTRA_POINTS);
}