#pragma once
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Vector.h"
#define PI 3.14159265358979323846

//定義 Matrix
class Matrix
{	
public:
	Matrix();
	Matrix(std::vector<Vector>);

	std::vector<Vector> Data;	// Row Vector

	int getRow() const {		// Get Matrix Rows
		return Data.size();
	}

	int getCol() const {		// Get Matrix Columns
		return Data[0].getDim();
	}
	
	/* Operator Overloading */
	friend Matrix operator+(const Matrix&, const Matrix&);
	friend Matrix operator-(const Matrix&, const Matrix&);
	friend Matrix operator*(const Matrix&, const Matrix&);	// Matrix Multiplication

	/* Method */
	int Rank() const;		// nonZero rows

	friend Matrix Copy(const Matrix&);
	friend Matrix Transpose(const Matrix&);
	friend Matrix Inverse(const Matrix&);
	friend Matrix Solve_Linear_System(const Matrix&, const Matrix&);
	friend double Determinant(const Matrix&);
	friend  Matrix Adjoint(const Matrix&);
	friend Vector FindK(const Matrix&, int);	// Eigen used
	friend  Matrix Eigen(const Matrix&);
	friend std::vector<Vector> Power_Method(const Matrix&);
	friend Matrix LeastSquare(const Matrix&, const Matrix &);
	// 最後一筆測資
	friend Matrix U_Triangle(const Matrix&);
	
	// Vector用
	friend bool IsLI(const Matrix&);		// Linear Independent Judge
};