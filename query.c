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

/*********************************** Macros ***********************************/
#define TEST (0)

#if TEST == 0
#define TOTAL_DATA_NUM  (8)
#define NUM_WAYS        (2)
#define RUN_SIZE        (4)
#define BLOCK_SIZE      (2)
#else
#define TOTAL_DATA_NUM  (1048576)
#define NUM_WAYS        (128)
#define RUN_SIZE        (8129)
#define BLOCK_SIZE      (64)
#endif

#define UNINT_MAX       (0xFFFFFFFF)


/*********************************** Structures ***********************************/

/* Structure for a min heap node */
typedef struct MinHeapNode
{
    unsigned int element;       // The element to be stored
    int i;                      // index of the array from which the element is taken
    int j;                      // index of the next element to be picked from array
    unsigned int file_read_num; //number of reads from the data file
}MinHeapNode_t;

/* Structure for a min heap */
typedef struct MinHeap
{
    MinHeapNode_t *harr_n;        // pointer to an array of min heap nodes
    unsigned int heap_size;     // the size of the heap
}MinHeap_t;

/*********************************** Function Prototypes ***********************************/

/* @Function Name: externalSort
 * @Inputs:
 *  - input_file: name of the input data file to sort
 *  - output_file: name of the output file
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
void externalSort(char * input_file, char * output_file, unsigned int num_ways, unsigned int run_size);

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

unsigned int minHeap_getLeft(unsigned int node_index);
unsigned int MinHeap_getRight(unsigned int node_index);
MinHeapNode_t MinHeap_getMin(MinHeap_t * harr);
void MinHeap_heapify(MinHeap_t *harr, unsigned node_index);
void swapMinHeapNode(MinHeapNode_t * x, MinHeapNode_t * y);
void MinHeap_replaceMin(MinHeap_t * harr, MinHeapNode_t x);
MinHeap_t MinHeap_constructHeap(MinHeapNode_t * a, unsigned int size);


int main(int argc, char ** argv)
{
    char sorted_file[] = "sorted.bin";
    
    if (argc == 1) {printf("ERROR: no input file.\n"); exit(-1);}
    
    /* Read user-input testcases */
    
    
    /* Sorting the input data file in ascending order and write to the 'sorted_file' */
    externalSort(*(argv+1), sorted_file, NUM_WAYS, RUN_SIZE);
    
    /* Find matching number for each testcase */
    //result = findMatch();
    
    return 0;
}

void externalSort(char * input_file, char * output_file, unsigned int num_ways, unsigned int run_size)
{
    FILE * pdata_file, * ptemp_file;
    unsigned int n, count;
    size_t result;
    unsigned int buffer[BLOCK_SIZE*(num_ways+1)];
    char temp_filename[11];
    MinHeapNode_t harr_n[num_ways], root;
    MinHeap_t mp;
    unsigned int * output;
    
    /* Open the input file */
    pdata_file = fopen(input_file, "rb");
    if (!pdata_file) {fputs ("File error", stderr); exit(1);}
    
    /* 1st Pass: Sort 'num_ways' runs and store to disk */
    for (n=0; n<num_ways; n++)
    {
        // Read 'run_size' of data from the file
        result = fread(buffer, sizeof(unsigned int), run_size, pdata_file);
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
    
    /* 2nd Pass: Merge the sorted 'num_ways' files into a single file in ascending order */
    
    // Read a block data from all temp data files into a corresponding array
    for (n=0; n<num_ways; n++)
    {
        sprintf(temp_filename, "temp%d.bin", n);
        ptemp_file = fopen(temp_filename, "rb");
        if(!ptemp_file) {fputs ("Write File error", stderr); exit(1);}
        result = fread(buffer+(n*BLOCK_SIZE), sizeof(unsigned int), BLOCK_SIZE, ptemp_file);
        fclose(ptemp_file);
    }
    
    // Use the last block of the buffer as the output buffer
    output = buffer + BLOCK_SIZE * num_ways;
    
    // Construct a min heap using the first elements of all buffers
    for (n=0; n<num_ways; n++)
    {
        harr_n[n].element = *(buffer+n*BLOCK_SIZE);
        harr_n[n].i = n;
        harr_n[n].j = 1;
        harr_n[n].file_read_num = 1;
    }
    mp = MinHeap_constructHeap(harr_n, num_ways);
    
    // Open the output file for writes
    pdata_file = fopen(output_file, "ab");
    if(!pdata_file) {fputs ("Write File error", stderr); exit(1);}
    
    for (n=0; n<(TOTAL_DATA_NUM/BLOCK_SIZE); n++)
    {
        for (count=0; count<BLOCK_SIZE; count++)
        {
            // Get the minimum element in the min heap and store it in output
            root = MinHeap_getMin(&mp);
            output[count] = root.element;
            
            // Find the next element that will replace the current root of the heap.
            // The next element belongs to same buffer as the current root.
            // If root was the last element of its buffer, fill the buffer with new
            // data from the corresponding data file.
            // If the data file doesn't have new data, assign UNINT_MAX to the next element
            if(root.j < BLOCK_SIZE)
            {
                root.element = *(buffer+root.i*BLOCK_SIZE + root.j);
                root.j += 1;
            }
            else if(root.file_read_num < RUN_SIZE/BLOCK_SIZE)
            {
                sprintf(temp_filename, "temp%d.bin", root.i);
                ptemp_file = fopen(temp_filename, "rb");
                if(!ptemp_file) {fputs ("Write File error", stderr); exit(1);}
                result = fseek(ptemp_file, BLOCK_SIZE*root.file_read_num*sizeof(unsigned int), SEEK_SET);
                if(result !=0) {printf("file No.%d seek error --- read No.%d\n", root.i, root.file_read_num); exit(1);}
                result = fread(buffer+root.i*BLOCK_SIZE, sizeof(unsigned int), BLOCK_SIZE, ptemp_file);
                if (result != BLOCK_SIZE) {fputs("Reading error", stderr); exit(3);}
                fclose(ptemp_file);
                root.element = *(buffer+root.i*BLOCK_SIZE);
                root.j = 1;
                root.file_read_num += 1;
            }
            else
            {
                root.element = UNINT_MAX;
            }
            // Replace root with the next element of its buffer
            MinHeap_replaceMin(&mp, root);
        }
        // Write(append) to the output file when the output buffer is full
        fwrite(output, sizeof(unsigned int), BLOCK_SIZE, pdata_file);
    }
    // Close the file
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

/***********************/
unsigned int minHeap_getLeft(unsigned int node_index)
{
    return (2*node_index+1);
}

unsigned int MinHeap_getRight(unsigned int node_index)
{
    return (2*node_index+2);
}

MinHeapNode_t MinHeap_getMin(MinHeap_t * mp)
{
    return mp->harr_n[0];
}

void MinHeap_heapify(MinHeap_t * mp, unsigned node_index)
{
    unsigned int left = minHeap_getLeft(node_index);
    unsigned int right = MinHeap_getRight(node_index);
    unsigned int smallest = node_index;
    if ((left < mp->heap_size) && (mp->harr_n[left].element < mp->harr_n[node_index].element))
        smallest = left;
    if ((right < mp->heap_size) && (mp->harr_n[right].element < mp->harr_n[smallest].element))
        smallest = right;
    if (smallest != node_index)
    {
        swapMinHeapNode(&mp->harr_n[node_index], &mp->harr_n[smallest]);
        MinHeap_heapify(mp, smallest);
    }
}

void swapMinHeapNode(MinHeapNode_t * x, MinHeapNode_t * y)
{
    MinHeapNode_t temp = *x;
    *x = *y;
    *y = temp;
}

void MinHeap_replaceMin(MinHeap_t * mp, MinHeapNode_t x)
{
    mp->harr_n[0] = x;
    MinHeap_heapify(mp, 0);
}

MinHeap_t MinHeap_constructHeap(MinHeapNode_t * a, unsigned int size)
{
    MinHeap_t mp;
    int i;
    mp.heap_size = size;
    mp.harr_n = a;
    i = (mp.heap_size-1)/2;
    while(i >= 0)
    {
        MinHeap_heapify(&mp, i);
        i--;
    }
    return mp;
}
