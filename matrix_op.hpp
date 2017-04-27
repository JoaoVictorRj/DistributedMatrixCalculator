#include <iostream>
#include <pthread.h>

#define NUM_THREADS 8

void *add_t(void *arg)
{
    // int id = (int)arg;
    //int start = std::max(width/NUM_THREADS, height/NUM_THREADS)*id;
    //int end = start + std::max(width/NUM_THREADS, height/NUM_THREADS);
    // std::cout << id << std::endl;
    pthread_exit(NULL);
}

template <typename T>
void add(Matrix<T> &mat1, Matrix<T> &mat2, Matrix<T> &output)
{
    if((mat1.getWidth() != mat2.getWidth()) || (mat1.getHeight() != mat2.getHeight()))
    {
        std::cout << "Matrix sizes must be equal" << std::endl;
        throw(1);
    }


    pthread_t threads[NUM_THREADS];
    int rc;   

    for (int t=0; t<NUM_THREADS; t++)
    {
        rc = pthread_create(&threads[t], NULL,add_t, (void *) t);
        if (rc)
            std::cout << "Erro ao criar a thread" << std::endl;
    }
}
