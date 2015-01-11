/*
 * ===========================================================================
 *
 *         Author:  Ming Chen, v.mingchen@gmail.com
 *        Created:  01/10/2015 10:42:55 AM
 *
 *    Description:  header of hello.h
 *
 *
 * ===========================================================================
 */

#ifndef __HELLO_H__
#define __HELLO_H__

#include <iostream>
#include <string>

class hello {
  private:
    std::string message {"Hello, World!"};
  public:
    hello();                            //default constructor
    hello (const hello&);               //copy constructor
    hello& operator= (const hello&);    //copy operator=
    hello (hello&&);                    //move constructor
    hello& operator= (hello&&);         //move operator=
    ~hello();                           //destructor
    hello (const string&);              //alternate constructor
    void say (ostream&);                //member function

};

#endif
