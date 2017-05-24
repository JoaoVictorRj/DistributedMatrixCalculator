
#include <iostream>
#include "matrix.hpp"
#include "matrix_op.hpp"

int main()
{
    Matrix<float> mat1(3, 3);

    // mat1[0][0] = 2.0;
    // mat1[0][1] = 7.0;
    // mat1[0][2] = 6.0;
    // mat1[1][0] = 9.0;
    // mat1[1][1] = 5.0;
    // mat1[1][2] = 1.0;
    // mat1[2][0] = 4.0;
    // mat1[2][1] = 3.0;
    // mat1[2][2] = 8.0;
    mat1[0][0] = 25.0;
    mat1[0][1] = 5.0;
    mat1[0][2] = 1.0;
    mat1[1][0] = 64.0;
    mat1[1][1] = 8.0;
    mat1[1][2] = 1.0;
    mat1[2][0] = 144.0;
    mat1[2][1] = 12.0;
    mat1[2][2] = 1.0;

    Matrix<float> mat2(1, 3);
    mat2[0][0] = 1.0;
    mat2[1][0] = 0.0;
    mat2[2][0] = 0.0;

    std::cout << "mat1:" << std::endl << mat1 << std::endl;
    std::cout << "mat2:" << std::endl << mat2 << std::endl;

    Matrix<float> output(1, 3);
    solveLinear(mat1, mat2, output);

    std::cout << "output:" << std::endl << output << std::endl;

}
