template <typename T> class Matrix{
    public:
    template Matrix();   //matrix class constructor

    Matrix<T> operator[](int i) const;   //

    void add(Matrix &other);   //sum other matrix to this matrix

    void sub(Matrix &other);   //subtracts other matrix to this matrix

    void det();   //calculates the matrix's determinant

    void inv();   //calculates the matrix's inverse

};
