#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cassert>
#include <cmath>

#include <vector>
#include <initializer_list>
#include <limits>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <type_traits>

template <typename Type, std::enable_if_t<std::is_floating_point_v<Type>, int> = 0>
bool equal (const Type& a, const Type& b)
{
    return std::abs(a - b) < std::numeric_limits<Type>::epsilon();
}

template <typename Type, std::enable_if_t<!std::is_floating_point_v<Type>, int> = 0>
bool equal (const Type& a, const Type& b)
{
    return a == b;
}

class Matrix
{
public:
    using Initializer = 
        std::initializer_list<std::initializer_list<double>>;

public:
    class Dimension
    {
    public:
        bool operator== (const Dimension& rhv) const noexcept
        {
            return m == rhv.m && n == rhv.n;
        }

        bool operator!= (const Dimension& rhv) const noexcept
        {
            return !((*this) == rhv);
        }

    public:
        size_t m = 0;
        size_t n = 0;
    };

public:
    Matrix () = default;
    Matrix (size_t m, size_t n) :
        _matrix(m, std::vector<double>(n, 0.0)),
        _dimension({m, n}) {}

    Matrix (Initializer initializer) :
        Matrix(initializer.size(), initializer.begin()->size())
    {
        std::copy(initializer.begin(), initializer.end(), _matrix.begin());
    }

public:
    const Dimension& dimension () const noexcept { return _dimension; }
    void RowSwap (size_t fidx, size_t sidx) { std::swap(_matrix[fidx], _matrix[sidx]); }
    void RemoveZerosRows ();
    void transpose ();

public:
    double& operator() (size_t m, size_t n) noexcept { return _matrix[m][n]; }
    const double& operator() (size_t m, size_t n) const noexcept { return _matrix[m][n]; }

private:
    std::vector<std::vector<double>> _matrix;
    Dimension _dimension;
};



Matrix& operator+= (Matrix& lhv, const Matrix& rhv) noexcept;
Matrix& operator-= (Matrix& lhv, const Matrix& rhv) noexcept;
Matrix& operator*= (Matrix& lhv, const Matrix& rhv);

Matrix operator+ (const Matrix& lhv, const Matrix& rhv);
Matrix operator- (const Matrix& lhv, const Matrix& rhv);
Matrix operator* (const Matrix& lhv, const Matrix& rhv);

std::ostream& operator<< (std::ostream& out, const Matrix& matrix);

double determinant (const Matrix& matrix);

void gauss (Matrix& matrix);

#endif//MATRIX_HPP