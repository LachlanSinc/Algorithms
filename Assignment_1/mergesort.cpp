/**************
*Author: Lachlan Sinclair
*Date: 6/26/2019
*Description: This program sorts arrays from a file
*named data.txt using merge sort and outputs the results to a file named
*merge.txt
****************/


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;


//prototypes
void mergeSort(int arr[], int size);
void merge(int first[], int second[], int lower, int middle, int upper);
void mergeSortHelper(int first[], int second[], int lower, int upper);

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



int main(int argc, const char *argv[])
{
	string fileName = "data.txt";
	ifstream inputData;

	if (argc > 1)
	{
		fileName = argv[1];
	}

	inputData.open(fileName.c_str());

	ofstream mergeFile("merge.txt");

	if (!inputData) {
		cout << "Unable to open file data.txt" << endl;
		exit(1); //exit
	}

	int size;
	int temp;

	while (inputData >> size)
	{
		//malloc an array based off the size of the first element of a line
		int *merge = (int*)malloc(sizeof(int)*size);

		//read in the values to the array
		for (int i = 0; i < size; i++)
		{
			inputData >> temp;
			merge[i] = temp;

		}
		//call insertion sort
		mergeSort(merge, size);

		//store sorted arrays into there corresponding files
		if (mergeFile.is_open())
		{
			for (int i = 0; i < size; i++)
			{
				mergeFile << merge[i] << " ";
			}
			mergeFile << endl;
		}
		else {
			cout << "failed to open merge file." << endl;
		}

		//free memory
		free(merge);
	}

	//close streams
	mergeFile.close();
	inputData.close();

	return 0;
}

