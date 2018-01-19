#include <iostream>
#include <cstdio>
#include "Eval.h"

using namespace std;

int main(){
    std::string expr="2+2";
    std::cout<<"expression to evaluate: ";
    std::cin>>expr;
    cout <<expr<<"="<<fixed<<Eval::do_eval(expr);
    return 0;
}
