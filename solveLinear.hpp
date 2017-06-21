#include <iostream>
#include <pthread.h>
#include "matrix.hpp"

template<typename T>
void solveLinear(Matrix<T> &mat_a, Matrix<T> &mat_b, Matrix<T> &output)
{
    //Solves a linear system of the type Ax = B, where A is a N x N matrix and B is a N x 1 column matrix
    // through the LUP decoposition method and fills the 'output' matrix (also N x 1) with it's solution

    if(mat_a.isSquare() && (mat_a.getHeight() == mat_b.getHeight()) && (mat_b.getWidth() == 1))
    {        
        Matrix<T> mat_y(1, mat_a.getHeight());

        output.setSize(1, mat_b.getHeight());

        Matrix<T> mat_lu(mat_a.getWidth(), mat_a.getHeight());
        Matrix<T> mat_p(mat_a.getWidth(), mat_a.getHeight());

        //the equasion starts as Ax = b

        decompositionLUP(mat_a, mat_lu, mat_p);

        //after LUP decomposition, the equasion becomes LUx = Pb, or L(Ux) = Pb

        Matrix<T> mat_pb(1, mat_a.getHeight());
        multiplyMatrix(mat_p, mat_b, mat_pb);

        //then, with the substitution Ux = y, the equasion becomes Ly = Pb
        for(int i=0; i<mat_lu.getHeight(); i++)
        {
            mat_y[i][0] = mat_pb[i][0];
            for(int j=0; j<i; j++)
            {
                mat_y[i][0] = mat_y[i][0] - mat_lu[i][j]*mat_y[j][0];
            }
        }

        //after solving Ly = Pb and finding the value of 'y', the equasion Ux = b is solved
        int last_col = mat_lu.getWidth()-1;
        int last_row = mat_lu.getHeight()-1;
        for(int i=last_row; i>=0; i--)
        {
            output[i][0] = mat_y[i][0];

            for(int j=last_row; j>i; j--)
            {
                output[i][0] = output[i][0] - mat_lu[i][j]*output[j][0];
            }
            output[i][0] = output[i][0]/mat_lu[i][i];
        }
        //finally, after solving Ux = b, the 'output' matrix contains the values of 'x' that satisfy
        // the coeficients in 'A'
    }
    else
    {
        std::cerr << "Invalid matrixes" << std::endl;
        throw(1);
    }
}