#include "MyMath.h"
#include <iostream>
int add(int a, int b) {
    return a + b;
}

int mypow(double a, int b) {
    double sum = 1;
    while(b) {
        sum *= a;
        --b;
    }   
    return (int)sum;
}