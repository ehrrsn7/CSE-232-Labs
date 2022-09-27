/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
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
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An interator through Vector
 * Author
 *    Hunter Powell
 *    Carol Mercau
 *    Elijah Harrison
 * 
 * if running from the command line:
 * unix: `g++ -std=c++17 -w testVector.cpp && ./a.out`
 ************************************************************************/

#pragma once

#include <cassert>   // because I am paranoid
#include <new>       // std::bad_alloc
#include <memory>    // for std::allocator
#include <cmath>     // for pow, log2 functions
#include <stdexcept> // for std::out_of_range error

#include <iostream>

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom {

/*****************************************
 * VECTOR
 * Just like the std :: vector <T> class
 ****************************************/
template <typename T>
class vector {

   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;

public:
   
   // 
   // Construct
   //
   vector();
   vector(size_t numElements                );
   vector(size_t numElements, const T & t   );
   vector(const std::initializer_list<T>& l );
   vector(const vector &  rhs);
   vector(      vector && rhs);
   ~vector();

   //
   // Assign
   //
   void swap(vector& rhs) {
      std::swap(this->data, rhs.data);
      std::swap(this->numElements, rhs.numElements);
      std::swap(this->numCapacity, rhs.numCapacity);
   }

   vector & operator = (const vector & rhs);
   vector & operator = (vector && rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin()  { return iterator(data); }
   iterator end()    { return iterator(data + size()); }

   //
   // Access
   //
   const T& operator [] (size_t index) const; // read only
         T& operator [] (size_t index); // read and write
   const T& at(size_t index) const; // ...
         T& at(size_t index);
   const T& front() const;
         T& front();
   const T& back() const;
         T& back();

   //
   // Insert
   //
   void push_back(const T & t);
   void push_back(T && t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //
   void clear() {
      numElements = 0;
   }

   void pop_back() {
      if (!data) return;
      numElements--;
   }
   
   void shrink_to_fit();

   //
   // Status
   //

   size_t   size()      const { return numElements; }
   size_t   capacity()  const { return numCapacity; }
   bool     empty()     const { return !numElements; }

   
   // adjust the size of the buffer
   
   // vector-specific interfaces
   
private:
   
   T *  data;           // user data, a dynamically-allocated array
   size_t numCapacity;  // the capacity of the array
   size_t numElements;  // the number of items currently used
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T>
class vector <T> ::iterator {

   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;

public:

   // constructors, destructors, and assignment operator
   iterator()                             : p (nullptr) { }
   iterator(T * p)                        : p (p) { }
   iterator(const iterator & rhs)         : p (rhs.p) { }
   iterator(size_t index, vector<T>& v)   : p (&v.at(index)) { }
   iterator & operator = (const iterator & rhs) {
      this->p = new T;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const { return this->p == rhs.p; }
   bool operator == (const iterator & rhs) const { return this->p != rhs.p; }

   // dereference operator
   const T & operator * () const { return *p; }
         T & operator * ()       { return *p; }

   // prefix increment
   iterator & operator ++ () {
      this->p++;
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix) {
      T* pTemp = this->p;
      this->p++;
      return pTemp;
   }

   // prefix decrement
   iterator & operator -- () {
      this->p--;
      return *this;
   }

   // postfix decrement
   iterator operator -- (int postfix) {
      T* pTemp = this->p;
      this->p--;
      return pTemp;
   }

private:
   T * p;
};

/*****************************************
 * VECTOR :: DEFAULT constructors
 * Default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector() {
   data = nullptr;
   numElements = 0;
   numCapacity = 0;
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num) {

   // exception: num == 0
   if (!num) { 
      *this = vector();
      return;
   }

   // allocate the minimum power of 2 greater than num
   numElements = num;
   numCapacity = num;
   data = new T[num];

   // fill in the values by calling default constructors
   for (int i = 0; i < numElements; i++) 
      data[i] = T();
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num, const T & t) {

   // set num member variables
   numElements = num;
   numCapacity = (numElements) ? pow(2, ceil(log2(numElements))) : 0;

   // allocate and set data
   data = new T[numCapacity];
   for (int i = 0; i < numElements; i++)
      data[i] = t;
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 * stackoverflow on how to use the syntax for initializer list
 * https://stackoverflow.com/questions/49312484/error-incompatible-types-in-assignment-of-long-int-4-to-long-int-44 
 ****************************************/
template <typename T>
vector <T> :: vector(const std::initializer_list<T> & l) {
   
   numElements = l.size();
   numCapacity = (numElements) ? pow(2, ceil(log2(numElements))) : 0;

   // allocate data
   data = new T[numCapacity];

   // transfer elements from initializer_list to data[]
   int i = 0;
   for (auto it = l.begin(); it != l.end(); it++, i++)
      data[i] = *it;
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T>
vector <T> :: vector (const vector & rhs) {
   
   this->numElements = rhs.numElements;
   this->numCapacity = rhs.numElements;

   this->data = nullptr;
   if (rhs.data) {
      data = new T[this->numCapacity];
      for (int i = 0; i < rhs.numElements; i++)
         data[i] = rhs.data[i];
   }
}

/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T>
vector <T> :: vector (vector && rhs) {
   *this = std::move(rhs);
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T>
vector <T> :: ~vector() {
   data = nullptr;
}  // destructors called automatically

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: resize(size_t newElements) {
   if (newElements > numElements) {
      reserve(newElements);
      for (size_t i = numElements; i < numCapacity; i++)
         data[i] = T();
   }

   numElements = newElements;
}

template <typename T>
void vector <T> :: resize(size_t newElements, const T & t) {
   if (newElements > numElements) {
      reserve(newElements);
      for (size_t i = numElements; i < numCapacity; i++)
         data[i] = t;
   }

   numElements = newElements;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: reserve(size_t newCapacity) {
   if (numCapacity >= newCapacity) return;   // only grow
   T * tmp = data;                           // save current to buffer
   data = new T[newCapacity];                // allocate to new capacity
   numCapacity = newCapacity;                // record that we've reallocated
   for (int i = 0; i < numElements; i++)     // transfer elements
      data[i] = tmp[i];
   delete [] tmp;                            // clean up buffer
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: shrink_to_fit() {
   
   if (numCapacity < numElements) return;    // only shrink
   
   if (!numElements) {                       // shrink to zero
      clear();
      numCapacity = 0;
      data = nullptr;
      return;
   }

   T * tmp = data;                           // save current to buffer
   data = new T[numElements];                // allocate to new capacity
   numCapacity = numElements;                // record that we've reallocated
   for (int i = 0; i < numElements; i++)     // transfer elements
      data[i] = tmp[i];
   delete [] tmp;                            // clean up buffer
}



/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: operator [] (size_t index) {
   return data[index];
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: operator [] (size_t index) const {
   return data[index];
}

/*****************************************
 * VECTOR :: AT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: at (size_t index) {
   if (index > numCapacity) 
      throw std::out_of_range("index > size()");

   if (empty()) 
      throw std::out_of_range("size() == 0");

   return data[index]; // else
}

/******************************************
 * VECTOR :: AT
 * Read-only access
 *****************************************/
template <typename T>
const T & vector <T> :: at (size_t index) const {
   if (index > numCapacity) 
      throw std::out_of_range("index > size()");

   if (empty()) 
      throw std::out_of_range("size() == 0");
      
   return data[index]; // else
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: front () {
   if (empty()) 
      throw std::out_of_range("size() == 0");
      
   return data[0]; // else
}

/******************************************
 * VECTOR :: FRONT
 * Read-only access
 *****************************************/
template <typename T>
const T & vector <T> :: front () const {
   if (empty()) 
      throw std::out_of_range("size() == 0");
      
   return data[0]; // else
}

/*****************************************
 * VECTOR :: BACK
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: back() {
   if (empty()) 
      throw std::out_of_range("size() == 0");
      
   return data[size() - 1]; // else
}

/******************************************
 * VECTOR :: BACK
 * Read-only access
 *****************************************/
template <typename T>
const T & vector <T> :: back() const {
   if (empty()) 
      throw std::out_of_range("size() == 0");
      
   return data[size() - 1]; // else
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T>
void vector <T> :: push_back (const T & t) {

   // if we haven't yet allocated data[]
   if (!data) data = new T[numCapacity];

   // make sure we can fit a new element in the array
   while (numCapacity <= numElements) {
      
      // set new capacity size
      numCapacity *= 2;
      if (!numCapacity) numCapacity = 1; // handle if cap is 0

      // reallocate data using buffer
      T * tmp = data;            // save into buffer
      data = new T[numCapacity]; // allocate new array with new cap
      for (int i = 0; i < numCapacity; i++)  
         data[i] = tmp[i];       // transfer elements from tmp to data
   }
   
   // now that we know we have enough space for it, set new element
   data[size()] = t;
   numElements++;
}

// v.push_back(std::move(newElement))
template <typename T>
void vector <T> :: push_back(T && t) {

   // if we haven't yet allocated data[]
   if (!data) data = new T[numCapacity];

   // make sure we can fit a new element in the array
   while (numCapacity <= numElements) {
      
      // set new capacity size
      numCapacity *= 2;
      if (!numCapacity) numCapacity = 1; // handle if cap is 0

      // reallocate data using buffer
      T * tmp = data;            // save into buffer
      data = new T[numCapacity]; // allocate new array with new cap
      for (int i = 0; i < numCapacity; i++)  
         data[i] = tmp[i];       // transfer elements from tmp to data
   }
   
   // now that we know we have enough space for it, set new element
   data[size()] = std::move(t);
   numElements++;
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T>
vector <T> & vector <T> :: operator = (const vector & rhs) {
   
   if (rhs.empty()) {
      // for efficiency, we'll set defaults for empty vector rhs and quick exit
      numElements = 0;
      numCapacity = 0;
      data = nullptr;
      return *this;
   }
   
   // copy constructor keeps original this->data[],
   // and only grows to fit rhs but does not shrink
   if (rhs.numCapacity > this->numCapacity) {
      delete [] data;
      data = new T[rhs.numCapacity];
      this->numCapacity = rhs.numCapacity; // keep track of numCapacity
   }

   // transfer rhs.data to this->data (we made sure capacity is big enough already)
   this->numElements = rhs.numElements; // keep track of numElements
   for (int i = 0; i < numElements; i++) 
      data[i] = rhs.data[i];

   // return reference to this
   return *this;
}

template <typename T>
vector <T>& vector <T> :: operator = (vector && rhs) {
   clear();
   shrink_to_fit();
   swap(rhs);
   return *this;
}

} // namespace custom
