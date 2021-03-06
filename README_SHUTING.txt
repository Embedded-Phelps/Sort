Dear Sir,

For the program development process, please refer to this github repository:
https://github.com/Embedded-Phelps/Sort

Below is a little walk-through of how this program works to meet the requirements of the challenge.

1. to find matching numbers quickly enough with the memory constraint

The system only has 35KB RAM available, but the data file contain 2^20 integers which are too large to be all loaded into fast memory for processing. Also, the program would need to conduct relatively fast inquries when the number of testcase is as small as one, or is as large as 10000. Therefore I have developed the program to sort the data file in ascending order first. It then conduct binary search on the sorted data file to find the matched or closest numbers to the testcase numbers and print out the results.

2. Considerations of the memory usage
    a) when the number of testcases is too large, like 10000, storing them all on RAM would exceed the limitation. Thus a threshold is set so that when the number of testcases exceeds the threshold, the program would find the matches for the first 1/5 of the testcases and store the results in a temp file before taking the rest 4/5 of the testcases. In this case, the threshold is 8000. It guarrantees that the program would be able to process testcases as many as 10000, and at the same time, still makes good use of all the available fast memory. Please refer to code for more details.
    b) When doing the external sorting of the data file, the size of a RUN needs to be able to be put in the RAM, and still makes good use of the available RAM space. Please refer to code for more details.
    c) In the program, variables are reused as much as possible to save memory space, while the readibility of the program is still maintained.

Note:
Please delete the "sorted.bin" file generated by the program first every time the program is rerun.


Future Improvements:
One of the things that would greatly improve the speed of the program is to use muliple threads to off-load the sorting process. The C11 std library <thread.h> would be helpful in further improving endeavors.

Look forward to hearing back from you!

Shuting Guo


