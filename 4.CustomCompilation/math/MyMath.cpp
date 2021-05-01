#include "MyMath.h"
int add(int a, int b) {
    return a + b;
}

double pow(double a, double b) {
    double sum = 1;
    b = int(b);
    while(b) {
        sum *= a;
        std::cout << sum << std::endl;
        --b;
    }   
    return sum;
}