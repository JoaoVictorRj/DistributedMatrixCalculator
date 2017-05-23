
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

    Matrix<float> matlu(3,3);
    Matrix<float> matp(3,3);

    std::cout << "mat1:" << std::endl << mat1 << std::endl;

    decompositionLUP(mat1, matlu, matp);

    std::cout << "mat1:" << std::endl << matlu << std::endl;


    std::cout << "matlu:" << std::endl << matlu << std::endl;

    std::cout << "matp:" << std::endl << matp << std::endl;
}
