/**************
*Author: Lachlan Sinclair
*Date: 6/26/2019
*Description: This program generates an array of size N filled with random numbers
*and then sorts them, then outputs the time in nanoseconds.
*insert.txt
****************/

#include <iostream>
#include <fstream>
#include <stdio.h>     
#include <stdlib.h>     
#include <time.h> 
#include <ctime>

using namespace std;

//prototypes
void mergeSort(int arr[], int size);
void merge(int first[], int second[], int lower, int middle, int upper);
void mergeSortHelper(int first[], int second[], int lower, int upper);

const int N = 50000;

/****************
* Function: mergeSort
* Description: copies the given array, then passes the orginal array address, and the new array address
* to the recursive mergesort function
* Input: Address of array, size of the array
* Output: none
******************/
void mergeSort(int arr[], int size)
{
	//malloc a new array to allow for merging/spliting into one another
	int *temp = (int*)malloc(sizeof(int)*size);
	//copy the orginal array into the temp array
	for (int i = 0; i < size; i++)
	{
		temp[i] = arr[i];
	}
	//call the helper function
	mergeSortHelper(arr, temp, 0, size);
	//free the temporary array
	free(temp);
	temp = 0;
}

/****************
* Function: merge
* Description: Merges the two halfs of the given range into sorted order
* Input: Addresses of the two array's, the lower/middle/upper indexes of the given range
* Output: none
******************/
void merge(int first[], int second[], int lower, int middle, int upper)
{
	//temp variables to allow for incrementing through the two halfs of the array subset
	int x = lower;
	int y = middle;

	//loop through the indicies of the given subset
	for (int index = lower; index < upper; index++)
	{
		//if there is still variables left in the upper half, and the lower half is empty or it is less than the value in
		//the lower half, add the value from the upper half to the current position in the subset
		if (y < upper && (x >= middle || second[y] <= second[x]))
		{
			first[index] = second[y];
			y++;
		}
		//else add the next value from the lower half
		else
		{
			first[index] = second[x];
			x++;
		}
	}
}

/****************
* Function: mergeSortHelper
* Description: The recursive mergesort function. Divides the array into halfs, until it is aof size one, it aleternates splitting and merging of the array
* into the two provided
* Input: Address of two arrays
* Output: none
******************/
void mergeSortHelper(int first[], int second[], int lower, int upper)
{
	if (upper - lower < 2)
	{
		return;
	}
	int middle = (upper + lower) / 2;
	//alternate which array is getting merged to set up for merging the two halfs back together
	//start by spliting the first array into the second array(this insures merging occurs correctly)
	mergeSortHelper(second, first, lower, middle);
	mergeSortHelper(second, first, middle, upper);
	//merge the two sorted halfs together, the final call combined's two halfs from the temp array to the oringal array
	merge(first, second, lower, middle, upper);
}



int main()
{

	int arr[N];

	timespec time1, time2, differ;
	int temp;

	srand(time(NULL));

	//create the array of random numbers
	for (int i = 0; i < N; i++)
	{
		arr[i] = rand() % 10000;
	}
	
	//http://man7.org/linux/man-pages/man2/clock_gettime.2.html
	//set the feilds of the first timespec
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	mergeSort(arr, N);

	//set the feilds of the second timespec
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	//calculate the difference of the seonds and nanoseconds feilds
	differ.tv_nsec = time2.tv_nsec - time1.tv_nsec;
	differ.tv_sec = time2.tv_sec - time1.tv_sec;

	//output the results
	cout << "N: " << N << endl;
	//cout << differ.tv_sec << endl;
	cout << "Time in nanoseconds: " << ((differ.tv_sec * 1000000000) + differ.tv_nsec) << endl;

	return 0;
}
