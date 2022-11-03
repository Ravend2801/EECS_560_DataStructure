#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include <iostream>
#include <algorithm>

#include "MyVector_3052436.h"

template <typename DataType>
class MyQueue : private MyVector<DataType>
{
  private:
    size_t dataStart, dataEnd;
    MyVector<DataType> theArray;
    // changes the size of the array to newSize
    void resize(size_t newSize)
    {
        // code begins
        theArray.resize();
        // code ends
    }

    // requests for newCapacity amount of space
    void reserve(size_t newCapacity)
    {
        // code begins
        theArray.reserve();
        // code ends
    }

  public:

    // default constructor
    explicit MyQueue(size_t initSize = 0)
    {
        // code begins
        dataStart = dataEnd = 0;
        // code ends
    }

    // copy constructor
    MyQueue(const MyQueue & rhs)
    {
        // code begins
        dataStart = rhs.dataStart;
        dataEnd = rhs.dataEnd;
        for(int i = dataStart; i < dataEnd; i++){
          theArray.push_back(rhs.theArray[i]);
        }
        // code ends
    }

    // move constructor
    MyQueue(MyQueue && rhs)
    {
        // code begins
        dataStart = rhs.dataStart;
        dataEnd = rhs.dataEnd;
        for(int i = 0; i < dataEnd; i++){
          theArray.push_back(rhs.theArray[i]);
        }
        rhs.dataStart = rhs.dataEnd = 0;
        // code ends
    }

    // destructor
    ~MyQueue()
    {
        // code begins

        // code ends
    }

    // copy assignment
    MyQueue & operator= (const MyQueue & rhs)
    {
        // code begins
        MyQueue copy(rhs);
        std::swap(*this, copy);
        return *this;
        // code ends
    }

    // move assignment
    MyQueue & operator= (MyQueue && rhs)
    {
        // code begins
        std::swap(dataStart, rhs.dataStart);
        std::swap(dataEnd, rhs.dataEnd);
        for(int i = 0; i < dataEnd; i++){
          theArray.push_back(rhs.theArray[i]);
        }
        return *this;
        // code ends
    }

    // insert x into the queue
    void enqueue(const DataType & x)
    {
        // code begins
        theArray.push_back(x);
        dataEnd++;
        // code ends
    }

    // insert x into the queue
    void enqueue(DataType && x)
    {
        // code begins
        theArray.push_back(std::move(x));
        dataEnd++;
        // code ends
    }

    // remove the first element from the queue
    void dequeue(void)
    {
        // code begins
        for(int i = 0; i < dataEnd - 1; i++){
          theArray[i] = theArray[i + 1];
        }
        theArray.pop_back();
        dataEnd--;
        // code ends
    }

    // access the first element of the queue
    const DataType & front(void) const
    {
        // code begins
        return theArray[dataStart];
        // code ends
    }

    // check if the queue is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        // code begins
        if(dataStart == dataEnd){
          return true;
        } else{
          return false;
        }
        // code ends
    }

    // access the size of the queue
    size_t size() const
    {
        // code begins
        return theArray.size();
        // code ends
    }

    // access the capacity of the queue
    size_t capacity(void) const
    {
        // code begins
        return theArray.capacity();
        // code ends
    }

};


#endif // __MYQUEUE_H__
