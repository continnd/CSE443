#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>

/** A set of convenient, commonly used statistical methods.
 */

/**
   This method returns the mean of a given set of numbers.

   \param[in] numbers A vector containing one or more numbers whose
   mean is to be returned. It is assumed that the vector as at least
   one entry in it.

   \return The mean of the numbers in the list.
*/
double mean(const std::vector<int>& numbers) {

}

/**
   Return the median of the given list of numbers.

   The median is described as the numerical value separating the
   higher half of the numbers from the lower half. The median of a
   list of numbers can be found by arranging (or sorting) all the
   observations from lowest value to highest value and picking the
   middle one. If there is an even number of observations, then there
   is no single middle value; the median is then usually defined to be
   the mean of the two middle values.

   In order to sort the given list of numbers, make a copy of the
   given vector using a suitable constructor (as the parameter is
   constant and cannot be modified) and then sort the numbers using
   call to std::sort() method as: std::sort(numbersCopy.begin(),
   numbersCopy.end());

   \param[in] numbers A vector containing one or more numbers whose
   median is to be returned.  Assume that the vector has at least one
   entry in it.

   \return The median of the numbers in the list.   
*/
double median(const std::vector<int>& numbers) {
 
}

/**
   Return the mode of the given list of numbers.

   The mode is the value that most frequently occurrs in the given list.

   \param[in] numbers A vector containing one or more numbers whose
   median is to be returned.  Assume that the vector has at least one
   entry in it.

   \return The mode of the numbers in the list.   
*/
int mode(const std::vector<int>& numbers) {

}

/**
   This method returns true only if every element in list2 (immaterial
   of position in list2) is in list1 (immaterial of position in the
   list1).

   \param[in] list1 A vector containing one or more numbers whose that
   are to be searched to determine if list1 contains all numbers in
   list2.  Assume that the vector has at least one entry in it.

   \param[in] list2 A vector containing one or more numbers that are
   to be searched in list1 to determine if list1 contains all numbers
   in list2.  Assume that the vector has at least one entry in it.
   
   \return This method returns true if every element in list2
   (immaterial of position in list2) is in list1 (immaterial of
   position in the list1).
*/
bool contains(const std::vector<int>& list1, const std::vector<int>& list2) {
}

/**
   This method reads integers from the given input stream until -1 is
   encountered and returns the numbers read as a vector.

   This method must repeatedly read integers from the given input
   stream (unitl -1 is encountered), add the integers to a vector and
   return the vector back to the caller.

   \param[in] is The input stream from where the data is to be read.
   Numbers can be read using the extraction operator (>>).
   
   \return A vector of integers entered by the user. The -1 (which
   indicates end of list) is not stored in the returned vector.
*/
std::vector<int> readVector(std::istream& is) {
  
}

/**
   Prints each number (followed by a space) in the given list to the
   given output stream.

   \param[in] list The list of numbers whose entries are to be
   printed.

   \param[out] os The output stream to which the numbers are to be
   printed.
*/
void printList(const std::vector<int>& list, std::ostream& os) {
   
}

// -----------------------------------------------------------
// DO NOT MODIFY CODE BELOW THIS LINE
// -----------------------------------------------------------

/**
   A main method that calls various methods for performing some simple
   tests.
*/
int main() {
    const std::string NoYes[] = {"no", "yes"};
    std::cout << "Enter first list of numbers (-1 to end list): ";
    std::vector<int> list1 = readVector(std::cin);
    std::cout << "Statistics about the first list of numbers:\n";
    std::cout << "  Numbers: "; printList(list1, std::cout);
    std::cout << "  mean   = " << mean(list1)   << std::endl;
    std::cout << "  median = " << median(list1) << std::endl;
    std::cout << "  mode   = " << mode(list1)   << std::endl;

    std::cout << "Enter second list of numbers (-1 to end list): ";
    std::vector<int> list2 = readVector(std::cin);
    std::cout << "Statistics about the second list of numbers:\n"; 
    std::cout << "  Numbers: "; printList(list2, std::cout);   
    std::cout << "  mean   = " << mean(list2)   << std::endl;
    std::cout << "  median = " << median(list2) << std::endl;
    std::cout << "  mode   = " << mode(list2)   << std::endl;

    std::cout << "Checking to see if one list contains the other:\n"
              << "  First list contains second?: "
              << NoYes[contains(list1, list2)] << std::endl
              << "  Second list contains first?: "
              << NoYes[contains(list2, list1)] << std::endl;    
    return 0;
}
