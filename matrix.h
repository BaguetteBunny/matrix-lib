#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

template<typename T>

class Matrix {
private:
    int rows, cols;
    std::vector<T> data;

    const bool isIdentical(Matrix<T>& other) const {
        return rows == other.rows && cols == other.cols;
    }
    const bool canMultiply(Matrix<T>& other) const {
        return cols == other.rows;
    }

public:
    Matrix(int r, int c, T init = T()) : rows(r), cols(c), data(r * c, init) {}

    Matrix(const std::vector<std::vector<T>>& values) {
        rows = values.size();
        cols = rows > 0 ? values[0].size() : 0;
        data.reserve(rows * cols);

        for(const auto& row : values) {
            if(row.size() != cols)
                throw std::invalid_argument("All rows must have the same number of columns");
            data.insert(data.end(), row.begin(), row.end());
        }
    }

    // Add to idx
    T& operator()(int r, int c) {
        if(r < 0 || r >= rows || c < 0 || c >= cols) throw std::out_of_range("Matrix index out of range");
        return data[r * cols + c];
    }

    const T& operator()(int r, int c) const {
        return data[r * cols + c];
    }

    // Get dimensions
    int getRows() const { return rows; }
    int getCols() const { return cols; }

    // Print matrix
    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) std::cout << operator()(i,j) << " ";

            std::cout << "\n";
        }
    }

    // Kronecker Product
    Matrix<T> kronecker(const Matrix<T>& other) const {
        int totalRows = rows*other.getRows();
        int totalCols = cols*other.getCols();

        Matrix<T> result(totalRows, totalCols);
        for (int i = 0; i < totalRows; ++i) {
            for (int j = 0; j < totalCols; ++j) {
                int selfRow = i / other.getRows();
                int selfCol = j / other.getCols();
                int otherRow = i % other.getRows();
                int otherCol = j % other.getCols();
                
                result.data[i * totalCols + j] = data[selfRow * cols + selfCol] * other.data[otherRow * other.getCols() + otherCol];
            }
        }
        
        return result;
    }

    // Matrix Addition
    Matrix<T> operator + (const Matrix<T>& other) const {
        if (isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for addition");

        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows * cols; ++i) result.data[i] = data[i] + other.data[i];
        return result;
    }

    // Scalar Addition
    Matrix<T> operator + (const T& scalar) const {
      Matrix<T> result(rows, cols);
      for (int i = 0; i < rows * cols; ++i) result.data[i] = data[i] + scalar;
      return result;
    }

    // Matrix Substraction
    Matrix<T> operator - (const Matrix<T>& other) const {
        if (isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for substraction");

        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows * cols; ++i) result.data[i] = data[i] - other.data[i];
        return result;
    }

    // Scalar Substraction
    Matrix<T> operator - (const T& scalar) const {
      Matrix<T> result(rows, cols);
      for (int i = 0; i < rows * cols; ++i) result.data[i] = data[i] - scalar;
      return result;
    }

    // Element-wise Multiplication
    Matrix<T> elementMultiply (const Matrix<T>& other) const {
        if (isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for element-wise multiplication");

        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows * cols; ++i) result.data[i] = data[i] * other.data[i];
        return result;
    }

    // Scalar Multiplication
    Matrix<T> operator * (const T& scalar) const {
      Matrix<T> result(rows, cols);
      for(int i = 0; i < rows * cols; ++i)
          result.data[i] = data[i] * scalar;
      return result;
    }

    // Element-wise Division
    Matrix<T> elementDivide (const Matrix<T>& other) const {
        if (isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for element-wise division");

        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows * cols; ++i) result.data[i] = data[i] / other.data[i];
        return result;
    }

    // Scalar Division
    Matrix<T> operator / (const T& scalar) const {
      Matrix<T> result(rows, cols);
      for(int i = 0; i < rows * cols; ++i)
          result.data[i] = data[i] / scalar;
      return result;
    }

    bool isRow() const {return rows > 0 && !cols;}
    bool isCol() const {return cols > 0 && !rows;}
    bool isNull() const {return std::all_of(data.begin(), data.end(), [](int element) {return !element;});}
    bool isSquare() const {return rows == cols && cols > 0;}

    bool isLowerTriangular() const {
        for (int i = 1; i < (rows - 1) * (cols - 1); ++i) {
            if (i % cols == 0) continue;
            if (data[i] != 0 && (i)) return false;
        }
        return true;
    }
};


#endif