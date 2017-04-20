#ifndef __MATRIX__
#define __MATRIX__

#include <iostream>

template <class T> 
class Matrix
{
private:
	T **elements = 0;
	int width = 0;
	int height = 0;

public:

    Matrix(){}   //matrix class constructor

    Matrix(int w, int h)
    {
    	width = w;
    	height = h;

    	elements = new T*[width];
    	for(int i=0; i<width; i++)
    	{
    		elements[i] = new T[height];
    	}

    	std::cout << "Creating matrix" << std::endl;
    }

    ~Matrix()
    {
    	std::cout << "deleting matrix" << std::endl;
    	if(elements != 0)
    	{
    		for(int i=0; i<width; i++)
    		{
    			delete[] elements[i];
    		}
    		delete[] elements;
    	}
    }

    void add(Matrix &other);   //sum other matrix to this matrix

    void sub(Matrix &other);   //subtracts other matrix to this matrix

    void det();   //calculates the matrix's determinant

    void inv();   //calculates the matrix's inverse

};

#endif