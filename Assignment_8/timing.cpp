/*********
* Author: Lachlan Sinclair
* Date: 8/10/2019
* Description:
***********/

#include <fstream>
#include <stdio.h>     
#include <stdlib.h>  
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

void mergeSort(int arr[], int size);
void merge(int first[], int second[], int lower, int middle, int upper);
void mergeSortHelper(int first[], int second[], int lower, int upper);

int const A = 50;


struct bin {
	int capacity;
	int currentWeight;
};

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
		if (y < upper && (x >= middle || second[y] >= second[x]))
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
* into the two provided arrays.
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

int doubleArr(struct bin* bins[], int numOfBins, int binCapacity)
{
	int newSize = 2 * numOfBins;
	struct bin *newArr = (struct bin *)malloc(sizeof(struct bin) * newSize);

	for (int i = 0; i < newSize; i++)
	{
		newArr[i].capacity = binCapacity;

		if (i < numOfBins)
		{
			newArr[i].currentWeight = (*bins)[i].currentWeight;
		}
		else
		{
			newArr[i].currentWeight = 0;
		}
	}

	free(*bins);
	*bins = newArr;
	newArr = NULL;

	return newSize;

}


int firstFit(int arr[], int numOfItems, int binCapacity)
{

	struct bin * bins;
	int numOfBins = 10;

	int openBins = 1;
	bins = (struct bin *)malloc(sizeof(struct bin) * 10);

	for (int i = 0; i < 10; i++)
	{
		bins[i].capacity = binCapacity;
		bins[i].currentWeight = 0;
	}


	for (int i = 0; i < numOfItems; i++)
	{
		for (int j = 0; j < openBins; j++)
		{
			if (arr[i] + bins[j].currentWeight <= bins[j].capacity)
			{
				bins[j].currentWeight = arr[i] + bins[j].currentWeight;
				break;
			}
			//check to see if a new bins needs  to be open
			else if (j == openBins - 1)
			{
				if (openBins == numOfBins)
				{
					//double size
					numOfBins = doubleArr(&bins, numOfBins, binCapacity);

				}
				openBins++;
				//bins[j + 1].currentWeight = arr[i] + bins[j + 1].currentWeight;
			}
		}
	}

	free(bins);
	bins = NULL;
	return openBins;

}

int bestFit(int arr[], int numOfItems, int binCapacity)
{

	struct bin * bins;
	int numOfBins = 10;

	int openBins = 1;
	bins = (struct bin *)malloc(sizeof(struct bin) * 10);

	for (int i = 0; i < 10; i++)
	{
		bins[i].capacity = binCapacity;
		bins[i].currentWeight = 0;
	}

	int bestBin = -1;
	int bestCap = 0;

	for (int i = 0; i < numOfItems; i++)
	{
		for (int j = 0; j < openBins; j++)
		{
			if (arr[i] + bins[j].currentWeight <= bins[j].capacity)
			{
				if (arr[i] + bins[j].currentWeight > bestCap)
				{
					bestCap = arr[i] + bins[j].currentWeight;
					bestBin = j;
				}
			}
		}
		//check to see if a new bins needs  to be open
		if (bestBin == -1)
		{
			if (openBins == numOfBins)
			{
				//double size
				numOfBins = doubleArr(&bins, numOfBins, binCapacity);

			}
			openBins++;

			bins[openBins - 1].currentWeight = arr[i];
		}
		else
		{
			bins[bestBin].currentWeight = bestCap;
		}
		bestBin = -1;
		bestCap = 0;
	}


	free(bins);
	bins = NULL;
	return openBins;

}

int main() {


	srand(time(NULL));

	int numOfCases;
	int binCapacity;
	int numOfItems;

	timespec ffTime1, ffTime2, ffDiffer;
	timespec ffdTime1, ffdTime2, ffdDiffer;
	timespec bfTime1, bfTime2, bfDiffer;

	int firstFitBins, sortedFirst, numBestFit;

	numOfCases = A;
	int *capacities = (int *)malloc(sizeof(int) * A);
	int *itemAmount = (int *)malloc(sizeof(int) * A);
	int ** items = (int **)malloc(sizeof(int*) * A);
	
	for (int j = 0; j < A; j++)
	{	
		capacities[j] = (rand() % 100) + 5;
		itemAmount[j] = (rand() % 100) + 5;
		items[j] = (int *)malloc(sizeof(int) * itemAmount[j]);

		for (int i = 0; i < itemAmount[j]; i++)
		{
			items[j][i] = (rand() % capacities[j]) + 1;
		}

	}

	ofstream outFile("out.txt");


	for (int i = 0; i < numOfCases; i++)
	{
		binCapacity = capacities[i];
		numOfItems = itemAmount[i];

		int * curItems = items[i];
		
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ffTime1);
		firstFitBins = firstFit(curItems, numOfItems, binCapacity);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ffTime2);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &bfTime1);
		numBestFit = bestFit(curItems, numOfItems, binCapacity);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &bfTime2);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ffdTime1);
		mergeSort(curItems, numOfItems);


		sortedFirst = firstFit(curItems, numOfItems, binCapacity);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ffdTime2);

		ffDiffer.tv_nsec = ffTime2.tv_nsec - ffTime1.tv_nsec;
		ffDiffer.tv_sec = ffTime2.tv_sec - ffTime1.tv_sec;

		ffdDiffer.tv_nsec = ffdTime2.tv_nsec - ffdTime1.tv_nsec;
		ffdDiffer.tv_sec = ffdTime2.tv_sec - ffdTime1.tv_sec;

		bfDiffer.tv_nsec = bfTime2.tv_nsec - bfTime1.tv_nsec;
		bfDiffer.tv_sec = bfTime2.tv_sec - bfTime1.tv_sec;

		outFile << numOfItems << ", ";
		outFile << firstFitBins << ", " << ((ffDiffer.tv_sec * 1000000000) + ffDiffer.tv_nsec);
		outFile << ", " << sortedFirst << ", " << ((ffdDiffer.tv_sec * 1000000000) + ffdDiffer.tv_nsec);
		outFile << ", " << numBestFit << ", " << ((bfDiffer.tv_sec * 1000000000) + bfDiffer.tv_nsec) << endl;

		/**
		firstFitBins = firstFit(curItems, numOfItems, binCapacity);
		

		
		numBestFit = bestFit(curItems, numOfItems, binCapacity);
	

		
		mergeSort(curItems, numOfItems);
		/*
		for (int j = 0; j < numOfItems; j++)
		{
			cout << items[j] << " " << endl;
		}

		sortedFirst = firstFit(curItems, numOfItems, binCapacity);
		
	

		cout << "Test Case " << i + 1 << " number of items: " << numOfItems;
		cout << " First Fit: " << firstFitBins << ", ";
		cout << "First Fit Decreasing: " << sortedFirst << ", ";
		cout << "Best Fit: " << numBestFit << ", "<< endl;*/


		firstFitBins = 0;
		numBestFit = 0;
		sortedFirst = 0;
	}

	free(items);
	items = NULL;

}



