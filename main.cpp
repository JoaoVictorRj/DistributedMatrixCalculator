#include "matrix.hpp"

int main(){
    Matrix<int> mat1(5,5,13);
    Matrix<int> mat2(5,5,17);
    Matrix<int> mat3(5,5);

    std::cout << mat2[2][2] << std::endl;

    mat3 = mat1 + mat2;

    std::cout << mat3 << std::endl;

    return 0;
}
