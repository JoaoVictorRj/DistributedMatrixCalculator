#include "matrix.hpp"
#include <iostream>
#include <typeinfo>

int main(){

    Matrix<int> mat1(3,3,11);
    
    std::cout << mat1 << std::endl;

    void *pointer = (void*)mat1;


    mat1[1][1] = 3;
    
    Matrix<int> mat2;

    mat2 = (Matrix<int>)mat1;

    std::cout << "------------" << std::endl << mat2 << std::endl;

    return 0;
}
