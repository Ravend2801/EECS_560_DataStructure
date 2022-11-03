#ifndef __MYINFIXCALCULATOR_H__
#define __MYINFIXCALCULATOR_H__

#include <algorithm>
#include <string>

#include "MyStack_3052436.h"
#include "MyVector_3052436.h"

class MyInfixCalculator{

  public:
    
    MyInfixCalculator()
    {

    }

    ~MyInfixCalculator()
    {
     
    }

    double calculate(const std::string& s)
    {
        // code begins
        MyVector<std::string> prefix;
        MyVector<std::string> postfix;
        tokenize(s, prefix);
        infixToPostfix(prefix, postfix);
        return calPostfix(postfix);
        // code ends
    }

    private:

    // returns operator precedance; the smaller the number the higher precedence
    // returns -1 if the operator is invalid
    // does not consider parenthesis
    int operatorPrec(const char c) const
    {
        switch(c)
        {
            case '*':
                return 2;
            case '/':
                return 2;
            case '+':
                return 3;
            case '-':
                return 3;
            default:
                return -1;
        }
    }

    // checks if a character corresponds to a valid parenthesis
    bool isValidParenthesis(const char c) const
    {
        switch(c)
        {
            case '(':
                return true;
            case ')':
                return true;
            default:
                return false;
        }
    }

    // checks if a character corresponds to a valid digit
    bool isDigit(const char c) const
    {
        if(c >= '0' && c <= '9')
            return true;
        return false;
    }

    // computes binary operation given the two operands and the operator in their string form
    double computeBinaryOperation(const std::string& ornd1, const std::string& ornd2, const std::string& opt) const
    {
        double o1 = std::stod(ornd1);
        double o2 = std::stod(ornd2);
        switch(opt[0])
        {
            case '+':
                return o1 + o2;
            case '-':
                return o1 - o2;
            case '*':
                return o1 * o2;
            case '/':
                return o1 / o2;
            default:
                std::cout << "Error: unrecognized operator: " << opt << std::endl;
                return 0.0;
        }
    }


    // tokenizes an infix string s into a set of tokens (operands or operators)
    void tokenize(const std::string& s, MyVector<std::string>& tokens)
    {
        // code begins
        std::string temp;
        int i = 0;
        while (i<s.length()) {
            if (isValidParenthesis(s[i])) {
                temp += s[i++];
            } else if (operatorPrec(s[i]) >= 2 && isDigit(s[i-1]) && (i!=0)) {
                temp += s[i++];
            } else if (operatorPrec(s[i]) >= 2 && s[i-1] == ')' && (i!=0)) {
                temp += s[i++];
            } else {
                temp += s[i++];
                while ((isDigit(s[i]) || s[i] == '.')) {
                    temp += s[i++];
                }
            }
            tokens.push_back(temp);
            temp.clear();
        }
        // code ends
    }

    // converts a set of infix tokens to a set of postfix tokens
    void infixToPostfix(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens)
    {
        // code begins
        MyStack<std::string> temp;
        for (size_t i = 0; i < infix_tokens.size(); ++i) {
            std::string tok = infix_tokens[i];
            if (operatorPrec(tok[0]) == -1 || tok.size() > 1) {
                if (isValidParenthesis(tok[0])) {
                    if (tok == "(") {
                        temp.push(tok);
                    } else {
                        while (temp.top() != "(") {
                            postfix_tokens.push_back(temp.top());
                            temp.pop();
                        } 
                        temp.pop();
                    }
                } else {
                    postfix_tokens.push_back(infix_tokens[i]);
                }
            } else {
                
                if (temp.size() == 0) {
                    temp.push(tok);
                } else if (temp.top() == "(") {
                    temp.push(tok);
                } else {
                    while (operatorPrec(tok[0]) >= operatorPrec(temp.top()[0]) && !isValidParenthesis(temp.top()[0]))         {
                        postfix_tokens.push_back(temp.top());
                        temp.pop();
                        if (temp.size()==0) break;
                    }
                    temp.push(tok);
                }
            }
        }
        while (temp.size() > 0) {
            postfix_tokens.push_back(temp.top());
            temp.pop();
        }
        // code ends
    }

    // calculates the final result from postfix tokens
    double calPostfix(const MyVector<std::string>& postfix_tokens) const
    {
        // code begins
        MyStack<std::string> stack_result;
        double result;
        for (size_t i = 0; i < postfix_tokens.size(); ++i) {
            if (operatorPrec(postfix_tokens[i][0]) == -1 || postfix_tokens[i].size() >1) {
                stack_result.push(postfix_tokens[i]);
            } else {
                std::string o1 = stack_result.top();
                stack_result.pop();
                std::string o2 = stack_result.top();
                stack_result.pop();
                result = computeBinaryOperation(o2, o1, postfix_tokens[i]);
                stack_result.push(std::to_string(result));
            }
        }
        return result;
        // code ends
    }
};

#endif // __MYINFIXCALCULATOR_H__

