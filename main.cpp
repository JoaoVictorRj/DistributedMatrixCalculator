#include "ufrjmatrix.hpp"
#include <iostream>
#include <typeinfo>
#include <time.h>

static double TimeSpecToSeconds(struct timespec* ts)
	{
		return (double)ts->tv_sec + (double)ts->tv_nsec / 1000000000.0;
	}

int main(){

	struct timespec start;
	struct timespec end;
	double timeElapsed;

    Matrix<int> mat1(10,10,24);
    Matrix<int> mat2(10,10,12);
    Matrix<int> mat3(1,1);

    clock_gettime(CLOCK_MONOTONIC, &start);
    add(mat1,mat2,mat3);
    clock_gettime(CLOCK_MONOTONIC, &end);

    timeElapsed = TimeSpecToSeconds(&end) - TimeSpecToSeconds(&start);

    std::cout << "------------" << std::endl << mat3 << std::endl;

    std::cout << "Time elapsed: " << timeElapsed << std::endl;

    return 0;
}
