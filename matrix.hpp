#ifndef __MATRIX__
#define __MATRIX__

#include <iostream>
#include <sstream>
#include <algorithm>

template <class T> 
class Matrix
{
private:
	T **elements = 0;   //pointer to matrix elements
	int width = 0;   //
	int height = 0;
public:

    Matrix(){}   //constructor 1

    Matrix(int w, int h)   //contructor 2
    {
    	width = w;
    	height = h;

    	elements = new T*[height];
    	for(int i=0; i<height; i++)
    	{
    		elements[i] = new T[width];
    	}
    }

    Matrix(int w, int h, T value)   //constructor 3
    {
    	width = w;
    	height = h;

    	elements = new T*[height];
    	for(int i=0; i<height; i++)
    	{
    		elements[i] = new T[width];
    	}
    	fill(value);
    }

    Matrix(Matrix<T> &other)   //constructor 4
    {
    	width = other.getWidth();
    	height = other.getHeight();

		elements = new T*[height];
		for(int i=0; i<height; i++)
		{
			elements[i] = new T[width];
			for(int j=0; j<other.width; j++)
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

    //valgrind
    
    void setWidth(int new_width) 
    {
        if((new_width < width) && (elements != 0))
        {
            for(int i=0; i<height; i++)
            {
                T *temp = new T[new_width];
                for(int j=0; j<new_width; j++)
                {
                    temp[i] = this[i][j];
                }
                delete[] elements[i];
                elements[i] = temp;
                delete[] temp;
            }
        }
        width = new_width;
        return;
    }

    void setHeight(int new_height)
    {
    	if((new_height < height) && (elements != 0))
    	{
    		for(int i=0; i<width; i++)
    		{
    			for(int j=0; j<new_height; j++)
    			{
    				T temp = new T;
    				temp = this[i][j];
    				delete[] elements[i][j];
    				elements[i][j] = temp;
    				delete temp;
    			}
    		}
    	}
    	height = new_height;
    	return;
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

	Matrix<T>& operator+=(const Matrix<T> &other) 
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
