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

    Matrix(void *&p){   //don't edit this weird function
        Matrix(*const_cast<Matrix<T>*>(static_cast<const Matrix<T>*>(p)));
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

    void setWidth(int new_width) 
    {
        if((new_width != width) && (elements != 0))
        {
            for(int j=0; j<height; j++)
            {
                T *temp = new T[new_width];
                for(int i=0; i<std::min(new_width,width); i++)
                {
                    temp[i] = elements[j][i];
                }
                delete[] elements[j];
                elements[j] = temp;
            }
        	width = new_width;
        }
        return;
    }

    void setHeight(int new_height)
    {
    	if((new_height != height) && (elements != 0))
    	{
    		T **temp = new T*[new_height];
    		for(int i=0; i<std::min(new_height,height); i++)
    		{
    			temp[i] = elements[i];
    		}

    		if(new_height < height)
	    	{
	    		for(int i=new_height; i<height; i++)
	    		{
	    			delete[] elements[i];
	    		}
	    	}
	    	else if(new_height > height)
	    	{
	    		for(int i=height; i<new_height; i++)
	    		{
	    			temp[i] = new T[width];
	    		}
	    	}
    		delete[] elements;
    		elements = temp;
    		height = new_height;
    	}
    	return;
    }

    void fill(T element)
    {
    	//(to-do) multithread
    	for(int i=0; i<height; i++)
		{
		for(int j=0; j<width; j++)
		{
			elements[i][j] = element;
		}
		}
    }

    void clear()
    {
    	if(elements != 0)
    	{
    		for(int i=0; i<height; i++)
    		{
    			delete[] elements[i];
    		}
    		delete[] elements;

    		elements = 0;
	    	width = 0;
	    	height = 0;
    	}
    }

    bool isSquare()
    {
        if(height == width)
        {
            return true;
        }
        return false;
    }

    //(to-do) can be optimized
    void identity()
    {
        if(isSquare())
        {
            fill(0);
            for(int i=0; i<height; i++)
            {
                elements[i][i] = 1;
            }
        }
        else
        {
            std::cerr << "Only a square matrix can be identity" << std::endl;
            throw(1);
        }
    }

    void swapRows(int row1, int row2)
    {
        if((row1 < height) && (row2 < height))
        {
            T* temp = elements[row2];
            elements[row2] = elements[row1];
            elements[row1] = temp;
        }
        else
        {
            std::cerr << "Index out of range" << std::endl;
            throw(1);
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
		for(int i=0; i<obj.getHeight(); i++)
		{
			output << "[";
			for(int j=0; j<obj.getWidth(); j++)
			{
				output << obj[i][j];
				if(j != obj.getWidth() -1)
				{
					output << ",";
				}
			}
			output << "]" << std::endl;

		}
		return output;            
	}

    explicit operator void*(){
        return (void *) this;
    }
};

#endif
