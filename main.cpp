#include <iostream>
#include <cstdlib>

#include "Eval.h"

int main(){
    std::string expr;
    //std::cout<<"expression to evaluate: ";
    std::cin>>expr;
    system("cls");
    std::cout<<std::fixed<<expr<<"="<<Eval::do_eval(expr)<<"\n";
    system("pause");
    return 0;
}
