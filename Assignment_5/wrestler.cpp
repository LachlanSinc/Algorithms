/*********
* Author: Lachlan Sinclair
* Date: 7/24/2019
* Description: This program uses a BFS to determine if
* rivaleries between babyfaces and heels can be set up.
***********/

#include <fstream>
#include <stdio.h>     
#include <stdlib.h>  
#include <iostream>
#include <string>
#include <queue> 
#include <string.h>

using namespace std;

int BFS(struct wrestler *arr, int index, int numOfWrestlers);
void displayResults(struct wrestler *wrestlers, int numOfWrestlers);

struct wrestler {

	char name[50];
	int distance;
	struct link *next;

	//color of the node
	//0 white, 1 grey, 2 black
	int finished;
};

struct link {
	struct link *next;
	struct wrestler *wres;
};

/******************
*Note: I left the process of reteiving the data in the main since
*obtaining it in a function required a pretty complicated proccess of
*passing alot of pointers making the code harder to follow.
******************/
int main(int argc, const char *argv[])
{

	string fileName;

	//check for command line input
	if (argc > 1)
	{
		fileName = argv[1];
	}
	//error, no file provided
	else
	{
		cout << "Valid file not provided." << endl;
		exit(1);
	}

	int numOfWrestlers;
	int numOfRivals;

	//call the function to get data

	//getData(wrestlers, mat, fileName, numOfWrestlers, numOfRivals);
	ifstream inputData;
	inputData.open(fileName.c_str());

	if (!inputData.is_open())
	{
		cout << "error opening file." << endl;
		exit(1);
	}

	inputData >> numOfWrestlers;


	struct wrestler *wrestlers = (struct wrestler *)malloc(sizeof(struct wrestler)*numOfWrestlers);

	//initialize all of the wrestlers structs
	for (int i = 0; i < numOfWrestlers; i++)
	{
		wrestlers[i].distance = -1;
		wrestlers[i].finished = 0;
		inputData >> wrestlers[i].name;
		wrestlers[i].next = NULL;
	}

	//read in the number of rivals
	inputData >> numOfRivals;


	//ints and strings used for indexing edges
	int wrest1Ind;
	int wrest2Ind;
	char wrest1Str[50];
	char wrest2Str[50];

	//fill out the edge list with the rivalaries
	for (int i = 0; i < numOfRivals; i++)
	{
		inputData >> wrest1Str;
		inputData >> wrest2Str;


		//find the corresponding indexes
		for (int j = 0; j < numOfWrestlers; j++)
		{
			//when the name of the struct in the array matchs record the index
			if (strcmp(wrest1Str, wrestlers[j].name) == 0)
			{
				wrest1Ind = j;
			}
			else if (strcmp(wrest2Str, wrestlers[j].name) == 0)
			{
				wrest2Ind = j;
			}
		}
		//add the link to the front of the adjancey list
		struct link *temp1 = new link();
		temp1->next = wrestlers[wrest1Ind].next;
		temp1->wres = &wrestlers[wrest2Ind];
		wrestlers[wrest1Ind].next = temp1; 

		struct link *temp2 = new link();
		temp2->next = wrestlers[wrest2Ind].next;
		temp2->wres = &wrestlers[wrest1Ind];
		wrestlers[wrest2Ind].next = temp2;

	}



	//used to determine if the rivalries are possible
	int returnVal;

	//loop through all of the wrestlers making sure there node get processed by BFS
	for (int i = 0; i < numOfWrestlers; i++)
	{
		if (wrestlers[i].finished == 0)
		{
			returnVal = BFS(wrestlers, i, numOfWrestlers);
			//if at any point the rivaleries cant be made for a subgraph, break out
			if (returnVal != 0)
			{
				break;
			}
		}
	}

	//if the rivalaries are possible display them
	if (returnVal == 0)
	{
		//call the function to display the results
		displayResults(wrestlers, numOfWrestlers);
	}

	//clear the memory used for the adjancey list
	for (int i = 0; i < numOfWrestlers; i++)
	{
		struct link *temp = wrestlers[i].next;
		struct link *prev;
		while (temp != NULL)
		{
			prev = temp;
			temp = temp->next;
			free(prev);
		}

	}
	free(wrestlers);

	return 0;
}
/******************
* Desciption: this funtion displays the results returned by the BFS function
******************/
void displayResults(struct wrestler *wrestlers, int numOfWrestlers)
{
	string babyface = "Babyfaces:";
	string heels = "Heels:";

	for (int i = 0; i < numOfWrestlers; i++)
	{
		//append the babyfaces/heels to their corresponding strings
		if ((wrestlers[i].distance % 2) == 0)
		{
			babyface.append(" ");
			babyface.append(wrestlers[i].name);
		}
		else
		{
			heels.append(" ");
			heels.append(wrestlers[i].name);
		}
	}
	//display the results
	cout << "Yes Possible" << endl;
	cout << babyface << endl;
	cout << heels << endl;

}
/******************
* Description:
******************/
int BFS(struct wrestler *arr, int cur, int numOfWrestlers)
{
	//set the first element in the tree to distance 0
	arr[cur].distance = 0;

	//create the queue and add the first vertice to it
	queue <struct wrestler *> bfsQ;
	bfsQ.push(&arr[cur]);

	//search all connected nodes
	while (!bfsQ.empty())
	{
		//return the next value from the queue
		struct wrestler tempW = *bfsQ.front();
		bfsQ.pop();
		
		//check the adjancey list for the current vertice
		struct link *temp = tempW.next;
		while(temp!=NULL)
		{
				//if the node is white
				if (temp->wres->finished == 0)
				{
					//set it it grey, mark add it to the queue, update its distance
					temp->wres->finished = 1;
					bfsQ.push(temp->wres);
					temp->wres->distance = tempW.distance + 1;
				}
				//if the other vertex is grey or black, and has the same parity as the current vertex the rivaleries cannot be set up
				else if ((temp->wres->finished == 1 || temp->wres->finished == 2) && ((tempW.distance % 2) == (temp->wres->distance % 2)))
				{
					//they do not match
					cout << "There is no solution." << endl;
					//return 1 since the rivaleries cannot be set up
					return 1;
				}
				temp = temp->next;
		}
		//set the node to black
		tempW.finished = 2;
	}
	//return 0 since the rivaleries can be set up
	return 0;
}








