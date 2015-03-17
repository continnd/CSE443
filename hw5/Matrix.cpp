#include "Matrix.h"
#include <algorithm>
#include <stdexcept>
#include <iterator>

int Matrix::blockSize = 1;
Matrix::Matrix() {
    data = TwoDVec(1);
    data[0] = std::vector<Val>(1);
}

Matrix::Matrix(int rows, int cols) {
    data = TwoDVec(rows, std::vector<Val>(cols, 0));
}

Matrix::Matrix(int rows, int cols, std::function<Val(int, int)> init) {
    data = TwoDVec(rows, std::vector<Val>(cols, 0));
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            data[i][j] = init(i, j);
        }
    }
}

Matrix::Matrix(const Matrix& src) : data(src.data) {
}

Matrix::Matrix(Matrix&& src) : data(std::move(src.data)) {
}

Matrix Matrix::operator+(const Matrix& src) const throw(std::exception) {
    Val rows = data.size();
    Val cols = data[0].size();

    if (rows != src.data.size() || cols != src.data[0].size()) {
        throw std::invalid_argument("Matrix sizes are incompatible");
    }

    Matrix res(rows, cols);

    for (size_t i = 0; i < rows; i++) {
        res.data[i].data()[0:cols] = data[i].data()[0:cols] + src.data[i].data()[0:cols];
    }
    return res;
}

Matrix Matrix::operator*(const Matrix& src) const throw(std::exception) {
    Val rows = data.size();
    Val elements = data[0].size();
    Val cols = src.data[0].size();

    if (elements != src.data.size()) {
        throw std::invalid_argument("Matrix sizes are incompatible");
    }

    Matrix res(rows, cols);

    for (size_t i = 0; i < rows; i += blockSize) {
        for (size_t j = 0; j < rows; j += blockSize) {
            for (size_t k = 0; k < elements; k+= blockSize) {
                multiply(data, src.data, res.data, i, k, k, j);
            }
        }
    }

    return res;
}

Matrix& Matrix::operator=(const Matrix& src) {
    data = src.data;
    return *this;
}

Val Matrix::sumDiag() const {
    size_t diagLength = data.size() < data[0].size() ? data.size() : data[0].size();
    Val res = 0;
    for (size_t i = 0; i < diagLength; i++) {
        res += data[i][i];
    }
    return res;
}

void Matrix::multiply(const TwoDVec& matrix1, const TwoDVec& matrix2,
        TwoDVec& matrix3, size_t startRow1, size_t startCol1,
        size_t startRow2, size_t startCol2) const {

    size_t max_i = blockSize + startRow1 >= matrix1.size() ? matrix1.size() : blockSize + startRow1;
    size_t max_j = blockSize + startCol2 >= matrix2[0].size() ? matrix2[0].size() : blockSize + startCol2;
    size_t max_k = blockSize + startCol1 >= matrix1[0].size() ? matrix1[0].size() : blockSize + startCol1;

    for (size_t i = startRow1; i < max_i; i++) {
        for (size_t j = startCol2; j < max_j; j++) {
            Val sum = 0;
            for (size_t k1 = startCol1, k2 = startRow2; k1 < max_k; k1++, k2++) {
                sum += matrix1[i][k1] * matrix2[k2][j];
            }
            matrix3[i][j] += sum;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << matrix.data.size() << " " << matrix.data[0].size() << std::endl;
    for (std::vector<Val> row : matrix.data) {
        std::copy(row.begin(), row.end(), std::ostream_iterator<int>(os, " "));
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& matrix) {
    size_t rows, cols;
    is >> rows >> cols;
    matrix = Matrix(rows, cols);
    for (size_t i = 0; i < rows; i++) {
        std::copy_n(std::istream_iterator<Val>(is), cols,
                matrix.data[i].begin());
    }
    return is;
}
