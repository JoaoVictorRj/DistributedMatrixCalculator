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
    ThreadData<T> data[num_threads];

    for(int i = 0; i<num_threads; i++)
    {
        data[i] = ThreadData<T>();

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


            if(max_pivot != i)
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
    // through the LUP decoposition method and fills the 'output' matrix (also N x 1) with it's solution

    if(mat_a.isSquare() && (mat_a.getHeight() == mat_b.getHeight()) && (mat_b.getWidth() == 1))
    {        
        Matrix<T> mat_y(1, mat_a.getHeight());

        output.setSize(1, mat_b.getHeight());

        Matrix<T> mat_lu(mat_a.getWidth(), mat_a.getHeight());
        Matrix<T> mat_p(mat_a.getWidth(), mat_a.getHeight());

        //the equasion starts as Ax = b

        decompositionLUP(mat_a, mat_lu, mat_p);

        //after LUP decomposition, the equasion becomes LUx = Pb, or L(Ux) = Pb

        Matrix<T> mat_pb(1, mat_a.getHeight());
        multiplyMatrix(mat_p, mat_b, mat_pb);

        //then, with the substitution Ux = y, the equasion becomes Ly = Pb
        for(int i=0; i<mat_lu.getHeight(); i++)
        {
            mat_y[i][0] = mat_pb[i][0];
            for(int j=0; j<i; j++)
            {
                mat_y[i][0] = mat_y[i][0] - mat_lu[i][j]*mat_y[j][0];
            }
        }

        //after solving Ly = Pb and finding the value of 'y', the equasion Ux = b is solved
        int last_col = mat_lu.getWidth()-1;
        int last_row = mat_lu.getHeight()-1;
        for(int i=last_row; i>=0; i--)
        {
            output[i][0] = mat_y[i][0];

            for(int j=last_row; j>i; j--)
            {
                output[i][0] = output[i][0] - mat_lu[i][j]*output[j][0];
            }
            output[i][0] = output[i][0]/mat_lu[i][i];
        }
        //finally, after solving Ux = b, the 'output' matrix contains the values of 'x' that satisfy
        // the coeficients in 'A'
    }
    else
    {
        std::cerr << "Invalid matrixes" << std::endl;
        throw(1);
    }
}

template <typename T>
void *inverseMatrix_t(void *arg)
{
    ThreadData<T> *data;
    data = (ThreadData<T> *) arg;

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

    //(to-do) check if the determinant != 0
    if(mat1.isSquare())
    {
        output.setSize(mat1.getWidth(), mat1.getHeight());

        Matrix<T> mat_lu(mat1.getWidth(), mat1.getHeight());
        Matrix<T> mat_p(mat1.getWidth(), mat1.getHeight());

        decompositionLUP(mat1, mat_lu, mat_p);

        int num_operations = mat1.getWidth();

        pthread_t threads[num_threads];
        ThreadData<T> data[num_threads];

        for(int i = 0; i<num_threads; i++)
        {
            data[i] = ThreadData<T>();

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
