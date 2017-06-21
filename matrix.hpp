#ifndef __MATRIX__
#define __MATRIX__

#include <iostream>
#include <sstream>
#include <algorithm>
<<<<<<< HEAD

//#define NUM_THREADS 1
=======
>>>>>>> grupo8

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
        setSize(w, h);
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
        setSize(w, h);        
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

    void setSize(int new_width, int new_height)
    {
        if((new_width <= 0) || (new_height <= 0))
        {
            std::cerr << "Invalid matrix size" << std::endl;
            throw(1);
        }

        if((height == new_height) && (width == new_width))
        {
            return;
        }
        else if(height == new_height)
        {
            setWidth(new_width);
        }
        else if(width == new_width)
        {
            setHeight(new_height);
        }
        else if(elements == 0)
        {
            width = new_width;
            height = new_height;

            elements = new T*[height];
            for(int i=0; i<height; i++)
            {
                elements[i] = new T[width];
            }
        }
        else
        {
            T **temp = new T*[new_height];
            for(int j=0; j<height; j++)
            {
                if(j < new_height)
                {
                    temp[j] = new T[new_width];

                    for(int i=0; i<std::min(new_width,width); i++)
                    {
                        temp[j][i] = elements[j][i];
                    }
                }
                delete[] elements[j];
            }
            for(int j=height; j<new_height; j++)
            {
                temp[j] = new T[new_width];                
            }

            delete[] elements;
            elements = temp;

            width = new_width;
            height = new_height;
        }
        return;
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

    bool isSquare() const
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

    Matrix<T> row(int row_index)
    {
        if(row_index < width)
        {
            Matrix<T> r(width, 1);
            for(int i=0; i<width; i++)
            {
                r[0][i] = elements[row_index][i];
            }
            return r;
        }
        else
        {
            std::cerr << "Inex out of range" << std::endl;
            throw(1);
        }
    }

    Matrix<T> col(int col_index)
    {
        if(col_index < height)
        {
            Matrix<T> r(1, height);
            for(int i=0; i<height; i++)
            {
                r[i][0] = elements[i][col_index];
            }
            return r;
        }
        else
        {
            std::cerr << "Inex out of range" << std::endl;
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

	    	elements = new T*[height];
			for(int i=0; i<height; i++)
			{
				elements[i] = new T[width];

				for(int j=0; j<width; j++)
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

    T determinant(){
        Matrix<T> LU(width, height);
        Matrix<T> P(width, height);
        decompositionLUP(*const_cast<Matrix<T>*>(this), LU, P);
        T det=1;
        int signal=-1;
        for (int i=0;i<width;i++){
            det*=LU[i][i];
            if (P[i][i]==0)
                signal*=-1;
        }
        det*=signal;
        return det;
    }
};

#endif
