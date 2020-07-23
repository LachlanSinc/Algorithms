/**************
*Author: Lachlan Sinclair
*Date: 7/7/2019
*Description: This program uses stooge sort to sort a
* txt file named data.txt and stores it in a txt file called stooge.txt
****************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

//prototypes
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

int main(int argc, const char *argv[])
{
	//create andd open a input stream
	string fileName = "data.txt";

	if (argc > 1)
	{
		fileName = argv[1];
	}

	ifstream inputData;
	inputData.open(fileName.c_str());

	//create/open an output stream
	ofstream insertFile("stooge.txt");

	//check to make sure the input file opened correctly
	if (!inputData) {
		cout << "Unable to open file data.txt" << endl;
		exit(1); //exit
	}

	int size;
	int temp;

	//loop while not at eof
	while (inputData >> size)
	{
		//malloc an array based of the first element of a line
		int *stooge = (int*)malloc(sizeof(int)*size);

		//loop through the rest of the line reading in elements to the array
		for (int i = 0; i < size; i++)
		{
			inputData >> temp;
			stooge[i] = temp;
		}
		//call stooge sort
		stoogeSort(stooge, 0, size-1);

		//send the sorted array to the output file
		if (insertFile.is_open())
		{
			for (int i = 0; i < size; i++)
			{
				insertFile << stooge[i] << " ";
			}
			insertFile << endl;
		}
		else {
			cout << "failed to open inesert file." << endl;
		}

		//free memory
		free(stooge);
	}
	//close streams
	insertFile.close();
	inputData.close();

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