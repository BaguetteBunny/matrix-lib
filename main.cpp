#include <iostream>
#include <chrono>
#include "matrix.h"

void time(void (*func_ptr)()) {
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

void functionToTime() {
    Matrix<double> A({{1,2}, {3,4}});
    Matrix<double> B({{5,6}, {7,8}});
    for (int i = 0; i < 1000000; i++) {
        Matrix<double> C = A.kronecker(B);
    }
}

int main() {

    time(functionToTime);

    return 0;
}