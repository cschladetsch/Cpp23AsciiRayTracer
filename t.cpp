#include <iostream>

int main() {
    std::cout << "\033[31mRed Text\033[0m" << std::endl;
    std::cout << "\033[32mGreen Text\033[0m" << std::endl;
    std::cout << "\033[34mBlue Text\033[0m" << std::endl;
    std::cout << "\033[0mDefault Color Text" << std::endl;
    return 0;
}

