#include <iostream>

#ifdef use_mymath
    #include "math/MyMath.h"
#else 
    #include <math.h>
#endif

int main() {
    #ifdef use_mymath
    std::cout << "2 ^ 2 = " << mypow(2, 2) << std::endl;
    #else 
    std::cout << "2 ^ 2 = " << pow(2, 2) << std::endl;
    #endif
    

    return 0;
}