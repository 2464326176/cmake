#include <iostream>

#ifdef use_mymath
    #include "math/MyMath.h"
#else 
    #include <math.h>
#endif

int main() {

    std::cout << "5 + 9 = " << pow(5, 9) << std::endl;

    return 0;
}