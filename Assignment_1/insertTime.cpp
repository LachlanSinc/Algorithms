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

const int N = 50000;

//prototype
void insertionSort(int[], int);

int main()
{

	int arr[N];
	timespec time1, time2, differ;
	int temp;
	srand(time(NULL));

	//generate the array of random numbers
	for (int i = 0; i < N; i++)
	{
		arr[i] = rand() % 10000;
	}


	//http://man7.org/linux/man-pages/man2/clock_gettime.2.html
	//set the feilds of the first timespec
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	insertionSort(arr, N);
	
	//set the feilds of the second timespec
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	//calculate the difference of the seonds and nanoseconds feilds
	differ.tv_nsec = time2.tv_nsec - time1.tv_nsec;
	differ.tv_sec = time2.tv_sec - time1.tv_sec;

	//output the results
	cout << "N: "<< N << endl;
	//cout << differ.tv_sec << endl;
	cout << "Time in nanoseconds: " << ((differ.tv_sec*1000000000)+differ.tv_nsec) << endl;

	return 0;
}

/****************
* Function: insertion sort
* Description: implements the insertion sort method to sort an array
* Input: Address of array, the size of the array
* Output: none
******************/
void insertionSort(int arr[], int size)
{
	for (int i = 1; i < size; i++)
	{
		int val = arr[i];
		int index = i - 1;
		while (index >= 0 && val < arr[index])
		{
			arr[index + 1] = arr[index];
			index-- ;
		}
		arr[index + 1] = val;
	}
}