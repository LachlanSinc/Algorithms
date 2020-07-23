/**************
*Author: Lachlan Sinclair
*Date: 6/26/2019
*Description: This program generates an array of size N filled with random numbers
*and then sorts them using stooge sort, then outputs the time in nanoseconds.
****************/

#include <iostream>
#include <fstream>
#include <stdio.h>     
#include <stdlib.h>     
#include <time.h> 
#include <ctime>

using namespace std;

const int N = 500;

//prototype
void stoogeSort(int[], int, int);
void swap(int[], int, int);

/****************
* Function: swap
* Description: swaps two elements in an array
* Input: Address of array, two indicies of values to be swapped
* Output: none
******************/
void swap(int arr[], int x, int y)
{
	//create a temp int to hold one of the values, then perform the swap
	int temp = arr[x];
	arr[x] = arr[y];
	arr[y] = temp;
}

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

	stoogeSort(arr, 0, N-1);

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

/****************
* Function: stoogesort
* Description: implements the stooge sort method to sort an array
* Input: Address of array, lower index, upper index
* Output: none
******************/
void stoogeSort(int arr[], int low, int high)
{
	//check for base case
	if (low >= high) {
		return;
	}
	//swap if needed
	if (arr[high] < arr[low]) {
		swap(arr, low, high);
	}
	//check for two or more elements left to sort
	if (high - low + 1 > 2) {
		int third = ((high - low + 1) / 3);
		//make recursive calls on lower two thirds, upper two thirds, then lower two thirds
		stoogeSort(arr, low, high - third);
		stoogeSort(arr, low + third, high);
		stoogeSort(arr, low, high - third);
	}
}