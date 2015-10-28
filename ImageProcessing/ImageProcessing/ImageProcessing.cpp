// ImageProcessing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sstream> // stringstream
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <istream>
#include "MatrixClass.h"
#include <math.h>

using namespace std;

void unShuffleImage();
void findWally();
void ncFindWally();
double* readTXT(char *fileName, int sizeR, int sizeC);
void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q);

int _tmain(int argc, _TCHAR* argv[])
{	
	int usersChoice;
	
	cout << "\nPlease Select you process\n1: Logo Restoration\n2: Find Wally\n3: Exit" << endl;
	cin >> usersChoice;
	
	if (usersChoice == 1)
	{
		cout << "\nRstoring Logo, please wait..." << endl;
		unShuffleImage();		
		cout << "\nLogo Restored\n\n" << endl;
	}
	else
	{
		if (usersChoice == 2)
		{
			cout << "\nFinding Wally, please wait..." << endl;
			ncFindWally();
			cout << "\nWally Found\n\n" << endl;
		}
		else
		{
			if(usersChoice == 3)
			{
				exit(0);
			}
			else
			{
				cout << "\nInvalid input!\n" << endl;	
			}
		}
	}
	return 0;
}

void unShuffleImage()
{
	//Variables to hold size of images
	const int rows = 512;
	const int columns = 512;
	const int blockSize = 32;

	//Nearest neighbour search variables
	double nnsScore = 0;
	double nnsBestScore = 0;
	int bestRow = 0;
	int bestCol = 0;

	int blockCount = 0;

	//load shuffled logo image and store in new matrix
	char* shuffledFileName = "logo_shuffled.txt"; 
	double* shuffled_data = 0;
	shuffled_data = readTXT(shuffledFileName, rows, columns);
	MatrixClass shuffledLogo(rows, columns, shuffled_data);

	//load noisey logo image perform restoration and store in new matrix
	char* restoredFileName = "logo_with_noise.txt";
	double* restored_data = 0;
	restored_data = readTXT(restoredFileName, rows, columns);

	//Noisey restoration 
	for (int i = 0; i < (rows * columns); i++)
	{
		if(restored_data[i] < 170)
		{
			restored_data[i] = 0;
		}
		else
		{
			restored_data[i] = 255;
		}
	}

	MatrixClass restoredImage(rows, columns, restored_data);

	//Creates new matrix to hold solved logo unshuffle
	double* unShuffled_data = new double[rows * columns]; 

	for (int i = 0; i < (rows * columns); i++)
	{
		unShuffled_data[i] = 170; //set each value of matrix
	}

	MatrixClass unshuffledLogo(rows, columns, unShuffled_data);

	//Matrix ceation to hold parts and results of NNS calculations
	MatrixClass shuffledImageBlock(blockSize, blockSize, 0.0);
	MatrixClass restoredImageBlock(blockSize, blockSize, 0.0);
	MatrixClass nnsDifference(blockSize, blockSize, 0.0);
	MatrixClass nnsSquaredSum(blockSize, blockSize, 0.0);
	

	//NNS and unshuffle process
	//nested loops to get each block of the shuffled image
	for (int sRow = 0; sRow <= (rows - blockSize); sRow += blockSize)
	{
		for (int sCol = 0; sCol <= (columns - blockSize); sCol += blockSize)
		{
			nnsBestScore = 100000;

			shuffledImageBlock = shuffledLogo.getBlock(sRow, sCol, (sRow + blockSize), (sCol + blockSize)); //change rows and cols around if not working 
				

			//nested loops to get block of restored image to compare to
			for (int rRow = 0; rRow <= (rows - blockSize); rRow += blockSize)
			{
				for (int rCol = 0; rCol <= (columns - blockSize); rCol += blockSize)
				{
					restoredImageBlock = restoredImage.getBlock(rRow, rCol, (rRow + blockSize), (rCol + blockSize));

					blockCount += 1;
					//cout << "BLOCK COUNT: " << blockCount << endl;


					//calculation of NNS
					nnsDifference = shuffledImageBlock - restoredImageBlock;
					nnsSquaredSum = nnsDifference * nnsDifference;
					nnsScore = nnsSquaredSum.sumOfElements();

					if (nnsScore == 0)
					{
						nnsScore = 0.00000001;
					}

					//sets row and column value for the best scoring match
					if (nnsScore >= 0 && nnsScore <= nnsBestScore)
					{
						nnsBestScore = nnsScore;
						bestRow = rRow;
						bestCol = rCol;
					}

					//set block to new location
					for (int bRow = bestRow; bRow < (bestRow + blockSize); bRow++)
					{
						for (int bCol = bestCol; bCol < (bestCol + blockSize); bCol++)
						{
							unshuffledLogo.setVal(bRow, bCol, restoredImage.getVal(bRow, bCol)); //swap row and col ifnot working
						}
					}
				}
			}

			//testing purposes
			//set unshuffled_data for output
			/*for (int pRow = 0; pRow < rows; pRow++)
			{
				for (int pCol = 0; pCol < columns; pCol++)
				{
					unShuffled_data[pRow * columns + pCol] = unshuffledLogo.getVal(pRow, pCol);
				}
			}
	

			int Q = 255;
			char* solvedOutput = "C:\\Users\\Daniel\\Documents\\Logo_UnShuffled.pgm";
			WritePGM(solvedOutput, unShuffled_data, rows, columns, Q);	*/
		}
	}


	//set unshuffled_data for output
	for (int pRow = 0; pRow < rows; pRow++)
	{
		for (int pCol = 0; pCol < columns; pCol++)
		{
			unShuffled_data[pRow * columns + pCol] = unshuffledLogo.getVal(pRow, pCol);
		}
	}
	

	int Q = 255;
	char* solvedOutput = "Logo_UnShuffled.pgm";
	WritePGM(solvedOutput, unShuffled_data, rows, columns, Q);	

	delete [] shuffled_data;
	delete [] restored_data;
	delete [] unShuffled_data;
}

void ncFindWally()
{
	//Sets sizes of the images used
	const int sceneRows = 768;
	const int sceneColumns = 1024;
	const int wallyRows = 49;
	const int wallyColumns = 36;

	//Sets initial values of variables used in NNS calculation
	int row = 0;
	int blockCount = 0;
	double nnsScore = 0;
	double nnsBestScore = 0;
	int bestRow = 0;
	int bestCol = 0;

	//Reads in Cluttered scene text file and stored in heap memory, additionally creating a matrix to hold this data
	double* scene_data = 0;
	char* sceneFileName = "Cluttered_scene.txt"; 
	scene_data = readTXT(sceneFileName, sceneRows, sceneColumns);
	MatrixClass sceneMatrix(sceneRows,sceneColumns,scene_data);

	//Reads in Wally text file and stored in heap memory, additionally creating a matrix to hold this data
	double* wally_data = 0;
	char* wallyFileName = "Wally_grey.txt";
	wally_data = readTXT(wallyFileName, wallyRows, wallyColumns);
	MatrixClass wallyMatrix(wallyRows, wallyColumns, wally_data);

	//Create a new pointer to heap that hold the data of the wally found image with a matrix to hold this data 
	double* wallyFound_data = scene_data;
	MatrixClass wallyFound(sceneRows, sceneColumns, wallyFound_data);

	//Start of calculation 
	//Executes while the row value is below that of the stated limit
	while (row <= sceneRows - wallyRows)
	{
		//Loops through each columns of the cluttered scene matrix untill the limit it reached
		for (int col = 0; col <= sceneColumns - wallyColumns; col++)
		{
			//Gets block of the cluttered scene image using the current row and col value and the size of the wally image
			MatrixClass sceneImageBlock = sceneMatrix.getBlock(row, col, (row + wallyRows), (col + wallyColumns));
			blockCount += 1;
			
			//Creates and set initial values of calculation variables
			double wallyMean = 0;
			double blockMean = 0;
			double WallySq = 0;
			double blockSq = 0;
			double num = 0;
			double denom = 0;

			//Setting the mean of each matrix by calling mean()
			wallyMean = wallyMatrix.mean();
			blockMean = sceneImageBlock.mean();

			//Create and set martix's holding the new matrix values by calling minusingMean()
			MatrixClass newWallyVal = wallyMatrix.minusingMean(wallyMean);
			MatrixClass newBlockVal = sceneImageBlock.minusingMean(blockMean); 

			//Create a matrix to hold the value of the numerator
			MatrixClass numerator = newWallyVal * newBlockVal; 

			//Create matrix to store the sum of image squared
			MatrixClass wallySquared = (newWallyVal * newWallyVal);
			MatrixClass blockSquared = (newBlockVal * newBlockVal);

			//Set value of each squared sum variable by calling sumOfElements()
			WallySq = wallySquared.sumOfElements();
			blockSq = blockSquared.sumOfElements();

			//Set num and denom values
			num = numerator.sumOfElements();
			denom = sqrt(WallySq * blockSq);

			//Calculate NNS score
			nnsScore = (num / denom);
			
			//Execute and set bestrow, bestCol and nnsBestScore if score is less than best score
			if (nnsScore > nnsBestScore)
			{
				nnsBestScore = nnsScore;
				bestRow = row;
				bestCol = col;
			}
		}

		//increament the row value
		row += 1;
	}

	//Loop throuh wally_found data for the best position found and set values to 0
	for (int sRow = bestRow; sRow < (bestRow + wallyRows); sRow++)
	{
		for (int sCol = bestCol; sCol < (bestCol + wallyColumns); sCol++)
		{
			wallyFound_data[sRow * sceneColumns + sCol] = 0;
		}
	}


	int Q = 255; 
	char* outputFileName = "Wally_Found.pgm";
	WritePGM(outputFileName, wallyFound_data, sceneRows, sceneColumns, Q); 
	
	//Delete vdata stored on the heap
	delete [] scene_data;
	delete [] wally_data;
}

double* readTXT(char *fileName, int sizeR, int sizeC)
{
  double* data = new double[sizeR*sizeC];
  int i=0;
  ifstream myfile (fileName);
  if (myfile.is_open())
  {
	 
	while ( myfile.good())
    {
       if (i>sizeR*sizeC-1) break;
		 myfile >> *(data+i);
        // cout << *(data+i) << ' '; // This line display the converted data on the screen, you may comment it out. 
	     i++;                                                             
	}
    myfile.close();
  }

  else cout << "Unable to open file"; 
  //cout << i;

  return data;
}

void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q)
{

 int i, j;
 unsigned char *image;
 ofstream myfile;

 image = (unsigned char *) new unsigned char [sizeR*sizeC];

 // convert the integer values to unsigned char
 
 for(i=0; i<sizeR*sizeC; i++)
	 image[i]=(unsigned char)data[i];

 myfile.open(filename, ios::out|ios::binary|ios::trunc);

 if (!myfile) {
   cout << "Can't open file: " << filename << endl;
   exit(1);
 }

 myfile << "P5" << endl;
 myfile << sizeC << " " << sizeR << endl;
 myfile << Q << endl;

 myfile.write( reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));

 if (myfile.fail()) {
   cout << "Can't write image " << filename << endl;
   exit(0);
 }

 myfile.close();

 delete [] image;

}