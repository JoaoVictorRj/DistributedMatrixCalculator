#include "ufrjmatrix.hpp"
#include <iostream>
#include <typeinfo>
#include <time.h>

static double TimeSpecToSeconds(struct timespec* ts)
	{
		return (double)ts->tv_sec + (double)ts->tv_nsec / 1000000000.0;
	}

int main(int argc, char* argv[]){

	struct timespec start;
	struct timespec end;
	double timeElapsed;
    int w,h,m;
    w = atoi(argv[2]);
    h = atoi(argv[3]);
    m = atoi(argv[1]);

    Matrix<int> M(h,w);

    for (int i=0;i<h;i++){
        for (int j=0;j<w;j++){
            M[i][j]=atoi(argv[4+w*i+j]);
        }
    }

    std::cout<<M<<std::endl;

    Matrix<int> mat1(10,10,32);

    clock_gettime(CLOCK_MONOTONIC, &start);
    smul(M,m,M);
    clock_gettime(CLOCK_MONOTONIC, &end);

    timeElapsed = TimeSpecToSeconds(&end) - TimeSpecToSeconds(&start);

    std::cout << "------------" << std::endl << std::endl << M << std::endl;

    std::cout << "Time elapsed: " << timeElapsed << std::endl;

    return 0;
}
