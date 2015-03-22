#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <omp.h>


/** A simple class to encapsulate two numbers and their
    Greatest Common Divisor (GCD)
**/
class ThreeInt {
    friend std::ostream&  operator<<(std::ostream& os, const ThreeInt& ti);
private:
    int num1, num2;
    int gcd;
public:
    ThreeInt() : num1(rand()), num2(num1 + 12345), gcd(0) {}
    void findGCD();
};

/** Convert the following computeGCD method to operate as a data
    parallel application using Open MP
*/
void computeGCD(std::vector<ThreeInt>& numList) {
    // std::for_each(numList.begin(), numList.end(),
    //              std::mem_fun_ref(&ThreeInt::findGCD));
    // Convet this method to operate as a data parallel application.
    // In exam you will not be supplied any more information. Here are
    // a few tips:
    //
    // The above for-each loop should be restructured as:
    //

#pragma omp parallel
    {
        int threadID = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
        int chunkSize = numList.size() / numThreads + 1;
        int startIdx = threadID * chunkSize;
        int endIdx = std::min<int>(startIdx + chunkSize, numList.size());

        for(int i = startIdx; (i < endIdx); i++) {
            numList[i].findGCD();
        }
    }
    //
    // The only basic math that is left to do is to determine values
    // for startIdx and endIdx based on thread ID
    // (omp_get_thread_num()) and number of threads
    // (omp_get_num_threads()).  Each entry is numList[i] must be
    // processed by only one thread, but every entry in numList must
    // be processed by calling findGCD() method on it.
}

// ----------------------------------------------------------------------
//     DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
// ----------------------------------------------------------------------

void
ThreeInt::findGCD() {
    int a = num1, b = num2;
    while (a != b) {
        int temp = std::min(a, b);
        a        = std::abs(a - b);
        b        = temp;
    }
    gcd = a;
}

std::ostream&
operator<<(std::ostream& os, const ThreeInt& ti) {
    os << "gcd("  << ti.num1 << ", " 
        << ti.num2 << ") = "  << ti.gcd;
    return os;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Enter number of pairs to generate "
            << "as command-line arguments.\n";
        return 1;
    }
    // Create a vector of numbers
    std::vector<ThreeInt> numList(atoi(argv[1]));
    // Compute GCD for all the numbers
    computeGCD(numList);
    std::copy(numList.begin(), numList.end(),
            std::ostream_iterator<ThreeInt>(std::cout, "\n"));
    return 0;

}
