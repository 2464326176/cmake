#include <iostream>
#include "./build/config.h"


#ifdef USE_MYMATH
    #include "math/MyMath.h"
#else 
    #include <math.h>
#endif

int main(int argc, char* argv[]) {
    #ifdef USE_MYMATH
    std::cout << argv[1] << "^" <<  argv[2] << " = " << mypow(atof(argv[1]), atoi(argv[2])) << std::endl;
    #endif
    return 0;
}