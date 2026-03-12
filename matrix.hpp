#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <cmath>

template<typename T>

class Matrix {
private:
    int rows, cols;
    std::vector<T> data;

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

    static Matrix<T> identity(int n) {
        Matrix<T> I(n, n);
        for (int i = 0; i < n; ++i) I(i, i) = 1;
        return I;
    }

    void swapRows(int r1, int r2, int start, int end) {
        for (int j = start; j < end; ++j)
            std::swap((*this)(r1,j), (*this)(r2,j));
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

    // Comparison
    bool operator==(const Matrix<T>& other) const {
        if (!isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for equal");
        for (int i = 0; i < rows*cols; ++i) 
            if (other.data[i] != data[i])
                return false;

        return true;
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

    // Determinant
    T det() const {
        try {
            auto [L,U,P,swaps] = LUP();

            T total = 1;
            for (int i = 0; i < rows; ++i)
                total *= U(i,i);

            if (swaps % 2) total = -total;

            return total;
        } catch (const std::invalid_argument&) { return T(0); }
    }

    std::tuple<Matrix<T>, Matrix<T>> LU() const {
        if (!isSquare()) throw std::invalid_argument("Matrix must be a square matrix.");
        Matrix<T> U(rows, cols);
        Matrix<T> L = Matrix<T>::identity(rows);

        for (int i = 0; i < rows; ++i) {
            for (int j = i; j < rows; ++j) {
                T sum = 0;
                for (int k = 0; k < i; ++k) { sum += L(i, k) * U(k, j); }
                U(i, j) = data[rows * i + j] - sum;
            }
            
            for (int j = i+1; j < rows; ++j) {
                T sum = 0;
                for (int k = 0; k < i; ++k) { sum += L(j, k) * U(k, i); }
                L(j, i) = (data[rows * j + i] - sum) / U(i, i);
            }
        }

        return {L, U};
    }

    std::tuple<Matrix<T>, Matrix<T>, Matrix<T>, int> LUP() const {
        if (!isSquare()) throw std::invalid_argument("Matrix must be square.");
        Matrix<T> L(rows, cols);
        Matrix<T> U = *this;
        Matrix<T> P = Matrix<T>::identity(rows);

        int swaps = 0;
        for (int i = 0; i < rows; ++i) {
            int pivot = i;
            T maxVal = std::abs(U(i,i));

            for (int r = i+1; r < rows; ++r) {
                if (std::abs(U(r,i)) > maxVal) {
                    maxVal = std::abs(U(r,i));
                    pivot = r;
                }
            }

            if (maxVal == 0) throw std::invalid_argument("Matrix must be invertible.");

            if (pivot != i) {
                U.swapRows(i, pivot, 0, U.getCols());
                P.swapRows(i, pivot, 0, P.getCols());
                L.swapRows(i, pivot, 0, i);
                swaps++;
            }

            for (int j = i+1; j < rows; ++j) {
                L(j,i) = U(j,i) / U(i,i);
                for (int k = i; k < rows; ++k) 
                    U(j,k) -= L(j,i) * U(i,k);
            }
        }

        for (int i = 0; i < rows; ++i) L(i,i) = 1;

        return {L,U,P,swaps};
    }

    // Matrix Addition
    Matrix<T> operator + (const Matrix<T>& other) const {
        if (!isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for addition");

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

    // Negative
    Matrix<T> operator - () const {
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows * cols; ++i) result.data[i] = -data[i];
        return result;
    }

    // Matrix Substraction
    Matrix<T> operator - (const Matrix<T>& other) const {
        if (!isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for substraction");

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

    // Matrix Multiplication (NAIVE)
    Matrix<T> operator * (const Matrix<T>& other) const {
        if (!canMultiply(other)) throw std::invalid_argument("Invalid matrix multiplication dimensions.");

        Matrix<T> result(rows, other.cols);
        for (int i = 0; i < rows; ++i) {
            for (int k = 0; k < cols; ++k) {
                for (int j = 0; j < other.cols; ++j) {
                    result.data[i * other.cols + j] += data[i * cols + k] * other.data[k * other.cols + j];
                }
            }
        }
        return result;
    }

    // Element-wise Multiplication
    Matrix<T> elementMultiply (const Matrix<T>& other) const {
        if (!isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for element-wise multiplication");

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
        if (!isIdentical(other)) throw std::invalid_argument("Matrix dimensions must match for element-wise division");

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

    bool isSingular() const {return cols == rows && rows == 1;}
    bool isRow() const {return cols > 0 && rows == 1;}
    bool isCol() const {return rows > 0 && cols == 1;}
    bool isNull() const {return std::all_of(data.begin(), data.end(), [](int element) {return !element;});}
    bool isSquare() const {return rows == cols && cols > 0;}

    bool isIdentical(const Matrix<T>& other) const {return rows == other.rows && cols == other.cols;}
    bool canMultiply(const Matrix<T>& other) const {return cols == other.rows;}

    bool isLowerTriangular() const {
        if (!isSquare()) return false;
        
        for (int i = 0; i < rows; ++i) 
            for (int j = i+1; j < cols; ++j) 
                if (data[i * cols + j] != 0) return false;
        
        return true;
    }
    bool isUpperTriangular() const {
        if (!isSquare()) return false;
        
        for (int i = 0; i < rows; ++i) 
            for (int j = 0; j < i; ++j) 
                if (data[i * cols + j] != 0) return false;
        
        return true;
    }
    bool isDiagonal() const {
        if (!isSquare()) return false;
        
        for (int i = 0; i < rows; ++i) {
            for (int j = i + 1; j < cols; ++j) {
                if (data[i * cols + j] != 0) return false;
                if (data[j * cols + i] != 0) return false;
            }
        }

        return true;
    }
    bool isScalar() const {
        if (!isSquare()) return false;
        auto lambda = data[0];
        
        for (int i = 0; i < rows; ++i) {
            if (data[i * cols + i] != lambda) return false;
            for (int j = i + 1; j < cols; ++j) {
                if (data[i * cols + j] != 0) return false;
                if (data[j * cols + i] != 0) return false;
            }
        }
        return true;
    }
    bool isIdentity() const {
        if (!isSquare()) return false;
        
        for (int i = 0; i < rows; ++i) {
            if (data[i * cols + i] != 1) return false;
            for (int j = i + 1; j < cols; ++j) {
                if (data[i * cols + j] != 0) return false;
                if (data[j * cols + i] != 0) return false;
            }
        }
        return true;
    }
};