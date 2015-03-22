#include <iostream>
#include <vector>
#include <iterator>
#include <omp.h>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    std::copy(vec.begin(), vec.end(),
              std::ostream_iterator<T>(std::cout, " "));
    return os;
}

int main() {
    std::vector<int> data(4);
    int a = 0;
#pragma omp parallel
    { // Fork
        const int numThreads = omp_get_num_threads();
        const int threadID   = omp_get_thread_num();
        std::cout << "hello OpenMP from "
                  << threadID   << " of "
                  << numThreads << " threads.\n";
        for(int i = 0; (i < 100000); i++) {
            a++;
            data[threadID]++;
        }
    } // Join

    // Print outputs from main thread
    std::cout << "The value of a = "  << a    << std::endl;
    std::cout << "Value in vector = " << data << std::endl;
}
