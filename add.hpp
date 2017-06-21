#include <iostream>
#include <pthread.h>
#include <thread>
#include "matrix.hpp"

template <typename T>
void *add_t(void *arg)
{
    threadData<T> *data;
	data = (threadData<T> *) arg;

    Matrix<T>& mat1 = *data->mat1;
    Matrix<T>& mat2 = *data->mat2;
    Matrix<T>& output = *data->output;

	for (int i=data->start; i<data->end; i++)
	{
		int row = i/mat1.getWidth();
		int col = i%mat1.getWidth();

		output[row][col] = mat1[row][col] + mat2[row][col];
	}

    pthread_exit(NULL);
}

template <typename T>
void add(Matrix<T> &mat1, Matrix<T> &mat2, Matrix<T> &output)
{
	int num_threads = std::thread::hardware_concurrency();

    if(num_threads == 0)
    {
        num_threads = 4;
    }

	if((mat1.getWidth() != mat2.getWidth()) || (mat1.getHeight() != mat2.getHeight()))
    {
        std::cout << "Matrix sizes must be equal" << std::endl;
        throw(1);
    }

    output.setWidth(mat2.getWidth());
    output.setHeight(mat2.getHeight());

    int num_operations = output.getWidth() * output.getHeight();

    pthread_t thread[num_threads];
    int rc;

    threadData<T> data[num_threads];

    for (int t=0; t<num_threads; t++)
    {
        data[t] = threadData<T>();

        data[t].mat1 = &mat1;
        data[t].mat2 = &mat2;
        data[t].output = &output; 

		data[t].start = num_operations*t/num_threads;
		data[t].end = num_operations*(t+1)/num_threads;

        rc = pthread_create(&thread[t], NULL, add_t<T>, (void *) &data[t]);
        if (rc)
            std::cout << "Erro ao criar a thread" << std::endl;
    }

    for (int t=0; t<num_threads; t++){
        pthread_join(thread[t], NULL);
    }

}