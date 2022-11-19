/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    Carol Mercau
 *    Elijah Harrison
 *    Hunter Powell
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h"

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT>
   friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);

public:
   //
   // construct
   //
   priority_queue()                           { }
   priority_queue(const priority_queue & rhs) { this = rhs; }
   priority_queue(     priority_queue && rhs) { this = std::move(rhs); }
   
   template <class Iterator>
   priority_queue(Iterator first, Iterator last) 
   {
      // newCapacity = last - first;
      // reserve(newCapacity);
      for (auto it = first; it != last; it++)
         container.push_back(*it);
   }
   
   explicit priority_queue(custom::vector<T> && rhs) 
   {
      this->container = rhs.container;
   }
   
   explicit priority_queue(custom::vector<T>& rhs)
   {
      this->container = std::move(rhs.container);
   }
  
  ~priority_queue() {}

   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);     

   //
   // Remove
   //
   void  pop(); 

   //
   // Status
   //
   size_t size() const { return container.size(); }
   bool empty() const { return container.empty(); }
   
private:

   // fix heap from index down. This is a heap index!
   bool percolateDown(size_t indexHeap);

   custom::vector<T> container; 
};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T>
const T & priority_queue <T> :: top() const
{
   assert(!empty());
   return container[0];
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T>
void priority_queue <T> :: pop()
{
   if (!empty())
      std::swap(container.front(), container.back());
   container.pop();
   percolateDown(1);
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T>
void priority_queue <T> :: push(const T & t)
{
}

template <class T>
void priority_queue <T> :: push(T && t)
{
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T>
bool priority_queue <T> :: percolateDown(size_t indexHeap)
{
   return false;
}


/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T>
inline void swap(custom::priority_queue <T>& lhs,
                 custom::priority_queue <T>& rhs)
{
}

};

