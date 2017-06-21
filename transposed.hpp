#include <iostream>
#include <pthread.h>
#include "matrix.hpp"

template <typename T>
void *transposed_t(void *arg)
{
    threadData<T> *data;
	data = (threadData<T> *) arg;

    Matrix<T>& mat1 = *data->mat1;
    Matrix<T>& output = *data->output;

	for (int i=data->start; i<data->end; i++)
	{
		int row = i/mat1.getWidth();
		int col = i%mat1.getWidth();

        if (output[col][row] != mat1[row][col])
		output[col][row] = mat1[row][col];
	}

    pthread_exit(NULL);
}

template <typename T>
void transposed(Matrix<T> &mat1, Matrix<T> &output)
{
    output.setWidth(mat1.getHeight());
    output.setHeight(mat1.getWidth());

    int num_operations = output.getWidth() * output.getHeight();

    pthread_t thread[NUM_THREADS];
    int rc;

    threadData<T> data[NUM_THREADS];

    for (int t=0; t<NUM_THREADS; t++)
    {
        data[t] = threadData<T>();

        data[t].mat1 = &mat1;
        data[t].output = &output; 

		data[t].start = num_operations*t/NUM_THREADS;
		data[t].end = num_operations*(t+1)/NUM_THREADS;

        rc = pthread_create(&thread[t], NULL, transposed_t<T>, (void *) &data[t]);
        if (rc)
            std::cout << "Erro ao criar a thread" << std::endl;
    }

    for (int t=0; t<NUM_THREADS; t++){
        pthread_join(thread[t], NULL);
    }

}
