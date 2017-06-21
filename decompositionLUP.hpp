#include <iostream>
#include <pthread.h>
#include "matrix.hpp"

template<typename T>
void decompositionLUP(Matrix<T> &mat_a, Matrix<T> &mat_lu, Matrix<T> &mat_p)
{
    if(mat_a.isSquare() && mat_lu.isSquare() && mat_p.isSquare() &&
      (mat_a.getWidth() == mat_lu.getWidth()) && 
      (mat_a.getWidth() == mat_p.getWidth()) )
    {
        mat_p.identity();
        //(to-do) evaluate if this copy is necessary and if so, optimize it
        mat_lu = mat_a;

        for(int i=0; i<mat_lu.getHeight() -1; i++)
        {
            int max_pivot = i;
            for(int j = i+1; j<mat_lu.getHeight(); j++)
            {
                if(abs(mat_lu[j][i]) > abs(mat_lu[max_pivot][i]))
                {
                    max_pivot = j;
                }
            }


            if(max_pivot != i)
            {
                mat_lu.swapRows(max_pivot,i);
                mat_p.swapRows(max_pivot,i);
            }

            for(int j=i+1; j<mat_lu.getHeight(); j++)
            {
                mat_lu[j][i] = mat_lu[j][i]/mat_lu[i][i];
                for(int k=i+1; k<mat_lu.getWidth(); k++)
                {
                    mat_lu[j][k] = mat_lu[j][k] - mat_lu[i][k]*mat_lu[j][i];
                }
            }
        }
    }
    else
    {
        std::cerr << "In a LUP decomposition all matrixes must be square" << std::endl;
        throw(1);
    }
}