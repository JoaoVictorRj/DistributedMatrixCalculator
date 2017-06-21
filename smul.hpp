#include <iostream>
#include <pthread.h>
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
    output.setWidth(mat1.getWidth());
    output.setHeight(mat1.getHeight());

    int num_operations = output.getWidth() * output.getHeight();

    pthread_t thread[NUM_THREADS];
    int rc;

    threadData<T> data[NUM_THREADS];

    for (int t=0; t<NUM_THREADS; t++)
    {
        data[t] = threadData<T>();

        data[t].mat1 = &mat1;
        data[t].m = m;
        data[t].output = &output; 

		data[t].start = num_operations*t/NUM_THREADS;
		data[t].end = num_operations*(t+1)/NUM_THREADS;

        rc = pthread_create(&thread[t], NULL, smul_t<T>, (void *) &data[t]);
        if (rc)
            std::cout << "Erro ao criar a thread" << std::endl;
    }

    for (int t=0; t<NUM_THREADS; t++){
        pthread_join(thread[t], NULL);
    }

}