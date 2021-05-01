#include <iostream>
#include "./build/config.h"



int main(int argc, char* argv[]) {

    std::cout << argv[0]
        << "major version:" << Demo_VERSION_MAJOR
        << "minor version:" << Demo_VERSION_MINOR
        << std::endl;
    return 0;
}