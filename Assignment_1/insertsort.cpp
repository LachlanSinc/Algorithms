/**************
*Author: Lachlan Sinclair
*Date: 6/26/2019
*Description: This program sorts arrays from a file
*named data.txt using insertion sort and outputs the results to a file named
*insert.txt
****************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

//prototypes
void insertionSort(int[], int);
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
	ofstream insertFile("insert.txt");

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
		int *insert = (int*)malloc(sizeof(int)*size);

		//loop through the rest of the line reading in elements to the array
		for (int i = 0; i < size; i++)
		{
			inputData >> temp;
			insert[i] = temp;
		}
		//call insertion sort
		insertionSort(insert, size);

		//send the sorted array to the output file
		if (insertFile.is_open())
		{
			for (int i = 0; i < size; i++)
			{
				insertFile << insert[i] << " ";
			}
			insertFile << endl;
		}
		else {
			cout << "failed to open inesert file." << endl;
		}

		//free memory
		free(insert);
	}
	//close streams
	insertFile.close();
	inputData.close();

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
		//while the current value is less than current position in the array
		while (index >= 0 && val < arr[index])
		{
			//shift the array up
			arr[index + 1] = arr[index];
			index--;
		}
		//place the value in the correct spot
		arr[index + 1] = val;
	}
}