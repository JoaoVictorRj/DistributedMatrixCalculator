#ifndef __MATRIX__
#define __MATRIX__
#define N 8

#include <iostream>
#include <sstream>
#include <pthread.h>

pthread_t threads[N];
int rc;

template <class T> 
class Matrix
{
private:
	T **elements = 0;
	int width = 0;
	int height = 0;

public:

    Matrix(){}

    Matrix(int w, int h)
    {
    	width = w;
    	height = h;

    	elements = new T*[width];
    	for(int i=0; i<width; i++)
    	{
    		elements[i] = new T[height];
    	}
    }

    Matrix(int w, int h, T value)
    {
    	width = w;
    	height = h;

    	elements = new T*[width];
    	for(int i=0; i<width; i++)
    	{
    		elements[i] = new T[height];
    	}
    	fill(value);
    }

    Matrix(Matrix<T> &other)
    {
    	width = other.getWidth();
    	height = other.getHeight();

		elements = new T*[width];
		for(int i=0; i<width; i++)
		{
			elements[i] = new T[height];
			for(int j=0; j<other.height; j++)
			{
				elements[i][j] = other[i][j];
			}
		}
    }

    ~Matrix()
    {
    	clear();
    }

    int getWidth() const
    {
    	return width;
    }

    int getHeight() const
    {
    	return height;
    }

    T** getPointer() const
    {
    	return elements;
    }

    void fill(T element)
    {
    	//(to-do) multithread
    	for(int i=0; i<width; i++)
		{
		for(int j=0; j<height; j++)
		{
			elements[i][j] = element;
		}
		}
    }

    void clear()
    {
    	if(elements != 0)
    	{
    		for(int i=0; i<width; i++)
    		{
    			delete[] elements[i];
    		}
    		delete[] elements;

    		elements = 0;
	    	width = 0;
	    	height = 0;
    	}
    }
	
	void *add_t();

    void add(Matrix<T> &other)
    {
	    if(	(other.getWidth()  == width) && 
		(other.getHeight() == height) )
	    {
		for(int i=0; i<width; i++)
		{
			for(int j=0; j<height; j++)
			{
				elements[i][j] += other[i][j]
			}
		}
	    }
    }
    void sub(Matrix<T> &other);
    void mul(Matrix<T> &other);
    void copy(Matrix<T> &other);

    void determinant();
    void inverse();
    void transpose();


    T* operator[](int i) const
    {
    	return elements[i];
    }

    Matrix<T>& operator=(const Matrix<T>& other) // copy assignment
    {
    	if(this != &other)
    	{
    		clear();

	    	width = other.getWidth();
	    	height = other.getHeight();

	    	elements = new T*[width];
			for(int i=0; i<width; i++)
			{
				elements[i] = new T[height];
				for(int j=0; j<height; j++)
				{
					elements[i][j] = other[i][j];
				}
			}
    	}

    	return *this;
    }

    Matrix<T>& operator=(Matrix<T>&& other) noexcept // move assignment
	{
	    if(this != &other) {
	    	clear();

	    	elements = other.getPointer();
	    	other.elements = 0;

	    	width = other.getWidth();
	    	height = other.getHeight();

	    }
	    return *this;
	}

	Matrix<T>& operator+=(const Matrix<T>& other) 
	{
    	//(to-do) multithread
	 	if(	(other.getWidth()  == width) && 
			(other.getHeight() == height) )
		{
			for(int i=0; i<width; i++)
			{
			for(int j=0; j<height; j++)
			{
				elements[i][j] += other[i][j];
			}
			}
		}	
		return *this;
	}

	Matrix<T> operator+(const Matrix<T>& other)
	{
		Matrix<T> result(*this);
		result += other;
		return result;
	}

    friend std::ostream& operator<<(std::ostream &output, const Matrix<T> &obj) 
	{
		for(int i=0; i<obj.getWidth(); i++)
		{
			output << "[";
			for(int j=0; j<obj.getHeight(); j++)
			{
				output << obj[i][j];
				if(j != obj.getHeight() -1)
				{
					output << ",";
				}
			}
			output << "]" << std::endl;

		}
		return output;            
	}
};

#endif
