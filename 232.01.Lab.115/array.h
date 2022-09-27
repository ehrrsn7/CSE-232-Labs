/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *    
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author
 *    Elijah Harrison
 ************************************************************************/

#pragma once

#include <stdexcept>

class TestArray; // forward declaration for unit tests

namespace custom {

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <typename T, int N>
struct array {

public:
   //
   // Construct
   //

   // No constructors or destructors needed here

   //
   // Assign
   //

   // No assignment operators needed here

   //
   // Iterator
   //

   // Forward declaration for the iterator class
   class iterator;

   // Begin and end methods
   iterator begin() {
      if (size() <= 0) return nullptr;
      return iterator(&(__elems_[0]));
   }
   
   iterator end() {
      if (size() <= 0) return nullptr;
      return iterator(&(__elems_[size()]));
   }

   // 
   // Access
   //

   // Subscript
   T& operator [] (size_t index) { return __elems_[index]; }
   const T& operator [] (size_t index) const { return __elems_[index]; }

   // At
   void handleRangeError(size_t index) const {
      if (index >= size()) throw std::out_of_range("index > custom::array{}.size()");
   }
   
   T& at(size_t index) {
      handleRangeError(index);
      return __elems_[index];
   }

   const T& at(size_t index) const {
      handleRangeError(index);
      return __elems_[index];
   }

   // Front and Back
   T& front() { return __elems_[0]; }
   T& back() { return __elems_[size() - 1]; }

   const T& front() const { return __elems_[0]; }
   const T& back()  const { return __elems_[size() - 1]; }

   //
   // Remove

   // No clear or erase here

   //
   // Status
   //
   
   // number of elements in the array
   size_t size() const { return N; }
   
   // array is empty
   bool empty() const { return size() == 0; }

   //
   // Member Variable
   //

   // statically allocated array of T
   T __elems_[N];
};

/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <typename T, int N>
class array <T, N> :: iterator {
   friend class ::TestArray; // give unit tests access to the privates
   
private:
   T* p;

public:
   // constructors, destructors, and assignment operator
   iterator() : p {nullptr} { }
   
   iterator(T* p) : p {p} { }
   
   iterator(const iterator& rhs) : p {rhs.p} { }
   
   iterator& operator = (const iterator& rhs) {
      this->p = rhs.p;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return this->p != rhs.p; }
   bool operator == (const iterator& rhs) const { return this->p == rhs.p; }

   // dereference operator
   T& operator * () { return *p; }
   const T& operator * () const { return *p; }

   // prefix increment
   iterator& operator ++ () {
      this->p++;
      return *this;
   }

   // postfix increment
   iterator operator ++ (int) {
      T* pTemp = this->p;
      this->p++;
      return pTemp;
   }
};

}; // namespace custom
