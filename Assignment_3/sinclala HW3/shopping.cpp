/*********
* Author: Lachlan Sinclair
* Date: 7/11/2019
* Description: This program determines the optimal items
* for members a family to select in order to maxmize the value
* of the carrying capacity.
***********/

#include<fstream>
#include <stdio.h>     
#include <stdlib.h>  
#include <iostream>
#include <string>

using namespace std;



int main()
{
	int F, T, N;
	int W[100];
	int P[100];
	int M[30];
	int maxW = 0;

	ifstream shopFile;
	ofstream resFile;

	shopFile.open("shopping.txt");
	resFile.open("results.txt");

	if (!shopFile.is_open() || !resFile.is_open())
	{
		cout << "Files did not open." << endl;
		return 1;
	}

	//begin reading in from the file

	shopFile >> T;

	//loop through all of the problems
	for (int z = 1; z <= T; z++)
	{
		//display the problem number in the output
		resFile << "Test Case " << z << endl;
		shopFile >> N;
		//read in the weight/price values of the items
		for (int i = 0; i < N; i++)
		{
			//price first
			shopFile >> P[i];
			shopFile >> W[i];
		}
		//read in the carrying weights of each family member
		shopFile >> F;
		for (int i = 0; i < F; i++)
		{
			//track the max weight, this allows the table to only be generated once
			shopFile >> M[i];
			if (M[i] > maxW)
			{
				maxW = M[i];
			}
		}
		//Everything has now been read in for this problem.

		//malloc a table of size N+1, max weight +1
		//plus ones allow for the additional 0 value column/row
		int **B = (int **)malloc((N + 1) * sizeof(int *));
		for (int i = 0; i < N + 1; i++) {
			B[i] = (int *)malloc((maxW + 1) * sizeof(int));
		}
		//all weights paired with not items have zero value
		for (int i = 0; i <= maxW; i++)
		{
			B[0][i] = 0;
		}
		//nested loop similiar to the one provide in the lectures
		for (int i = 1; i <= N; i++)
		{
			//for all number of items paired with weight zero set the value to zero
			B[i][0] = 0;

			for (int w = 1; w <= maxW; w++)
			{
				//check to see if the current items weight is less that current weight
				//-1 when accessing W and P to account for the 0th rows in the table
				if (W[i - 1] <= w)
				{
					//compare the previous item to using the current item + the best case for the remainder
					if (P[i-1] + B[i - 1][w - W[i - 1]] > B[i - 1][w]) 
					{
						//set the current item + the best case for the remainder
						B[i][w] = P[i - 1] + B[i - 1][w - W[i - 1]];
					}
					else
					{
						//the previous item is still the best option
						B[i][w] = B[i - 1][w];
					}
				}
				else 
				{
					B[i][w] = B[i - 1][w];
				}
			}
		}

		//sum up all the family members max values
		int sum = 0;
		for(int i=0; i<F; i++)
		{
			sum = sum + B[N][M[i]];
		}

		//Write the total to the output file
		resFile << "Total Price " << sum << endl;
		//format for the member section
		resFile << "Member Items:" << endl;


		//cout << sum << endl;

		//loop through every family member, back tracking through the table to determine which
		//items were used in selecting there , max value
		for (int i = 1; i <= F; i++)
		{
			//save their max, and weight as temp vals
			int tempVal = B[N][M[i-1]];
			int memW = M[i-1];

			//string used to reverse the order in which items are found, to allow for proper output
			string reverseString = "";

			//iterate back through the table, starting at the max weight and items number
			for (int x = N; x >= 1 && tempVal >= 1; x--)
			{
				//if the previous columns value does not equal the current columns value
				//the item was used in finding the total
				if (B[x - 1][memW] != tempVal)
				{
					//add the item to the front of the string
					reverseString = to_string(x) + " " + reverseString;

					//subtract the weight and values to look for the next item
					memW -= W[x - 1];
					tempVal -= P[x - 1];
				}
			}

			//cout << reverseString << endl;

			reverseString = to_string(i) + ": "+ reverseString;
			resFile << reverseString << endl;

		}

		resFile << endl;

		//clear the table memory for the next problem
		for (int i = 0; i < N + 1; i++) {
			free(B[i]);
		}
		free(B);
	}


	shopFile.close();
	resFile.close();
	return 0;
}
