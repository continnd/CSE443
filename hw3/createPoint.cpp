#include "iostream"
#include "fstream"

int main() {
    std::ofstream os("big_points.txt");
    for (int i = 0; i < 500000; i++) {
        os << rand() % 100000 << " " << rand() % 100000 << std::endl;
    }
}
