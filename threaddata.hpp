#include "matrix.hpp"

template <typename T>
struct threadData
{
	Matrix<T> *mat1;
	Matrix<T> *mat2;
	double m;
	Matrix<T> *output;
	int start;
	int end;

    threadData(){}
};
