#ifndef EVAL_H
#define EVAL_H

#include <functional>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>

class Eval{
        struct node{
            int type;
            union{
                double number_d;
                char operator_c;
            };
            node();
            node(int type,double num);
            node(int type,char op);
        };
        /**
          * type 0 is (double) number
          * type 1 is (long) operator
          * type 2 is open bracket
          */
        struct operation{
            int weight;
            std::function<double(double,double)> action;
        };

        static std::map<char,operation> op_map;
        static std::vector<char> open_brackets;
        static std::vector<char> close_brackets;

        static double add(double,double);
        static double subtract(double,double);
        static double multiply(double,double);
        static double divide(double,double);
        static double power(double,double);
        static double root(double,double);

        static double execute(std::stack<node> &,bool,bool=false);
    public:
        static double do_eval(std::string,bool=false);
};

#endif // EVAL_H
