#include <iostream>
#include <pthread.h>
#include <thread>
#include "matrix.hpp"

template <typename T>
void *multiplyMatrix_t(void *arg)
{
    threadData<T> *data;
    data = (threadData<T> *) arg;

    Matrix<T>& mat1 = *data->mat1;
    Matrix<T>& mat2 = *data->mat2;
    Matrix<T>& output = *data->output;

    int num_operations = output.getWidth() * output.getHeight();

    for(int i=data->start; i<data->end; i++)
    {
        
        int row = i/output.getWidth();
        int col = i%output.getWidth();

        int sum = 0;
        for(int k=0; k< mat1.getWidth(); k++)
        {
            sum += mat1[row][k] * mat2[k][col];
        }
        output[row][col] = sum;
    }

    pthread_exit(NULL);
}

template <typename T>
void multiplyMatrix(Matrix<T> &mat1, Matrix<T> &mat2, Matrix<T> &output)
{
    //(to-do) currently output must be diferent from mat1 and mat2
    int num_threads = std::thread::hardware_concurrency();

    if(num_threads == 0)
    {
        num_threads = 4;
    }

    if(mat2.getHeight() != mat1.getWidth())
    {
        std::cerr << "Essas matrizes não podem ser multiplicadas nesta ordem." << std::endl;
        throw(1);
    }

    //(to-do) check how costly is this resizing and if it can be paralelized
    output.setSize(mat2.getWidth(), mat1.getHeight());

    int num_operations = output.getWidth() * output.getHeight();

    pthread_t threads[num_threads];
    threadData<T> data[num_threads];

    for(int i = 0; i<num_threads; i++)
    {
        data[i] = threadData<T>();

        data[i].start = num_operations*i/num_threads;
        data[i].end = num_operations*(i+1)/num_threads;

        data[i].mat1 = &mat1;
        data[i].mat2 = &mat2;
        data[i].output = &output;      

        int rc = pthread_create(&threads[i], NULL, multiplyMatrix_t<T>, (void *)&data[i]);
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
