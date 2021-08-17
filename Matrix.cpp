#include "Matrix.hpp"

void Matrix::RemoveZerosRows ()
{
    _matrix.erase(std::remove_if(_matrix.begin(), _matrix.end(), [](const auto& row) 
    {
        return std::all_of(row.begin(), row.end(), [](const auto& element) 
        {
            return equal(element, 0.0);
        });
    }), _matrix.end());

    _dimension.m = _matrix.size();
}

void Matrix::transpose ()
{
    Matrix tmp(_dimension.n, _dimension.m);

    for (size_t i = 0; i < tmp.dimension().m; ++i)
        for (size_t j = 0; j < tmp.dimension().n; ++j)
            tmp(i, j) = (*this)(j, i);

    *this = std::move(tmp);
}

Matrix& operator+= (Matrix& lhv, const Matrix& rhv) noexcept
{
    assert(lhv.dimension() == rhv.dimension());
 
    for (size_t i = 0; i < lhv.dimension().m; ++i)
        for (size_t j = 0; j < lhv.dimension().n; ++j)
            lhv(i, j) += rhv(i, j);

    return lhv;
}

Matrix& operator-= (Matrix& lhv, const Matrix& rhv) noexcept
{
    assert(lhv.dimension() == rhv.dimension());

    for (size_t i = 0; i < lhv.dimension().m; ++i)
        for (size_t j = 0; j < lhv.dimension().n; ++j)
            lhv(i, j) -= rhv(i, j);

    return lhv;
}

Matrix& operator*= (Matrix& lhv, const Matrix& rhv)
{
    assert(lhv.dimension().n == rhv.dimension().m);
    Matrix tmp(lhv.dimension().m, rhv.dimension().n);
    
    for (size_t i = 0; i < tmp.dimension().m; ++i)
        for (size_t j = 0; j < tmp.dimension().n; ++j)
            for (size_t k = 0; k < lhv.dimension().n; ++k)
                tmp(i, j) += lhv(i, k) * rhv(k, j);

    return lhv = std::move(tmp);
}

Matrix operator+ (const Matrix& lhv, const Matrix& rhv)
{
    auto tmp = lhv;
    return tmp += rhv;
}

Matrix operator- (const Matrix& lhv, const Matrix& rhv)
{
    auto tmp = lhv;
    return tmp -= rhv;
}

Matrix operator* (const Matrix& lhv, const Matrix& rhv)
{
    auto tmp = lhv;
    return tmp *= rhv;
}

void ForwardStroke (Matrix& matrix)
{
    auto pivoting = [&matrix](size_t row, size_t col)
    {
        size_t max = row;

        for (size_t cur = row + 1; cur < matrix.dimension().m; ++cur)
            if (std::abs(matrix(max, col)) < std::abs(matrix(cur, col)))
                max = cur;

        return max;
    };

    size_t col = 0; 
    size_t row = 0;
    for (; col < matrix.dimension().n && row < matrix.dimension().m; ++col)
    {
        auto max = pivoting(row, col);
        if (equal(matrix(max, col), 0.0))
            continue;

        matrix.RowSwap(row, max);
        for (size_t i = row + 1; i < matrix.dimension().m; ++i)
        {
            const auto FIRST_ELEMENT = matrix(i, col);
            for (size_t j = col; j < matrix.dimension().n; ++j)
                matrix(i, j) -= matrix(row, j) / matrix(row, col) * FIRST_ELEMENT; 
        }
        ++row;
    }
}

void gauss (Matrix& matrix)
{
    ForwardStroke(matrix);
    matrix.RemoveZerosRows();
}

double determinant (const Matrix& matrix)
{
    assert(matrix.dimension().m == matrix.dimension().n);

    auto tmp = matrix;
    ForwardStroke(tmp);

    double res = 1;
    size_t row = 0;
    size_t col = 0;
    while (row < tmp.dimension().m && col < tmp.dimension().n)
    {
        res *= tmp(row, col);
        ++row;
        ++col;
    }

    return res;
}

std::ostream& operator<< (std::ostream& out, const Matrix& matrix)
{
    for (size_t i = 0; i < matrix.dimension().m; ++i)
    {
        for (size_t j = 0; j < matrix.dimension().n; ++j)
        {
            out << std::setw(20) << matrix(i, j);
        }
        out << '\n';
    }
    return out;
}