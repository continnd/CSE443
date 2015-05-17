#include <iostream>
#include <string>

int main(int argc, char const* argv[])
{
    char str[15];
    std::cin >> str;
    std::string actualStr(str);
    str = actualStr.c_str();
    std::cout << actualStr.size() << std::endl;
    std::cout << sizeof(str) / sizeof(*str) << std::endl;
    return 0;
}
