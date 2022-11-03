#ifndef __MYSTACK_H__
#define __MYSTACK_H__

#include <iostream>
#include <algorithm>

#include "MyVector_3052436.h"

template <typename DataType>
class MyStack : private MyVector<DataType>
{
  private:
    int topOfStack;
    MyVector<DataType> theArray;
  public:

    // default constructor
    explicit MyStack(size_t initSize = 0)
    {
        // code begins
        topOfStack = -1;
        // code ends
    }

    // copy constructor
    MyStack(const MyStack & rhs)
    {
        // code begins
        topOfStack = rhs.topOfStack;
        for(int i = 0; i < topOfStack + 1; i++){
           theArray.push_back(rhs.theArray[i]);
        }
        // code ends
    }

    // move constructor
    MyStack(MyStack && rhs)
    {
        // code begins
        topOfStack = rhs.topOfStack;
        for(int i = 0; i < topOfStack + 1; i++){
          theArray.push_back(rhs.theArray[i]);
        }
        rhs.topOfStack = -1;
        // code ends
    }

    // destructor
    ~MyStack()
    {
        ;
    }

    // copy assignment
    MyStack & operator= (const MyStack & rhs)
    {
        // code begins
        MyStack copy(rhs);
        std::swap(*this, copy);
        return *this;
        // code ends
    }

    // move assignment
    MyStack & operator= (MyStack && rhs)
    {
        // code begins
        std::swap(topOfStack, rhs.topOfStack);
        for(int i = 0; i < topOfStack + 1; i++){
          theArray.push_back(rhs.theArray[i]);
        }
        return *this;
        // code ends
    }

    // insert x to the stack
    void push(const DataType & x)
    {
        // code begins
        theArray.push_back(x);
        topOfStack++;
        // code ends
    }

    // insert x to the stack
    void push(DataType && x)
    {
        // code begins
        theArray.push_back(std::move(x));
        topOfStack++;
        // code ends
    }

    // remove the last element from the stack
    void pop(void)
    {
        // code begins
        theArray.pop_back();
        topOfStack--;
        // code ends
    }

    // access the last element of the stack
    const DataType & top(void) const
    {
        // code begins
        return theArray.back();
        // code ends
    }

    // check if the stack is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        // code begins
        if(topOfStack == -1){
          return true;
        } else{
          return false;
        }
        // code ends
    }

    // access the size of the stack
    size_t size() const
    {
        // code begins
        return theArray.size();
        // code ends
    }

    // access the capacity of the stack
    size_t capacity(void) const
    {
        // code begins
        return theArray.capacity();
        // code ends
    }

};


#endif // __MYSTACK_H__
