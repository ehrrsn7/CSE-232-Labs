/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Authors
 *    Hunter Powell
 *    Carol Mercau
 *    Elijah Harrison
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include "vector.h"
// #include <vector>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates

public:
  
   // 
   // Construct
   // 

   stack()                            {                              }
   stack(const stack <T> &  rhs)      { *this = rhs;                 }
   stack(      stack <T> && rhs)      { *this = std::move(rhs);      }
   stack(const std::vector<T> &  rhs) { container = rhs;             }
   stack(      std::vector<T> && rhs) { container = std::move(rhs);  }
   ~stack()                           {                              }

   //
   // Assign
   //
   stack <T> & operator = (const stack <T> & rhs)
   {
      container = rhs.container;
      return *this;
   }

   stack <T>& operator = (stack <T> && rhs)
   {
      container = std::move(rhs.container);
      return *this;
   }

   void swap(stack <T>& rhs)
   {
      container.swap(rhs.container);
   }

   // 
   // Access
   //
         T& top()       { return container.back(); }
   const T& top() const { return container.back(); }

   // 
   // Insert
   //
   void push(const T&  t) { container.push_back(t); }
   void push(      T&& t) { container.push_back(std::move(t)); }

   //
   // Remove
   //
   void pop() 
   { 
      container.pop_back();
   }

   //
   // Status
   //
   size_t   size()   const { return container.size(); }
   bool     empty()  const { return container.empty(); }
   
private:
   std::vector<T> container;  // underlying container
};

} // custom namespace
