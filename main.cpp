
#include <iostream>
#include "matrix.hpp"
#include "matrix_op.hpp"

int main()
{
    Matrix<int> mat1(3, 2, 3);
    Matrix<int> mat2(1, 3, 2);
    Matrix<int> mat3(5,5);


    std::cout << mat1 << std::endl;
    std::cout << mat2 << std::endl;

    multiplyMatrix(mat1, mat2, mat3);

    std::cout << mat3 << std::endl;
}
