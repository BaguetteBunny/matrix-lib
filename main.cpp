#include <iostream>
#include <chrono>
#include <string>
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

    Matrix<double> LOWER_TRIANGULAR({{1,0,0}, {3,4,0}, {3,4,3}});
    Matrix<double> UPPER_TRIANGULAR({{1,2,1}, {0,4,3}, {0,0,3}});
    Matrix<double> DIAGONAL({{1,0,0}, {0,4,0}, {0,0,3}});
    Matrix<double> SCALAR({{2,0,0}, {0,2,0}, {0,0,2}});
    Matrix<double> I({{1,0,0}, {0,1,0}, {0,0,1}});

    checklist.push_back((A + B) == Matrix<double>({{6, 8}, {10, 12}}));
    checklist.push_back((A - B) == Matrix<double>({{-4, -4}, {-4, -4}}));
    checklist.push_back((A + 2) == Matrix<double>({{3, 4}, {5, 6}}));
    checklist.push_back((A - 2) == Matrix<double>({{-1, 0}, {1, 2}}));
    checklist.push_back((A * 2) == Matrix<double>({{2, 4}, {6, 8}}));
    checklist.push_back((A / 2) == Matrix<double>({{0.5, 1}, {1.5, 2}}));


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