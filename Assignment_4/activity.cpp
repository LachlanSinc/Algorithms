/*********
* Author: Lachlan Sinclair
* Date: 7/21/2019
* Description: Implements a greedy alogorithim
* for selecting the largest number of activities from a 
* set that do not overlap. 
***********/

#include<fstream>
#include <stdio.h>     
#include <stdlib.h>  
#include <iostream>
#include <string>

using namespace std;

//used to contain all 3 ints for activities
struct activity{
	int num;
	int start;
	int end;
};


//prototypes
void mergeSort(struct activity arr[], int size);
void merge(struct activity first[], struct activity second[], int lower, int middle, int upper);
void mergeSortHelper(struct activity first[], struct activity second[], int lower, int upper);

//I altered my mergesort code from HW1 to work with activity structs

/****************
* Function: mergeSort
* Description : copies the given array, then passes the orginal array address, and the new array address
* to the recursive mergesort function
* Input : Address of array, size of the array
* Output : none
* *****************/
void mergeSort(struct activity arr[], int size)
{
	//malloc a new array to allow for merging/spliting into one another
	struct activity *temp = (struct activity*)malloc(sizeof(struct activity)*size);
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
void merge(struct activity first[], struct activity second[], int lower, int middle, int upper)
{
	//temp variables to allow for incrementing through the two halfs of the array subset
	int x = lower;
	int y = middle;

	//loop through the indicies of the given subset
	for (int index = lower; index < upper; index++)
	{
		//if there is still variables left in the upper half, and the lower half is empty or it is less than the value in
		//the lower half, add the value from the upper half to the current position in the subset
		if (y < upper && (x >= middle || second[y].start >= second[x].start))
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
void mergeSortHelper(struct activity first[], struct activity second[], int lower, int upper)
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



/****************
* All new code is contained in the main, seperating it 
* into functions doesnt make to much sense since it is so simple.
***************/
int main()
{
	//set up the ifstream to read in the data
	ifstream actFile;
	actFile.open("act.txt");

	int numOfActs;
	int setNum =1;


	if (!actFile.is_open())
	{
		cout << "error opening file." << endl;
	}

	//loop until the end of file is found
	while (!actFile.eof())
	{
		//read in the number of activities for this set
		actFile >> numOfActs;

		//make sure the set has atleast one value
		if (numOfActs == 0)
		{
			continue;
		}
		
		//create an array to hold all activities for a set
		struct activity *set = (struct activity *)malloc(numOfActs * sizeof(struct activity));

		//for each activity in the set store its number, start, and end values into a struct in the struct array
		for (int i = 0; i < numOfActs; i++)
		{
			actFile >> set[i].num;
			actFile >> set[i].start;
			actFile >> set[i].end;
		}

		//use mergesort to sort the array based off of start times
		mergeSort(set, numOfActs);

		//applty the alogrithim
		
		//track the number of values in the optimal solution 
		int actCount = 0;
		//2nd array to hold the optimal solution
		struct activity *activities = (struct activity *)malloc(sizeof(struct activity)*numOfActs);
		int time;

		//intialize the first value of the selected activities
		//we know there is atleast one element so this wont error
		activities[0] = set[0];
		time = set[0].start;

		//for the rest of the sort array
		for (int i = 1; i < numOfActs; i++)
		{
			//if the  current elements end time is less than the last element in the solution arrrays start time
			//we know it is part of the optimal solution, add it to the solution array
			if (set[i].end <= activities[actCount].start)
			{
				actCount++;
				activities[actCount] = set[i];
			}
		}

		//display the reults for this set
		cout << "Set " << setNum << endl;
		cout << "Number of activities selected = " << actCount + 1 << endl;
		cout << "Activities:";

		//loop backwards through the solution array to match the examples given
		for (int i = actCount; i >= 0; i--)
		{
			cout << " " << activities[i].num;
		}
		cout << endl << endl;

		//increment the set number tracker(display purposes only
		setNum++;

		//free the two dynamically allocated arrays
		free(activities);
		free(set);
	}
}





