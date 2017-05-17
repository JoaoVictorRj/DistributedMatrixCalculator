#include <iostream>
#include <pthread.h>

//(to-do) Change the algorithm to take any amount of threads
//#define NUM_THREADS 8

template <typename T>
struct ThreadData
{
    int id;
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

    if (mat2.getWidth()<= mat1.getHeight())
    {
        for (int j=0; j< mat2.getWidth(); j++)
        {
            int sum=0;
            for (int k=0; k< mat1.getWidth(); k++)
            {
                sum += mat1[data->id][k] * mat2[k][j];
            }
            output[data->id][j] = sum;
        }
    }
    else
    {
        for (int j=0; j< mat1.getHeight(); j++)
        {
            int sum=0;
            for (int k=0; k< mat1.getWidth(); k++)
            {
                sum += mat1[j][k]*mat2[k][data->id];
            }
            output[j][data->id] = sum;
        }
    }
    pthread_exit(NULL);
}

template <typename T>
void multiplyMatrix(Matrix<T> &mat1, Matrix<T> &mat2, Matrix<T> &output)
{

    if (mat2.getHeight() != mat1.getWidth())
    {
        std::cerr << "Essas matrizes não podem ser multiplicadas nesta ordem." << std::endl;
        return;
    }

    output.setWidth(mat2.getWidth());
    output.setHeight(mat1.getHeight());

    int NUM_THREADS = std::max(mat1.getHeight(), mat2.getWidth());
    pthread_t threads[NUM_THREADS];
    ThreadData<T> data[NUM_THREADS];

    int rc;
    for (int i = 0; i<NUM_THREADS; i++)
    {
        data[i] = ThreadData<T>();
        data[i].id = i;
        data[i].mat1 = &mat1;
        data[i].mat2 = &mat2;
        data[i].output = &output;      

        rc = pthread_create(&threads[i], NULL, multiplyMatrix_t<T>, (void *)&data[i]);
        if (rc)
        {
            std::cerr << "Erro ao criar a thread" << std::endl;
            return;
        }
    }

    for(int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}
