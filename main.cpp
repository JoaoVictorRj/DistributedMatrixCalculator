#include "matrix.hpp"

int main(){

    Matrix<int> mat1(2,2);
    mat1[0][0] = 13;
    mat1[0][1] = 2;
    mat1[1][0] = 4;
    mat1[1][1] = 23;
    Matrix<int> mat2(2,2,3);
    mat1 += mat2;

    std::cout << mat1 << std::endl;
    
    return 0;
}
