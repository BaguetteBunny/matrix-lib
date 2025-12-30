#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>

class Matrix {
private:
    int rows, cols;
    std::vector<T> data;

public:
    Matrix(int r, int c, T init = T()) : rows(r), cols(c), data(r * c, init) {}

    // Add to idx
    T& operator()(int r, int c) {
        if(r < 0 || r >= rows || c < 0 || c >= cols)
            throw std::out_of_range("Matrix index out of range");
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
        for(int i = 0; i < rows; ++i) {
            for(int j = 0; j < cols; ++j)
                std::cout << operator()(i,j) << " ";
            std::cout << "\n";
        }
    }

    // Matrix Addition
    Matrix<T> operator+(const Matrix<T>& other) const {
        if(rows != other.rows || cols != other.cols)
            throw std::invalid_argument("Matrix dimensions must match for addition");

        Matrix<T> result(rows, cols);
        for(int i = 0; i < rows * cols; ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    // Scalar Addition
    Matrix<T> operator+(const T& scalar) const {
      Matrix<T> result(rows, cols);
      for(int i = 0; i < rows * cols; ++i)
          result.data[i] = data[i] + scalar;
      return result;
    }
    
  };


#endif