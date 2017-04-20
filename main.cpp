#include "matrix.hpp"

int main(){
    Matrix<int> mat(10,10);

    mat[2][2] = 5;

    std::cout << mat[2][2] << std::endl;

    return 0;
}
