#include <iostream>
#include <chrono>
#include <string>
#include <typeinfo>
#include "matrix.hpp"

void benchmark(void (*func_ptr)()) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();

    func_ptr();

    auto t2 = high_resolution_clock::now();

    duration<double> elapsed_seconds = t2 - t1;
    std::cout << "Function took: " << elapsed_seconds.count() << " seconds\n";
}

void kroneckerTime() {
    Matrix<double> A({{1,2}, {3,4}});
    Matrix<double> B({{5,6}, {7,8}});
    for (int i = 0; i < 1000000; i++) {
        Matrix<double> C = A.kronecker(B);
    }
}

void unitTest() {
    std::vector<bool> checklist;
    Matrix<double> A({{1,2}, {3,4}});
    Matrix<double> B({{5,6}, {7,8}});

    Matrix<double> A0({{-2,-4,-1}, {0,1,3}, {-2,3,1}});
    Matrix<int> A1({{0, 3, 1},{0, 2, 1},{0, 2, 1}});

    Matrix<double> SINGULAR(std::vector<std::vector<double>>{{1}});
    Matrix<double> ROW_MTX({{1,2,3}});
    Matrix<double> COL_MTX(std::vector<std::vector<double>>{{1}, {2}, {3}});
    Matrix<double> NULL_MTX({{0,0,0}, {0,0,0}, {0,0,0}});
    Matrix<double> LOWER_TRIANGULAR({{1,0,0}, {3,4,0}, {3,4,3}});
    Matrix<double> UPPER_TRIANGULAR({{1,2,1}, {0,4,3}, {0,0,3}});
    Matrix<double> DIAGONAL({{1,0,0}, {0,4,0}, {0,0,3}});
    Matrix<double> SCALAR({{2,0,0}, {0,2,0}, {0,0,2}});
    Matrix<double> I({{1,0,0}, {0,1,0}, {0,0,1}});

    // Matrix to Matrix Operations
    checklist.push_back((A + B) == Matrix<double>({{6, 8}, {10, 12}}));
    checklist.push_back((A - B) == Matrix<double>({{-4, -4}, {-4, -4}}));
    checklist.push_back((A * B) == Matrix<double>({{19, 22}, {43, 50}}));

    checklist.push_back((A.kronecker(B)) == Matrix<double>({{5, 6, 10, 12}, {7, 8, 14, 16}, {15, 18, 20, 24}, {21, 24, 28, 32}}));

    // Determinant & Inversion
    checklist.push_back(A0.det() == 38);
    checklist.push_back(A1.det() == 0);

    // Transpose
    checklist.push_back(A.transpose() == Matrix<double>({{1,3}, {2,4}}));
    
    // Scalar Operations
    checklist.push_back((A + 2) == Matrix<double>({{3, 4}, {5, 6}}));
    checklist.push_back((A - 2) == Matrix<double>({{-1, 0}, {1, 2}}));
    checklist.push_back((A * 2) == Matrix<double>({{2, 4}, {6, 8}}));
    checklist.push_back((A / 2) == Matrix<double>({{0.5, 1}, {1.5, 2}}));

    // Element-wise Operations
    checklist.push_back((A.elementMultiply(B)) == Matrix<double>({{5, 12}, {21, 32}}));
    checklist.push_back((B.elementDivide(B)) == Matrix<double>({{1, 1}, {1, 1}}));

    // Checks
    checklist.push_back(SINGULAR.isSingular());
    checklist.push_back(A.isSquare());
    checklist.push_back(ROW_MTX.isRow());
    checklist.push_back(COL_MTX.isCol());
    checklist.push_back(NULL_MTX.isNull());
    checklist.push_back(LOWER_TRIANGULAR.isLowerTriangular());
    checklist.push_back(UPPER_TRIANGULAR.isUpperTriangular());
    checklist.push_back(DIAGONAL.isDiagonal());
    checklist.push_back(SCALAR.isScalar());
    checklist.push_back(I.isIdentity());

    for (auto i = checklist.begin(); i != checklist.end(); ++i) { 
        int idx = std::distance(checklist.begin(), i); 
        if (!checklist[idx]) throw std::invalid_argument("Error at index " + std::to_string(idx));
    }

    std::cout << "All checks are successful.";

}

int main() {
    unitTest();
    return 0;
}