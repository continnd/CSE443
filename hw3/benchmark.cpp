#include <vector>
#include <algorithm>

std::vector<int> nums;

bool compNums(int num1, int num2) {
    return num1 < num2;
}

int main() {
    std::vector<int>nums(2);
    for (int i = 0; i < 10000000000 / 4; i++) {
        // std::sort(nums.begin(), nums.end(), [](int num1,
        //            int num2) { return num1 > num2; });
        std::sort(nums.begin(), nums.end(), compNums);
    }
    return 0;
}
