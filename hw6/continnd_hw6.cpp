// Copyright 2015 Nick Contini
//
#include <math.h>
#include <omp.h>
#include <string>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>

#define PRIME 128
#define FACTORABLE 64
#define FACTORIAL 32
#define PALLINDROME 16

char isPrime(int num) {
    if (num < 2) { return 0; }
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) { return 0; }
    }
    return PRIME;
}

char hasPrimeFactors(int num) {
    if (num < 4) { return 0; }
    for (int i = 2; i < sqrt(num); i++) {
        if (num % i == 0 && isPrime(i) && isPrime(num / i)) {
            return FACTORABLE;
        }
    }
    return 0;
}

char isPallindrome(int num) {
    std::vector<int> digits;
    while (num > 0) {
        digits.push_back(num % 10);
        num /= 10;
    }

    std::vector<int>::iterator it = digits.begin(); 
    std::vector<int>::reverse_iterator rit = digits.rbegin(); 
    std::vector<int>::iterator digitsEnd = digits.begin() + digits.size() / 2;

    for (; it != digitsEnd; it++, rit++) {
        if (*it != *rit) { return 0; }
    }
    return PALLINDROME;
}

char isFactorial(int num) {
    int i = 2;
    while (num != 1) {
        if (num % i != 0) { return 0; }
        num /= i++;
    }
    return FACTORIAL;
}

void printResults(std::vector<char>& numInfo, int begin) {
    for (unsigned int i = 0; i < numInfo.size(); i++) {
        std::printf("%d: %sprime, %sfactorable, %sfactorial, %spallindrome\n",
                begin + i, (numInfo[i] & PRIME ? "is " : "not "),
                (numInfo[i] & FACTORABLE ? "is " : "not "),
                (numInfo[i] & FACTORIAL ? "is " : "not "),
                (numInfo[i] & PALLINDROME ? "is " : "not "));
    }
}

int main(int argc, char const* argv[]) {
    if (argc != 3 || std::atoi(argv[1]) > std::atoi(argv[2])) {
        std::cout << "Not a valid range!" << std::endl;
        std::exit(1);
    }

    int begin = std::atoi(argv[1]), end = std::atoi(argv[2]);

    std::vector<char> numInfo(end - begin + 1);

#pragma omp parallel
    {
        unsigned int chunkSize = numInfo.size() / omp_get_num_threads() + 1;
        unsigned int thrStart = omp_get_thread_num() * chunkSize;
        unsigned int thrEnd = std::min((thrStart + chunkSize), (unsigned int)
                numInfo.size());
        for (unsigned int i = thrStart; i < thrEnd; i++) {
            numInfo[i] += isPrime(i + begin) + hasPrimeFactors(i + begin);
            numInfo[i] += isFactorial(i + begin) + isPallindrome(i + begin);
        }
    }

    printResults(numInfo, begin);
    return 0;
}
