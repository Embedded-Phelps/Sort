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
 *  - buffer: data sorting buffer
 * @Description:
 *  sort a binary data file in ascending order
 *  1) read input_file such that at most 'run_size' element are read at a time
 *     Do the following for each of the 'num_ways' runs:
 *         a) Sort the run using 'QuickSort' algorithm
 *         b) Store the sorted run in a temperary file on disk
 *  2) Merge the sorted files using 'K-way Merge' algorithm
 */
void externalSort(char * input_file, char * output_file,
             unsigned int num_ways, unsigned int run_size, unsigned int * buffer);

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


void swapByte(unsigned char * xp, unsigned char * yp);
void swapInt(unsigned int * xp, unsigned int * yp);
void convertEndianness(unsigned int * data_buf, unsigned int num);
void quickSort(unsigned int *arr, int low, int high);
int partition(unsigned int *arr, int low, int high);

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

void externalSort(char * input_file, char * output_file,
                  unsigned int num_ways, unsigned int run_size, unsigned int * buffer)
{
    FILE * pdata_file, * ptemp_file;
    unsigned int n;
    size_t result;
    char temp_filename[10];
    
    /* Open the input file */
    pdata_file = fopen(input_file, "rb");
    if (!pDataFile) {fputs ("File error", stderr); exit(1);}
    
    /* 1st Pass: Sort 'num_ways' runs and store to disk */
    for (n=0; n<num_ways;t++)
    {
        // Read 'run_size' of data from the file
        result = fread(&buffer, sizeof(unsigned int), run_size, pdata_file);
        if (result != run_size) {fputs("Reading error", stderr); exit(3);}
        
        // Convert the data to little-endian
        convertEndianness(buffer, run_size);
        
        // Sort the data in ascending order
        quickSort(buffer, 0, run_size-1);
        
        // Write the data to a new file
        sprintf(temp_filename, "temp%d.bin", n);
        ptemp_file = fopen(temp_filename, "w");
        if(!ptemp_file) {fputs ("Write File error", stderr); exit(1);}
        fwrite(buffer, sizeof(unsigned int), run_size, ptemp_file);
        fclose(ptemp_file);
    }
    // Close the input file
    fclose(pdata_file);
    
}

void convertEndianness(unsigned int * data_buf, unsigned int num)
{
    unsigned int i;
    unsigned char *temp;
    if ((data_buf == NULL) || (num == 0))
        return;
    for(i=0;i<num;i++)
    {
        temp = (unsigned char *)(data_buf + i);
        swapByte(temp, temp+3);
        swapByte(temp+1, temp+2);
    }
}

void quickSort(unsigned int *arr, int low, int high)
{
    int pi;
    if(low < high)
    {
        pi = partition(arr, low, high);
        quickSort(arr, low, pi-1);
        quickSort(arr, pi+1, high);
    }
}

int partition(unsigned int *arr, int low, int high)
{
    int pivot = *(arr+high);
    int i = low - 1;
    for(int j = low; j < high; j++)
    {
        if(*(arr+j) <= pivot)
        {
            i++;
            swapInt(arr+i, arr+j);
        }
    }
    swapInt(arr+i+1, arr+high);
    return (i+1);
}

void swapByte(unsigned char * xp, unsigned char * yp)
{
    if(xp == yp)
        return;
    *xp = *xp ^ *yp;
    *yp = *xp ^ *yp;
    *xp = *xp ^ *yp;
}

void swapInt(unsigned int * xp, unsigned int * yp)
{
    if(xp == yp)
        return;
    *xp = *xp ^ *yp;
    *yp = *xp ^ *yp;
    *xp = *xp ^ *yp;
}
