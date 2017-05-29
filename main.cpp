
#include <iostream>
#include "matrix.hpp"
#include "matrix_op.hpp"

int main()
{
    Matrix<float> mat1(3, 3);

    mat1[0][0] = 2.0;
    mat1[0][1] = 7.0;
    mat1[0][2] = 6.0;
    mat1[1][0] = 9.0;
    mat1[1][1] = 5.0;
    mat1[1][2] = 1.0;
    mat1[2][0] = 4.0;
    mat1[2][1] = 3.0;
    mat1[2][2] = 8.0;

    Matrix<float> mat3(1, 3);
    mat3[0][0] = 0.0;
    mat3[1][0] = 1.0;
    mat3[2][0] = 0.0;

    Matrix<float> output;
    
    inverseMatrix(mat1, output);

    std::cout << "mat1:" << std::endl << mat1 << std::endl;
    std::cout << "output:" << std::endl << output << std::endl;
}
