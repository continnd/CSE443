// Copyright Nick Contini 2015
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include "Point.h"
#include "continndChart.h"

Chart::Chart(const Chart& chart) {
    std::vector<Point> pointList(chart.pointList);
}

Chart::Chart(const Chart&& chart) {
    std::vector<Point> pointList(std::move(chart.pointList));
}

Chart::Chart(const std::string& filename) {
    std::ifstream inputfile(filename);
    std::copy(std::istream_iterator<Point>(inputfile),
            std::istream_iterator<Point>(), std::back_inserter(pointList));
}

std::ostream& operator<<(std::ostream& os, const Chart& c) {
    copy(c.pointList.begin(), c.pointList.end(),
            std::ostream_iterator<Point>(os, "\n"));
    return os;
}

std::istream& operator>>(std::istream& is, Chart& c) {
    std::copy(std::istream_iterator<Point>(is), std::istream_iterator<Point>(),
            std::back_inserter(c.pointList));
    return is;
}

Chart& Chart::operator=(const Chart& src) {
    this->pointList = src.pointList;
}

bool Chart::contains(const Point& p) const {
    return std::find(this->pointList.begin(), pointList.end(), p) !=
        pointList.end();
}

Chart Chart::operator+(const Chart& other) const {
    Chart c;
    std::copy(this->pointList.begin(), this->pointList.end(),
            std::back_inserter(c.pointList));
    std::copy_if(other.pointList.begin(), other.pointList.end(),
            std::back_inserter(c.pointList), [&](Point p) {
            return !c.contains(p); });
    return c;
}

Chart Chart::operator-(const Chart& other) const {
    Chart c;
    std::copy(this->pointList.begin(), this->pointList.end(),
            std::back_inserter(c.pointList));
    std::for_each(other.pointList.begin(), other.pointList.end(),
            [&](Point p) { auto it = std::find(c.pointList.begin(),
            c.pointList.end(), p); if (it != c.pointList.end()) {
            c.pointList.erase(it);} });
    return c;
}

Chart Chart::operator*(const Point& scale) const {
    Chart c;
    transform(this->pointList.begin(), this->pointList.end(),
            std::back_inserter(c.pointList), [&](const Point p) {
            return Point(p.getX() * scale.getX(), p.getY() * scale.getY()); });
    return c;
}

Chart Chart::operator/(const Point& scale) const {
    Chart c;
    transform(this->pointList.begin(), this->pointList.end(),
            std::back_inserter(c.pointList), [&](const Point p) {
            return Point(p.getX() / scale.getX(), p.getY() / scale.getY()); });
    return c;
}

Chart Chart::operator>>(int value) const {
    Chart c;
    for (int i = 0; i < value; i++) {
        c.pointList.push_back(Point());
    }
    std::copy(this->pointList.begin(), this->pointList.end(),
            back_inserter(c.pointList));
    return c;
}

Chart Chart::operator<<(int value) const {
    Chart c;
    std::copy(this->pointList.begin() + value, this->pointList.end(),
            std::back_inserter(c.pointList));
    return c;
}

void Chart::analyze(std::ostream& os, const int scale) const {
    Chart c;
    std::vector<int> histogram {0, 0, 0, 0};
    std::unique_copy(this->pointList.begin(), this->pointList.end(),
            std::back_inserter(c.pointList));
    for_each(c.pointList.begin(), c.pointList.end(), [&](Point p) {
            histogram[p.getQuadrant() - 1]++; });
    int maxe = *std::max_element(histogram.begin(), histogram.end());
    os << "I  : " << std::string(static_cast<int>(histogram[0] * 1.0 /
                maxe * scale), '*') << std::endl;
    os << "II : " << std::string(static_cast<int>(histogram[1] * 1.0 /
                maxe * scale), '*') << std::endl;
    os << "III: " << std::string(static_cast<int>(histogram[2] * 1.0 /
                maxe * scale), '*') << std::endl;
    os << "IV : " << std::string(static_cast<int>(histogram[3] * 1.0 /
                maxe * scale), '*') << std::endl;
}
