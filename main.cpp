
#include <iostream>
#include "matrix.hpp"
#include "matrix_op.hpp"

int main()
{
    Matrix<int> mat1(3, 3);
    Matrix<int> mat2(3, 3);
    Matrix<int> mat3(5,5);

    mat1[0][0] = 1;
    mat1[0][1] = 2;
    mat1[0][2] = 3;
    mat1[1][0] = 4;
    mat1[1][1] = 5;
    mat1[1][2] = 6;
    mat1[2][0] = 7;
    mat1[2][1] = 8;
    mat1[2][2] = 9;

    mat2[0][0] = 10;
    mat2[0][1] = 20;
    mat2[0][2] = 30;
    mat2[1][0] = 40;
    mat2[1][1] = 50;
    mat2[1][2] = 60;
    mat2[2][0] = 70;
    mat2[2][1] = 80;
    mat2[2][2] = 90;

    std::cout << mat1 << std::endl;
    std::cout << mat2 << std::endl;

    multiplyMatrix(mat1, mat2, mat3);

    std::cout << mat3 << std::endl;

    mat3.swapRows(2,0);
    std::cout << mat3 << std::endl;

    mat3.identity();

    std::cout << mat3 << std::endl;
}
