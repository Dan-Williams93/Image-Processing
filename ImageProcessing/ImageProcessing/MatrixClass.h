#pragma once
class MatrixClass
{
public:
	MatrixClass(void);
	MatrixClass(int sizeR, int sizeC, double value);
	MatrixClass(int sizeR, int sizeC, double* input_data);
	MatrixClass(const MatrixClass& copyMatrix);

	int getRows() const;
	int getColumns() const;

	double getVal(int row, int col);
	void setVal(int row, int col, double val);

	MatrixClass getBlock(int startR, int startC, int endR, int endC);

	MatrixClass operator +(const MatrixClass& current);
	MatrixClass operator -(const MatrixClass& current);
	MatrixClass operator *(const MatrixClass& current);
	MatrixClass operator /(const MatrixClass& current);
	MatrixClass operator =(const MatrixClass& current);
	MatrixClass& operator ++();								//prefix
	MatrixClass operator ++(int);							//postfix
	MatrixClass& operator --();
	MatrixClass operator --(int);

	double operator ()(int row, int col);

	double sumOfElements();
	double mean();
	MatrixClass minusingMean(double mean);

	void print();

	~MatrixClass(void);

private:
	int rows;												//M
	int columns;											//N
	double* data;
};

