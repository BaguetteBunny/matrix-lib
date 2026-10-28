#include <iostream>
#include <chrono>
#include <random>
#include <omp.h>
#include "../matrix.hpp"

Matrix<double> createLargeMatrix(size_t N, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    std::vector<std::vector<double>> data(N, std::vector<double>(N));

    for (auto& row : data)
        for (auto& v : row)
            v = dist(rng);
    return Matrix<double>(data);
}

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

void matMultTime() {
    size_t N = 100;
    std::mt19937 seed(42);

    Matrix<double> A = createLargeMatrix(N, seed);
    Matrix<double> B = createLargeMatrix(N, seed);

    volatile double sink = (A * B)(0, 0);

    #pragma omp parallel for
    for (int i = 0; i < 20'000; i++) {
        Matrix<double> C = A*B;
        sink = C(0, 0);
    }
}

int main() {
    benchmark(&matMultTime);
    return 0;
}