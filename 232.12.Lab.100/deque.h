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

// Debug stuff
#include <cassert>

class TestDeque;    // forward declaration for TestDeque unit test class

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
  ~deque() 
   {
      // delete data;
      data = nullptr;
   }

   //
   // Assign
   //
   deque<T> & operator = (const deque<T> & rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin() { return iterator(this, iaFront); }
   iterator end() { return iterator(); }

   //
   // Access
   //
   const T & operator[] (size_t id) const { return data[iaFromID(id)]; }
         T & operator[] (size_t id)       { return data[iaFromID(id)]; }
   const T & front() const                { return data[iaFront]; }
         T & front()                      { return data[iaFront]; }
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
   void reallocate(size_t newCapacity);
   
private:
   
   // fetch array index from the deque index
   size_t iaFromID(size_t id) const
   {
      int numCell = numCapacity;
      int numBlock = 1;
      assert (0 <= id && id < numElements);
      // assert (0 <= iaFront && iaFront < numCell); // this assert is invalid because the array wraps!
      int ia = (id + iaFront) % numCell;
      assert (0 <= ia && ia < (numCell * numBlock));
      return ia;
   }

   void resize(size_t newCapacity = 0);

   // member variables
   T * data;           // dynamically allocated data for the deque
   size_t numCapacity; // the size of the data array
   size_t numElements; // number of elements in the deque
   size_t iaFront;        // the index of the first item in the array
};

/**********************************************************
 * DEQUE ITERATOR
 * Forward and reverse iterator through a deque, just call
 *********************************************************/
template <typename T>
class deque<T> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   //
   // Construct
   //
   iterator() : pDeque(nullptr), id(0) { }
   iterator(custom::deque<T> * pDeque, size_t id) : pDeque(pDeque), id(id) { }
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
   size_t operator - (iterator rhs) const { return id - rhs.id; }

   iterator & operator += (size_t offset)
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
   size_t id; // deque index
   deque<T> * pDeque;
};


/****************************************************
 * DEQUE : CONSTRUCTOR - fill
 ***************************************************/
template <class T>
deque<T>::deque(size_t newCapacity) 
   : data(nullptr), numCapacity(newCapacity), numElements(newCapacity), iaFront(0)
{
   data = new T[newCapacity];
   for (size_t i = 0; i < newCapacity; i++)
      data[i] = T();
}

/****************************************************
 * DEQUE : ASSIGNMENT OPERATOR
 ***************************************************/
template <class T>
deque<T> & deque<T>::operator = (const deque<T> & rhs)
{
   data = rhs.data;
   numCapacity = rhs.numCapacity;
   numElements = rhs.numElements;
   iaFront = rhs.iaFront;
   return *this;
}

/**************************************************
 * DEQUE :: BACK
 * Fetch the item that is at the end of the deque
 *************************************************/
template <class T>
const T & deque<T>::back() const 
{
   assert (numElements);
   return data[iaFromID(numElements - 1)];
}

template <class T>
T & deque<T> ::back()
{
   assert (numElements);
   return data[iaFromID(numElements - 1)];
}

/*****************************************************
 * DEQUE : POP_FRONT
 *****************************************************/
template <class T>
void deque<T>::pop_front()
{
   numElements--;
   iaFront++;
   if (iaFront == numCapacity)
      iaFront = 0;
}

/******************************************************
 * DEQUE : PUSH_BACK
 ******************************************************/
template <class T>
void deque<T>::push_back(const T & t) 
{
   // reallocate
   if (numElements == numCapacity)
   {
      if (!numCapacity)
         reallocate(1);
      else
         reallocate(numCapacity * 2);
   }
   
   // push back
   data[iaFromID(numElements++)] = t;
}

/******************************************************
 * DEQUE : PUSH_FRONT
 ******************************************************/
template <class T>
void deque<T>::push_front(const T & t) 
{
   if (!numElements)
      reallocate(1);
   else if (numElements == numCapacity)
      reallocate(numCapacity * 2);
   iaFront--;
   if (iaFront < 0)
      iaFront = numCapacity - 1;
   data[iaFront] = t;
   numElements++;
}

/****************************************************
 * DEQUE :: GROW
 * If the deque is currently empty, allocate to size 2.
 * Otherwise, double the size
 ***************************************************/
template <class T>
void deque<T>::resize(size_t newCapacity)
{
   if (newCapacity > size())
      while (newCapacity > size())
         push_back(T());
         
   else if (newCapacity < size())
      while (newCapacity < size())
         pop_back();
}

template <class T>
void deque<T>::reallocate(size_t newCapacity)
{
   assert (newCapacity > 0 && newCapacity > numElements);
   auto tmp = new T[newCapacity];
   for (size_t id = 0; id < numElements; id++)
      tmp[id] = data[iaFromID(id)];
   numCapacity = newCapacity;
   iaFront = 0;
   delete data;
   data = tmp;
}

} // namespace custom
