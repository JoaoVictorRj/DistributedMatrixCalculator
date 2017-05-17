#include <iostream>
#include <pthread.h>

#define NUM_THREADS 8

template <typename T>
struct ThreadData
{
    int start;
    int end;
    Matrix<T> *mat1;
    Matrix<T> *mat2;
    Matrix<T> *output;

    ThreadData() {}
};

template <typename T>
void *multiplyMatrix_t(void *arg)
{
    ThreadData<T> *data;
    data = (ThreadData<T> *) arg;

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

    if(mat2.getHeight() != mat1.getWidth())
    {
        std::cerr << "Essas matrizes não podem ser multiplicadas nesta ordem." << std::endl;
        throw(1);
    }

    //(to-do) check how costly is this resizing and if it can be paralelized
    output.setWidth(mat2.getWidth());
    output.setHeight(mat1.getHeight());

    int num_operations = output.getWidth() * output.getHeight();

    pthread_t threads[NUM_THREADS];
    ThreadData<T> data[NUM_THREADS];

    int rc;
    for(int i = 0; i<NUM_THREADS; i++)
    {
        data[i] = ThreadData<T>();

        data[i].start = num_operations*i/NUM_THREADS;
        data[i].end = num_operations*(i+1)/NUM_THREADS;

        data[i].mat1 = &mat1;
        data[i].mat2 = &mat2;
        data[i].output = &output;      

        rc = pthread_create(&threads[i], NULL, multiplyMatrix_t<T>, (void *)&data[i]);
        if(rc)
        {
            std::cerr << "Erro ao criar a thread" << std::endl;
            throw(1);
        }
    }

    for(int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}
