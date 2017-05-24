#include <iostream>
#include <pthread.h>
#include <thread>

//#define num_threads 8

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
    output.setWidth(mat2.getWidth());
    output.setHeight(mat1.getHeight());

    int num_operations = output.getWidth() * output.getHeight();

    pthread_t threads[num_threads];
    ThreadData<T> data[num_threads];

    int rc;
    for(int i = 0; i<num_threads; i++)
    {
        data[i] = ThreadData<T>();

        data[i].start = num_operations*i/num_threads;
        data[i].end = num_operations*(i+1)/num_threads;

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

    for(int i=0; i<num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

template<typename T>
void decompositionLUP(Matrix<T> &mat_a, Matrix<T> &mat_lu, Matrix<T> &mat_p)
{
    if(mat_a.isSquare() && mat_lu.isSquare() && mat_p.isSquare() &&
      (mat_a.getWidth() == mat_lu.getWidth()) && 
      (mat_a.getWidth() == mat_p.getWidth()) )
    {
        mat_p.identity();
        //(to-do) evaluate if this copy is necessary and if so, optimize it
        mat_lu = mat_a;

        for(int i=0; i<mat_lu.getHeight() -1; i++)
        {
            int max_pivot = i;
            for(int j = i+1; j<mat_lu.getHeight(); j++)
            {
                if(abs(mat_lu[j][i]) > abs(mat_lu[max_pivot][i]))
                {
                    max_pivot = j;
                }
            }

            //(to-do) pivoting disabled for debugging, re-enable it when debuging is done
            //if(max_pivot != i)
            if(0)
            {
                mat_lu.swapRows(max_pivot,i);
                mat_p.swapRows(max_pivot,i);
            }

            for(int j=i+1; j<mat_lu.getHeight(); j++)
            {
                mat_lu[j][i] = mat_lu[j][i]/mat_lu[i][i];
                for(int k=i+1; k<mat_lu.getWidth(); k++)
                {
                    mat_lu[j][k] = mat_lu[j][k] - mat_lu[i][k]*mat_lu[j][i];
                }
            }
        }
    }
    else
    {
        std::cerr << "In a LUP decomposition all matrixes must be square" << std::endl;
        throw(1);
    }
}

template<typename T>
void solveLinear(Matrix<T> &mat_a, Matrix<T> &mat_b, Matrix<T> &output)
{
    //Solves a linear system of the type Ax = B, where A is a N x N matrix and B is a N x 1 column matrix
    // and fills the 'output' matrix (also N x 1) with the solution

    //(to-do) Solving a linear system shouldnt involve a lu decomposition, this must be moved to the 
    // inverse matrix function
    if(mat_a.isSquare() && (mat_a.getHeight() == mat_b.getHeight()) && (mat_b.getWidth() == 1))
    {        
        output.setHeight(mat_b.getHeight());
        output.setWidth(1);

        Matrix<T> mat_lu(mat_a.getWidth(), mat_a.getHeight());
        Matrix<T> mat_p(mat_a.getWidth(), mat_a.getHeight());

        decompositionLUP(mat_a, mat_lu, mat_p);

        for(int i=0; i<mat_lu.getHeight(); i++)
        {
            output[i][0] = mat_b[i][0];
            for(int j=0; j<i; j++)
            {
                output[i][0] = output[i][0] - mat_lu[i][j];
            }

            for(int j=i+1; j<mat_lu.getHeight(); j++)
            {
                mat_lu[j][i] = mat_lu[j][i]*output[i][0];
            }
        }
    }
    else
    {
        std::cerr << "Invalid matrixes" << std::endl;
        throw(1);
    }
}

template <typename T>
void inverseMatrix(Matrix<T> &mat1, Matrix<T> &output)
{
    int num_threads = std::thread::hardware_concurrency();

    if(num_threads == 0)
    {
        num_threads = 4;
    }

    //(to-do) check if the determinand != 0
    if(mat1.getWidth() != mat1.getHeight())
    {
        std::cerr << "Essa matriz não possui inversa." << std::endl;
        throw(1);
    }

    Matrix<T> mat_lu(mat1.getWidth(), mat1.getHeight());
    Matrix<T> mat_p(mat1.getWidth(), mat1.getHeight());

    decompositionLUP(mat1, mat_lu, mat_p);
}