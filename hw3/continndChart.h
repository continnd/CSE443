// Copyright 2015 Nick Contini

#ifndef _HOME_CONTINND_CSE443_HW3_CONTINNDCHART_H_
#define _HOME_CONTINND_CSE443_HW3_CONTINNDCHART_H_

#include "Point.h"
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
// A class used to store and manipulate points.
class Chart {
public:
    Chart() {}
    ~Chart() {}
    Chart(const Chart& chart);
    Chart(const Chart&& chart);
    explicit Chart(const std::string& filename);

    // Prints each point in chart to given stream in format:
    // a b
    // c d
    // e f
    // Must be less than 4 lines of code
    friend std::ostream& operator<<(std::ostream& os, const Chart& c);

    // Loads all points from given input stream into the given chart.
    friend std::istream& operator>>(std::istream& is, Chart& c);

    // Copies points from the source into this. Enables assignment of one
    // chart into another
    // Should be 2 lines.
    Chart& operator=(const Chart& src);

    // Checks to see if the given point exists within this.
    // One line of code.
    bool contains(const Point& p) const;

    // Adds points of two charts together (no duplicates). Don't modify this or
    // other use copy_if() for a 3 line definition
    Chart operator+(const Chart& other) const;

    // Removes all points that exist within this also contained in other and
    // returns a new chart with those points.
    Chart operator-(const Chart& other) const;

    // Creates a new Chart with points in this scaled by scale. Use
    // std::transform to get 3 line definition.
    Chart operator*(const Point& scale) const;

    // Creates a new Chart with points in this divided by scale. Use
    // std::transform to get 3 line definition.
    Chart operator/(const Point& scale) const;

    // Shift the points in the chart to the right by the given amount. four
    // lines using std::vector.
    Chart operator>>(int value) const;

    // Shift the points in the chart to the left by the given amount.
    Chart operator<<(int value) const;

    // Displays a histogram of distribution of unique points in the 4
    // quadrants. scale represents the number of '*' to be shown for the
    // highest occuring quadrant. No loops for full credit.
    void analyze(std::ostream& os, const int scale) const;

private:
    std::vector<Point> pointList;
};

#endif
