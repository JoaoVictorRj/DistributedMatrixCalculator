#include "matrix.hpp"

int main(){

    Matrix<int> mat1(2,2);
    mat1[0][0] = 13;
    mat1[0][1] = 2;
    mat1[1][0] = 4;
    mat1[1][1] = 23;

    std::cout << mat1 << std::endl;

    mat1.setWidth(4);
    mat1.setHeight(1);

    std::cout << mat1 << std::endl;
    
    return 0;
}
