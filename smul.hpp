#include <iostream>
#include <pthread.h>
#include <thread>
#include "matrix.hpp"

template <typename T>
void *smul_t(void *arg)
{
    threadData<T> *data;
	data = (threadData<T> *) arg;

    Matrix<T>& mat1 = *data->mat1;
    double m = data->m;
    Matrix<T>& output = *data->output;

	for (int i=data->start; i<data->end; i++)
	{
		int row = i/mat1.getWidth();
		int col = i%mat1.getWidth();

		output[row][col] = mat1[row][col] * m;
	}

    pthread_exit(NULL);
}

template <typename T>
void smul(Matrix<T> &mat1, double m, Matrix<T> &output)
{
	int num_threads = std::thread::hardware_concurrency();

    if(num_threads == 0)
    {
        num_threads = 4;
    }

    output.setWidth(mat1.getWidth());
    output.setHeight(mat1.getHeight());

    int num_operations = output.getWidth() * output.getHeight();

    pthread_t thread[num_threads];
    int rc;

    threadData<T> data[num_threads];

    for (int t=0; t<num_threads; t++)
    {
        data[t] = threadData<T>();

        data[t].mat1 = &mat1;
        data[t].m = m;
        data[t].output = &output; 

		data[t].start = num_operations*t/num_threads;
		data[t].end = num_operations*(t+1)/num_threads;

        rc = pthread_create(&thread[t], NULL, smul_t<T>, (void *) &data[t]);
        if (rc)
            std::cout << "Erro ao criar a thread" << std::endl;
    }

    for (int t=0; t<num_threads; t++){
        pthread_join(thread[t], NULL);
    }

}