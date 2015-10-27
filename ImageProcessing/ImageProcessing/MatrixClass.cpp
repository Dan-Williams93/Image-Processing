
#include "StdAfx.h"
#include "MatrixClass.h"
#include "iostream"
using namespace std;

//Default constructor
MatrixClass::MatrixClass(void)
{	
	rows = 10;
	columns = 10;
	data = 0;
}

//Constructor taking in number of rows, columns and a value
MatrixClass::MatrixClass(int sizeR, int sizeC, double value)
{
	rows = sizeR;
	columns = sizeC;

	data = new double[rows * columns];

	for (int i = 0; i < (rows * columns); i++)
	{
		data[i] = value;
	}
}

//Constructor taking in number of rows, columns and a pointer to data
MatrixClass::MatrixClass(int sizeR, int sizeC, double* input_data)
{
	rows = sizeR;
	columns = sizeC;

	data = new double[rows * columns];
	
	for (int i = 0; i < (rows * columns); i++)
	{
		data[i] = input_data[i];
	}
}

//Copy constructor taking in reference to matrix to copy
MatrixClass::MatrixClass(const MatrixClass& copyMatrix)
{
	rows = copyMatrix.getRows();
	columns = copyMatrix.getColumns();

	data = new double[rows * columns];

	for (int i = 0; i < (rows * columns); i++)
	{
		data[i] = copyMatrix.data[i];
	}
}

//Returns the number of rows in a matrix
int MatrixClass::getRows() const
{
	return rows;
}

//Returns the number of columns in a matrix
int MatrixClass::getColumns() const
{
	return columns;
}

//Returns element value at the location row, column passed in 
double MatrixClass::getVal(int row, int col)
{
	return data[row * columns + col];
}

//Sets element at position row, column to the value passed in
void MatrixClass::setVal(int row, int col, double val)
{
	data[row * columns + col] = val;
}

//Returns a block of a given matrix takes in the start and end positions of the block
MatrixClass MatrixClass::getBlock(int startR, int startC, int endR, int endC)
{
	int numRows = endR - startR;
	int numColumns = endC - startC;

	//Creates the block matrix to be returned 
	MatrixClass subMatrix(numRows, numColumns, 0.0);							

	//Loops through the given location moving each element from the matrix to the new block matrix
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numColumns; col++)
		{
			subMatrix.setVal(row, col, getVal(row + startR, col + startC));
		}
	}

	return subMatrix;
}

//Operator overloaders takes in reference to matrix to use
MatrixClass MatrixClass::operator +(const MatrixClass& current)
{
	MatrixClass temp;

	temp.rows = current.rows;
	temp.columns = current.columns;
	
	//Creates temporary matrix
	temp.data = new double[rows * columns];

	//Loops through the temp matrix and sets the new value of each element
	for (int i = 0; i < (rows * columns); i++)
	{
		temp.data[i] = this->data[i] + current.data[i];
	}

	return temp;
}
MatrixClass MatrixClass::operator -(const MatrixClass& current)
{
	MatrixClass temp;

	temp.rows = current.rows;
	temp.columns = current.columns;

	temp.data = new double[rows * columns];

	for (int i = 0; i < (rows * columns); i++)
	{
		temp.data[i] = this->data[i] - current.data[i];
	}

	return temp;
}
MatrixClass MatrixClass::operator *(const MatrixClass& current)
{
	MatrixClass temp;

	temp.rows = current.rows;
	temp.columns = current.columns;

	temp.data = new double[rows * columns];

	for (int i = 0; i < (rows * columns); i++)
	{
		temp.data[i] = this->data[i] * current.data[i];
	}

	return temp;
}
MatrixClass MatrixClass::operator /(const MatrixClass& current)
{
	MatrixClass temp;

	temp.rows = current.rows;
	temp.columns = current.columns;

	temp.data = new double[rows * columns];

	for (int i = 0; i < (rows * columns); i++)
	{
		temp.data[i] = this->data[i] / current.data[i];
	}

	return temp;
}

//Operator = overloader that takes in matrix to the right by reference
MatrixClass MatrixClass::operator =(const MatrixClass& current)
{
	//Deletes any data stored in matrix = is called from 
	delete [] data;

	rows = current.rows;
	columns = current.columns;

	//Creates new new pointer to data of size rows * columns
	data = new double [rows * columns];

	//Loop to set data of to that of matrix passed in by reference
	for (int i = 0; i < (rows * columns); i++)
	{
		data[i] = current.data[i];
	}

	return *this;
}

//Increment prefix overloader takes reference to matrix 
MatrixClass& MatrixClass::operator ++()
{
	for (int i = 0; i < rows * columns; i++)
	{
		data[i] = this->data[i] + 1;
	}

	return *this;
}//prefix

//Increment postfix overloader
MatrixClass MatrixClass::operator ++(int)
{
	//Create temp mateix that is the same as the matrix method is called from
	MatrixClass temp (*this);

	//Loop that sets data to data + 1
	for (int i = 0; i < rows * columns; i++)
	{
		temp.data[i] = this->data[i] + 1;
	}

	return temp;
}//postfix

MatrixClass& MatrixClass::operator --()
{
	for (int i = 0; i < rows * columns; i++)
	{
		data[i] = this->data[i] - 1;
	}

	return *this;
}
MatrixClass MatrixClass::operator --(int)
{
	MatrixClass temp(*this);

	operator--();

	return temp;
}

//Opeator overloade that returns the value at a specified positions
double MatrixClass::operator ()(int row, int col)
{
	return getVal(row, col);
}

//Calculates the sum of all elements of in a given matrix and returns a double 
double MatrixClass::sumOfElements()
{
	double tempSum = 0;

	//Loops through matrix adding each element value to double tempSum
	for(int row = 0; row < rows; row++)
	{
		for(int col = 0; col < columns; col++)
		{
			tempSum = tempSum + data[row * columns + col];
		}
	}
	return tempSum;
}

//Calculates the mean of a given matrix and returns a double
double MatrixClass::mean()
{
	double Mean = 0;

	double size = rows * columns;

	Mean = sumOfElements();
	Mean = Mean / size;

	return Mean;
}

//Takes in mean value and minuses that value from every element of a matrix returnin a new matrix with the new values
MatrixClass MatrixClass::minusingMean(double mean)
{
	//Creates tempory matrix
	MatrixClass temp;

	//Sets size of tempory matrix
	temp.rows = rows;
	temp.columns = columns;

	//Sets the length of dynamic memory needed
	temp.data = new double[rows * columns];

	//Loop that calculates the new value of each element in the temp matrix
	for (int i = 0; i < (rows * columns); i++)
	{
		temp.data[i] = this->data[i] - mean;
	}

	return temp;
}

//Prints each element of the matrix
void MatrixClass::print()
{
	for(int row = 0; row < rows; row++)
	{
		for(int col = 0; col < columns; col++)
		{
			cout << * (data + row * columns + col) << "\t";
		}
		cout << "\n";
	}
}

//Released memory that is held by the object
MatrixClass::~MatrixClass(void)
{
	delete [] data;
}