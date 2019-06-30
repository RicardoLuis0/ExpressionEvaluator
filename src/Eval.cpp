#include <iostream>
#include <stdexcept>
#include <cmath>
#include "Eval.h"

#include <map>

#define IS(ptr,type) (typeid(*(ptr))==typeid(type))

NumberToken::NumberToken(double d):num(d) {}

OperatorToken::OperatorToken(operation_t o):op(o) {}

static std::map<char,operation_t> operation_chars{
    {'+',OP_ADD},
    {'-',OP_SUB},
    {'*',OP_MUL},
    {'/',OP_DIV},
    {'^',OP_POW},
};

static std::map<operation_t,int> precedence{
    {OP_ADD,3},
    {OP_SUB,3},
    {OP_MUL,2},
    {OP_DIV,2},
    {OP_POW,1},
};

std::vector<std::shared_ptr<Token>> Eval::lex(std::string str){
    std::vector<std::shared_ptr<Token>> out;
    bool reading_number=false;
    bool reading_decimal=false;
    std::string buffer;
    for(char c:str){
        if(reading_number){
            if(c=='.'||c==','){
                if(reading_decimal){
                    throw std::runtime_error("invalid number '"+buffer+".'");
                }else{
                    buffer+='.';
                    reading_decimal=true;
                }
                continue;
            }else if(c>='0'&&c<='9'){
                buffer+=c;
                continue;
            }else{
                out.push_back(std::make_shared<NumberToken>(stod(buffer)));
                reading_number=false;
                reading_decimal=false;
            }
        }
        if(c>='0'&&c<='9'){
            buffer=c;
            reading_number=true;
            reading_decimal=false;
        }else if(c=='.'||c==','){
            buffer='.';
            reading_number=true;
            reading_decimal=true;
        }else if(operation_chars.find(c)!=operation_chars.end()){
            out.push_back(std::make_shared<OperatorToken>(operation_chars[c]));
        }else if(c=='('){
            out.push_back(std::make_shared<LBToken>());
        }else if(c==')'){
            out.push_back(std::make_shared<RBToken>());
        }else{
            throw std::runtime_error("unexpected characted '"+(std::string()=c)+"'");
        }
    }
    if(reading_number){
        out.push_back(std::make_shared<NumberToken>(stod(buffer)));
    }
    return out;
}

void Eval::parseBracket(uint32_t &i,std::vector<std::shared_ptr<Token>> &tks,std::stack<std::shared_ptr<Token>> &out){
    bool last_num=false;
    std::stack<std::shared_ptr<OperatorToken>> ops;
    for(;i<tks.size();i++){
        if(IS(tks[i],RBToken)){// ')'
            while(!ops.empty()){
                out.push(ops.top());
                ops.pop();
            }
            return;
        }
        read_tk(i,ops,tks,out,last_num);
    }
    throw std::runtime_error("mismatched brackets, expecting ')' before end of expression");
}

void Eval::read_tk(uint32_t &i,std::stack<std::shared_ptr<OperatorToken>> &ops,std::vector<std::shared_ptr<Token>> &tks,std::stack<std::shared_ptr<Token>> &out,bool &last_num){
    std::shared_ptr<Token> tk(tks[i]);
    if(IS(tk,LBToken)){// '('
        if(last_num)ops.push(std::make_shared<OperatorToken>(OP_MUL));
        last_num=false;
        i++;
        parseBracket(i,tks,out);
    }else if(IS(tk,OperatorToken)){
        last_num=false;
        std::shared_ptr<OperatorToken> op(std::static_pointer_cast<OperatorToken>(tk));
        while((!ops.empty())&&precedence[op->op]>precedence[ops.top()->op]){
            out.push(ops.top());
            ops.pop();
        }
        ops.push(op);
    }else{
        last_num=true;
        out.push(tk);
    }
}

std::stack<std::shared_ptr<Token>> Eval::parse(std::vector<std::shared_ptr<Token>> tks){
    bool last_num=false;
    std::stack<std::shared_ptr<Token>> out;
    std::stack<std::shared_ptr<OperatorToken>> ops;
    for(uint32_t i=0;i<tks.size();i++){
        if(IS(tks[i],RBToken)){// ')'
            throw std::runtime_error("mismatched brackets, ')' without '('");
        }
        read_tk(i,ops,tks,out,last_num);
    }
    while(!ops.empty()){
        out.push(ops.top());
        ops.pop();
    }
    return out;
}

Eval::Eval(std::string str):data(parse(lex(str))){
    
}

double Eval::eval_op(std::stack<std::shared_ptr<Token>> &st,std::shared_ptr<OperatorToken> op){
    std::shared_ptr<Token> t2(st.top());
    double t2val=0;
    st.pop();
    if(IS(t2,OperatorToken)){
        t2val=eval_op(st,std::static_pointer_cast<OperatorToken>(t2));
    }else{
        t2val=std::static_pointer_cast<NumberToken>(t2)->num;
    }
    std::shared_ptr<Token> t1(st.top());
    double t1val=0;
    st.pop();
    if(IS(t1,OperatorToken)){
        t1val=eval_op(st,std::static_pointer_cast<OperatorToken>(t1));
    }else{
        t1val=std::static_pointer_cast<NumberToken>(t1)->num;
    }
    switch(op->op){
    case OP_ADD:
        return t1val+t2val;
    case OP_SUB:
        return t1val-t2val;
    case OP_MUL:
        return t1val*t2val;
    case OP_DIV:
        return t1val/t2val;
    case OP_POW:
        return std::pow(t1val,t2val);
    default:
        throw std::runtime_error("invalid operator");
    }
}

double Eval::eval_op(std::stack<std::shared_ptr<Token>> &st){
    std::shared_ptr<OperatorToken> op(std::static_pointer_cast<OperatorToken>(st.top()));
    st.pop();
    return eval_op(st,op);
}

double Eval::eval(){
    std::stack<std::shared_ptr<Token>> st=data;
    if(IS(st.top(),OperatorToken)){
        return eval_op(st);
    }else{
        return std::static_pointer_cast<NumberToken>(st.top())->num;
    }
}
