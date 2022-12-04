/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a binary search tree
 *        deque::iterator       : An iterator through BST
 * Authors
 *    Hunter Powell
 *    Carol Mercau
 *    Elijah Harrison
 ************************************************************************/

#pragma once

#include <cassert> // Debug stuff

class TestDeque; // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *   0   1   2   3   4
 * +---+---+---+---+---+
 * |   | A | B | C |   |
 * +---+---+---+---+---+
 * iaFront = 1
 * numElements = 3
 * numCapacity = 5
 *****************************************************/
template <class T>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque() : data(nullptr), numCapacity(0), numElements(0), iaFront(0) { }
   deque(const deque<T> & rhs) { *this = rhs; }
   deque(size_t); // fill constructor
  ~deque() { }

   //
   // Assign
   //
   deque<T> & operator = (const deque<T> & rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin() { return iterator(this, iaFront); }
   iterator end()   { return iterator(this, iaFront + (int)numElements); }

   //
   // Access
   //
   const T & operator[] (int id) const { return data[iaFromID(id)]; }
         T & operator[] (int id)       { return data[iaFromID(id)]; }
   const T & front() const             { return data[iaWrap(iaFront)]; }
         T & front()                   { return data[iaWrap(iaFront)]; }
   const T & back() const;
         T & back();

   // 
   // Insert
   //
   void push_front(const T & t);
   void push_back(const T & t);

   //
   // Remove
   //
   void pop_front();
   void pop_back() { numElements--; }

   void clear()
   {
      delete data;
      data = nullptr;
      numElements = 0;
   }

   // 
   // Status
   //
   size_t size() const { return numElements; }
   bool empty() const { return !numElements; }
   void reallocate(int newCapacity);
   
private:
   
   // fetch array index from the deque index
   int iaFromID(int id) const
   {
      // assert
      assert (0 <= id && id < numElements);

      // add id to iaFront, then wrap and return
      return iaWrap(id + iaFront); // ia
   }

   // return value which is within 0 <= ia < numCapacity
   int iaWrap(int ia) const
   {
      // ia could be negative, and the % operator doesn't handle making it positive
      while (ia < 0)
         ia += numCapacity;
      
      // ia could be greater than numCapacity
      ia %= numCapacity;
      
      // assert and return :)
      assert (0 <= ia && ia < numCapacity);
      return ia;
   }

   // member variables
   T * data;           // dynamically allocated data for the deque
   size_t numCapacity; // the size of the data array
   size_t numElements; // number of elements in the deque
   int iaFront;        // the index of the first item in the array
};

/**********************************************************
 * DEQUE ITERATOR
 * Forward and reverse iterator through a deque, just call
 *********************************************************/
template <typename T>
class deque<T>::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   //
   // Construct
   //
   iterator() : pDeque(nullptr), id(0) { }
   iterator(custom::deque<T> * pDeque, int id) : pDeque(pDeque), id(id) { }
   iterator(const iterator & rhs) { *this = rhs; }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
      pDeque = rhs.pDeque;
      id = rhs.id;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const { return *this == rhs; }
   bool operator != (const iterator & rhs) const { return *this != rhs; }

   // 
   // Access
   //
   const T & operator * () const { return (*pDeque)[id]; }
         T & operator * ()       { return (*pDeque)[id]; }

   // 
   // Arithmetic
   //
   int operator - (iterator rhs) const { return id - rhs.id; }

   iterator & operator += (int offset)
   {
      id += offset;
      return *this;
   }

   iterator & operator ++ ()
   {
      id++;
      return *this;
   }

   iterator operator ++ (int postfix)
   {
      auto tmp = *this;
      id++;
      return tmp;
   }

   iterator & operator -- ()
   {
      id--;
      return *this;
   }

   iterator  operator -- (int postfix)
   {
      auto tmp = *this;
      id--;
      return tmp;
   }

private:

   // Member variables
   int id; // deque index
   deque<T> * pDeque;
};


/****************************************************
 * DEQUE : CONSTRUCTOR - fill
 ***************************************************/
template <class T>
deque<T>::deque(size_t newCapacity) 
   : data(nullptr), numCapacity(newCapacity), numElements(newCapacity), iaFront(0)
{
   // allocate
   data = new T[newCapacity];

   // fill
   for (size_t i = 0; i < newCapacity; i++)
      data[i] = T();
}

/****************************************************
 * DEQUE : ASSIGNMENT OPERATOR
 ***************************************************/
template <class T>
deque<T> & deque<T>::operator = (const deque<T> & rhs)
{
   numElements = rhs.numElements;
   iaFront = 0;
   int rhsI = rhs.iaFront;
   if (numCapacity < rhs.numCapacity)
   {
      numCapacity = rhs.numCapacity;
      data = new T[numCapacity];
      for (size_t i = 0; i < rhs.numCapacity; i++)
      {
         if (rhsI >= numCapacity)
            rhsI = 0;
         data[i] = rhs.data[rhsI];
         rhsI++;
      }

   }
   else
      data = rhs.data;
   return *this;
}

/**************************************************
 * DEQUE :: BACK
 * Fetch the item that is at the end of the deque
 *************************************************/
template <class T>
const T & deque<T>::back() const 
{
   assert (numElements && numCapacity);
   return data[iaFromID(numElements - 1)];
}

template <class T>
T & deque<T>::back()
{
   assert (numElements && numCapacity);
   return data[iaFromID((int)numElements - 1)];
}

/*****************************************************
 * DEQUE : POP_FRONT
 *****************************************************/
template <class T>
void deque<T>::pop_front()
{
   // shift iaFront/id
   iaFront++;
   if (iaFront == numCapacity)
      iaFront = iaWrap(iaFront);
   
   // pop front
   numElements--;
}

/******************************************************
 * DEQUE : PUSH_BACK
 ******************************************************/
template <class T>
void deque<T>::push_back(const T & t) 
{
   // reallocate
   if (numElements == numCapacity)
      (numCapacity) ? reallocate((int)numCapacity * 2) : reallocate(1);
   
   // push back
   data[iaFromID((int)numElements++)] = t;
}

/******************************************************
 * DEQUE : PUSH_FRONT
 ******************************************************/
template <class T>
void deque<T>::push_front(const T & t) 
{
   // reallocate
   if (numElements == numCapacity)
      (numCapacity) ? reallocate((int)numCapacity * 2) : reallocate(1);
   
   // shift iaFront/id
   iaFront--;

   // push_front
   data[iaWrap(iaFront)] = t;
   numElements++;
}

/****************************************************
 * DEQUE :: GROW
 ***************************************************/
template <class T>
void deque<T>::reallocate(int newCapacity)
{
   assert (newCapacity > 0 && newCapacity > numElements);

   // use buffer to reallocate
   auto tmp = new T[newCapacity];
   for (int id = 0; id < numElements; id++)
      tmp[id] = data[iaFromID(id)];
   delete data;
   data = tmp;

   // set attributes
   numCapacity = newCapacity;
   iaFront = 0;
}

} // namespace custom
