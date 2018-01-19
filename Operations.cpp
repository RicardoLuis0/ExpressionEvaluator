#include <cstdlib>
#include <cmath>
#include "Eval.h"

double Eval::add(double a,double b){
    return a+b;
}

double Eval::subtract(double a,double b){
    return a-b;
}

double Eval::multiply(double a,double b){
    return a*b;
}

double Eval::divide(double a,double b){
    return a/b;
}

double Eval::power(double a,double b){
    return std::pow(a,b);
}
double Eval::root(double a,double b){
    return std::pow(b,1.0/a);
}
