#include <iostream>
#include <pthread.h>
#include <thread>
#include "matrix.hpp"

template <typename T>
void *inverseMatrix_t(void *arg)
{
    threadData<T> *data;
    data = (threadData<T> *) arg;

    Matrix<T>& mat_lu = *data->mat1;
    Matrix<T>& mat_pb = *data->mat2;
    Matrix<T>& output = *data->output;

    for(int output_col=data->start; output_col < data->end; output_col++)
    {
        //then, with the substitution Ux = y, the equasion becomes Ly = Pb
        for(int i=0; i<mat_lu.getHeight(); i++)
        {
            output[i][output_col] = mat_pb[i][output_col];
            for(int j=0; j<i; j++)
            {
                output[i][output_col] = output[i][output_col] - mat_lu[i][j]*output[j][output_col];
            }
        }

        //after solving Ly = Pb and finding the value of 'y', the equasion Ux = b is solved
        int last_col = mat_lu.getWidth()-1;
        int last_row = mat_lu.getHeight()-1;
        for(int i=last_row; i>=0; i--)
        {
            output[i][output_col] = output[i][output_col];

            for(int j=last_row; j>i; j--)
            {
                output[i][output_col] = output[i][output_col] - mat_lu[i][j]*output[j][output_col];
            }
            output[i][output_col] = output[i][output_col]/mat_lu[i][i];
        }
    }

    pthread_exit(NULL);
}

template <typename T>
void inverseMatrix(Matrix<T> &mat1, Matrix<T> &output)
{
    int num_threads = std::thread::hardware_concurrency();

    if(num_threads == 0)
    {
        num_threads = 4;
    }


    if(mat1.isSquare() and mat1.determinant()!=0)
    {
        output.setSize(mat1.getWidth(), mat1.getHeight());

        Matrix<T> mat_lu(mat1.getWidth(), mat1.getHeight());
        Matrix<T> mat_p(mat1.getWidth(), mat1.getHeight());

        decompositionLUP(mat1, mat_lu, mat_p);

        int num_operations = mat1.getWidth();

        pthread_t threads[num_threads];
        threadData<T> data[num_threads];

        for(int i = 0; i<num_threads; i++)
        {
            data[i] = threadData<T>();

            data[i].start = num_operations*i/num_threads;
            data[i].end = num_operations*(i+1)/num_threads;

            data[i].mat1 = &mat_lu;
            data[i].mat2 = &mat_p;
            data[i].output = &output;

            int rc = pthread_create(&threads[i], NULL, inverseMatrix_t<T>, (void *)&data[i]);
            if(rc)
            {
                std::cerr << "Erro ao criar a thread" << std::endl;
                throw(1);
            }
        }

        for(int i=0; i<num_threads; i++)
        {
            pthread_join(threads[i], NULL);
        }
    }
    else
    {
        std::cerr << "Essa matriz não possui inversa." << std::endl;
        throw(1);
    }
}