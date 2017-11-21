/***************************************************************************************
 * fileName:    query.c
 * description: this program takes a user-specified number of integer testcases
 *              in range [0, 2^25] and return the closest integers to the corresponding
 *              testcases from a specified binary input file. If there is a tie for the
 *              closest, return the lower one.
 * author:      Shuting Guo
 * Date:
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/* @Function Name: externalSort
 * @Inputs:
 *  - input_file: name of the input data file to sort
 *  - output_file: name of the sorted file
 *  - num_ways: number of runs to be merged
 *  - run_size: size of a run (can fit in RAM)
 * @Description:
 *  sort a binary data file in ascending order
 *  1) read input_file such that at most 'run_size' element are read at a time
 *     Do the following for each of the 'num_ways' runs:
 *         a) Sort the run using 'QuickSort' algorithm
 *         b) Store the sorted run in a temperary file on disk
 *  2) Merge the sorted files using 'K-way Merge' algorithm
 */
void externalSort(char * input_file, char * output_file,
             unsigned int num_ways, unsigned int run_size);

/* @Function Name: findMatch
 * @Inputs:
 *  - data_file: name of the data file to search from
 *  - testcase: the number to find match
 * @Return:
 *  - matched number
 * @Description:
 *  find the matched or cloest number to 'testcase' from the 'data_file'
 *  with 'binary search' algorithm
 */
unsigned int findMatch(char * data_file, unsigned int testcase);

int main(int argc, char ** argv)
{
    /* Read input file */
    
    
    /* Read user-input testcases */
    
    
    /* Sorting the data file in ascending order */
    externalSort();
    
    /* Find matching number for each testcase */
    result = findMatch();
    
    return 0;
}
