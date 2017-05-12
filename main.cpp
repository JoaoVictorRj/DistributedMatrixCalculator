#include "matrix.hpp"
#include <iostream>
#include <typeinfo>

using namespace std;

int main(){

    Matrix<int> mat1(2,2);
    Matrix<int> mat2(2,2);

    cout << typeid(mat1).name() << endl;

    void* mymatrix = (void*) mat1;

    cout << typeid(mymatrix).name() << endl;

    mat2 = (Matrix<int>) mymatrix;    

    //Matrix<int>* mat2 = const_cast<Matrix<int>*>( static_cast<const Matrix<int>*>(mymatrix) );

    cout << typeid(mat2).name() << endl;

    return 0;
}
