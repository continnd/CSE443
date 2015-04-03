// A simple program to generate a large set of random numbers and
// operate on them using different scheduling options.

#include <vector>
#include <omp.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>

// A convenience typedef to hold a number and number of prime factor
// counts
typedef std::pair<long, int> NumFactCount;

// Forward declaration (aka prototype) to keep compiler happy
bool isPrime(const long);
int getPrimeFactorCount(const long);

/**
  This method returns a number from numList that has the highest
  number of prime factor counts.
  */
NumFactCount
findNumberWithMaxPrimeFactorCount(const std::vector<long>& numList) {
    std::vector<NumFactCount> maxFactCount;
    // In the pair maxFactCount, maxFactCount.first contains the
    // number and maxFactCount.second contains its primeFactorCount.
#pragma omp parallel default(none) shared(numList, maxFactCount)
    {
#pragma omp critical
        maxFactCount.resize(omp_get_num_threads());

        const int thrId = omp_get_thread_num();

#pragma omp for schedule(runtime)
        for(size_t i = 0; (i < numList.size()); i++) {
            const int primeFactCount = getPrimeFactorCount(numList[i]);
            if (maxFactCount[thrId].second < primeFactCount) {
                maxFactCount[thrId] = std::make_pair(numList[i], primeFactCount);
            }
        }
    }
    return *std::max_element(maxFactCount.begin(), maxFactCount.end(),
            [&](NumFactCount p1, NumFactCount p2) { return p1.second < p2.second; });
}


// ----------------------------------------------------------------------
//     DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
// ----------------------------------------------------------------------

int getPrimeFactorCount(const long number) {
    int count = 0;
    for(long factor = 1; (factor < number); factor++) {
        if ((number % factor == 0) && isPrime(factor)) {
            count++;
        }
    }
    return count;
}

bool isPrime(const long number) {
    int MaxFactor = int(sqrt(number)) + 1;
    for(int factor = 2; (factor < MaxFactor); factor++) {
        if (number % factor == 0) {
            return false;
        }
    }
    return true;
}

long generator() {
    const long digits = (rand() % 4 > 1 ? 1000L : 10000000L);
    return rand() % digits;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Specify count of numbers to generate "
            << "as command-line argument.\n";
        return 1;
    }
    // Generate vector with random numbers
    std::vector<long> numList(std::stoi(argv[1]));
    std::generate(numList.begin(), numList.end(), generator);
    std::sort(numList.begin(), numList.end());
    // Compute the largest prime number in the list.
    const NumFactCount bigPrime = findNumberWithMaxPrimeFactorCount(numList);
    // Print it.
    std::cout << "Number with most number of prime factors: "
        << bigPrime.first  << ", prime factor count = "
        << bigPrime.second << std::endl;
    return 0;
}





