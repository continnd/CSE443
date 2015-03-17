/** A program that uses custom operators on vector of integers for
    performing basic mathematical transformations.

    Copyright (C) 2015 raodm@miamiOH.edu
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

// Shortcut to refer to a vector of integers throughout the code
using IntVec = std::vector<int>;

/** A simple addition operator to perform element-wise addition.

    This addition operator modifies the first vector by adding the
    value of the corresponding elements in the second vector.  For
    example, if v1 = {1, 2, 3} and v2 = {5, -1, 1} then this method
    modifies vector v1 to {6, 1, 4}.

    \param[in] v1 The first vector to be used and modified for
    addition.

    \param[in] v2 The second vector to be used for addition.

    \return The resulting vector from element-wise addition. Note that
    the two input vectors v1 and v2 must be of the same size. The
    returned vector is a reference to v1.
*/
IntVec& operator+=(IntVec& v1, const IntVec& v2) {
    std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(),
                   std::plus<int>());
    return v1;
}

/** A simple multiplication operator to perform element-wise
    multiplication

    This multiplication operator modifies the first vector such that
    each element is the product of corresponding elements in v2.  For
    example, if v1 = {1, 2, 3} and v2 = {5, -1, 1} then this method
    modifies vector v1 to {5, -2, 3}.

    \param[in] v1 The first vector to be used and modified for
    multiplication.

    \param[in] v2 The second vector to be used for multiplication.

    \return The resulting vector from element-wise
    multiplication. Note that the two input vectors v1 and v2 must be
    of the same size. The returned vector is a reference to v1.
*/
IntVec operator*=(IntVec& v1, const IntVec& v2) {
    std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(),
                   std::multiplies<int>());
    return v1;
}

// --------------------------------------------------------------
// DO NOT MODIFY CODE BELOW THIS LINE
// --------------------------------------------------------------

int main(int argc, char *argv[]) {
    const int VecSize  = (argc > 1) ? std::stoi(argv[1]) : 100000;
    const int RepCount = (argc > 2) ? std::stoi(argv[2]) : 50000;
    IntVec a(VecSize, 1), b(VecSize, 2), c(VecSize, 1);
    for (int i = 0; (i < RepCount); i++) {
        b += c;
        a += c;
    }
    std::cout << "a[0]=" << a[0] << ", b[0]=" << b[0] << std::endl;
    return 0;
}

// End of source code
