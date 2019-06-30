#ifndef EVAL_H
#define EVAL_H

#include <memory>
#include <stack>
#include <vector>

enum operation_t{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
};

class Token {
    public:
        inline virtual void dummy() {}
};

class NumberToken : public Token {
    public:
        NumberToken(double);
        double num;
};

class OperatorToken : public Token {
    public:
        OperatorToken(operation_t);
        operation_t op;
};

class LBToken : public Token {};

class RBToken : public Token {};

class Eval{
        static std::vector<std::shared_ptr<Token>> lex(std::string str);
        static void parseBracket(uint32_t &i,std::vector<std::shared_ptr<Token>> &tks,std::stack<std::shared_ptr<Token>> &out);
        static void read_tk(uint32_t &i,std::stack<std::shared_ptr<OperatorToken>> &ops,std::vector<std::shared_ptr<Token>> &tks,std::stack<std::shared_ptr<Token>> &out,bool &last_num);
        static std::stack<std::shared_ptr<Token>> parse(std::vector<std::shared_ptr<Token>> tks);
        std::stack<std::shared_ptr<Token>> data;
        double eval_op(std::stack<std::shared_ptr<Token>> &st,std::shared_ptr<OperatorToken> op);
        double eval_op(std::stack<std::shared_ptr<Token>> &st);
    public:
        Eval(std::string);//parse
        double eval();//execute
};

#endif // EVAL_H
