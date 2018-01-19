#include <iostream>
#include <stdexcept>
#include "Eval.h"
Eval::node::node(){
    type=0;
    operator_c=0;
}
Eval::node::node(int t,double num){
    type=t;
    number_d=num;
}
Eval::node::node(int t,char op){
    type=t;
    operator_c=op;
}
std::map<char,Eval::operation> Eval::op_map={
    {'+',{0,add}},
    {'-',{0,subtract}},
    {'*',{1,multiply}},
    {'/',{1,divide}},
    {'^',{2,power}},
    {'v',{2,root}}
};
std::vector<char> Eval::open_brackets={
    '(',
    '[',
    '{'
};
std::vector<char> Eval::close_brackets={
    ')',
    ']',
    '}'
};
double Eval::execute(std::stack<Eval::node> &stack,bool consume_brackets,bool do_debug){
    bool resultset=false;
    double result=0;
    node current;
    current=stack.top();
    while(true){
        if(stack.empty()){
            if(do_debug)std::cout<<"stack empty\n";
            stack.push(node(0,result));
            return true;
        }
        char operation;
        current=stack.top();
        if(current.type==0){
            if(!resultset){
                resultset=true;
                result=current.number_d;
            }else{
                if(do_debug)std::cout<<"running "<<current.number_d<<" "<<operation<<" "<<result<<"\n";
                result=op_map[operation].action(current.number_d,result);
            }
        }
        if(current.type==1){
            if(do_debug)std::cout<<"reading operation "<<current.operator_c<<"\n";
            operation=current.operator_c;
        }
        if(current.type==2){
            if(do_debug)std::cout<<"reading open bracket\n";
            if(consume_brackets){
                stack.pop();
            }
            stack.push(node(0,result));
            return true;
        }
        stack.pop();
    }
}
double Eval::do_eval(std::string expr,bool do_debug){
    std::stack<node> stack;
    int lastweight=0;
    double number_acc=0,decimal_count=0;
    bool reading_number=false,number_decimal=false;
    for(char c:expr){
        if(c==' '){//if blank space, skip to next char
            continue;
        }else{
            if(reading_number){//number reader
                if(c=='.'||c==','){
                    if(do_debug)std::cout<<"switch number to decimal\n";
                    number_decimal=true;
                    decimal_count=1;
                    continue;
                }else if(c>=48&&c<=57){
                    if(do_debug)std::cout<<"read another digit\n";
                    if(number_decimal){
                        number_acc+=((c-48)/(10*decimal_count));
                    }else{
                        number_acc*=10;
                        number_acc+=c-48;
                    }
                    continue;
                }else{
                    if(do_debug)std::cout<<"finish reading number, result: "<<number_acc<<"\n";
                    stack.push(node(0,number_acc));
                    reading_number=false;
                }
            }else if(c>=48&&c<=57){
                if(do_debug)std::cout<<"Start reading number\n";
                reading_number=true;
                number_decimal=false;
                number_acc=c-48;
                continue;
            }else if(c=='.'||c==','){
                if(do_debug)std::cout<<"Start reading number as decimal\n";
                reading_number=true;
                number_decimal=true;
                number_acc=0;
                decimal_count=1;
                continue;
            }
            if(std::find(open_brackets.begin(),open_brackets.end(),c)!=open_brackets.end()){//open bracket
                if(do_debug)std::cout<<"Reading open bracket\n";
                stack.push(node(2,(char)0));
            }else if(std::find(close_brackets.begin(),close_brackets.end(),c)!=close_brackets.end()){//close bracket execute previous
                if(do_debug)std::cout<<"Reading close bracket\n";
                execute(stack,true);
            }else if(op_map.find(c)!=op_map.end()){//operator
                if(do_debug)std::cout<<"Reading operation "<<c<<"\n";
                operation op=op_map[c];
                if(do_debug)std::cout<<"Last weight is "<<lastweight<<" current weight is "<<op.weight<<"\n";
                if(op.weight<lastweight){//if the weight is lower, execute previous operations first
                    execute(stack,false,do_debug);
                }
                lastweight=op.weight;
                stack.push(node(1,c));
            }
        }
    }
    if(reading_number==true){
        if(do_debug)std::cout<<"finish reading number, result: "<<number_acc<<"\n";
        stack.push(node(0,number_acc));
        reading_number=false;
    }
    int tries=0;
    while(stack.size()>1){
        tries++;
        if(tries>100){
            break;
        }
        execute(stack,true,do_debug);
    }
    if(tries>100){
        throw std::logic_error("Unexpected loop");
    }else{
        if(stack.top().type==0){
            return stack.top().number_d;
        }else{
            throw std::invalid_argument("Expected double result");
        }
    }
    return -999;
}
