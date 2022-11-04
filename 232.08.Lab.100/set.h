/***********************************************************************
 * Header:
 *    Set
 * Summary:
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        set                 : A class that represents a Set
 *        set::iterator       : An iterator through Set
 * Authors
 *    Hunter Powell
 *    Carol Mercau
 *    Elijah Harrison
 ************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   // 
   // Construct
   //
   set() { }
   set(const set & rhs) : bst(rhs.bst) { }
   set(set && rhs) { bst = std::move(rhs.bst); }
   set(const std::initializer_list <T> & il) { bst = il; }
   
   template <class Iterator>
   set(Iterator first, Iterator last) 
   { 
      for (auto it = first; it != last; it++)
         bst.insert(*it);
   }

  ~set() { }

   //
   // Assign
   //
   set & operator = (const set & rhs)
   {
      bst = rhs.bst;
      return *this;
   }

   set & operator = (set && rhs)
   {
      bst = std::move(rhs.bst);
      return *this;
   }

   set & operator = (const std::initializer_list <T> & il)
   {
      bst = il;
      return *this;
   }

   void swap(set & rhs) noexcept
   {
      bst.swap(rhs.bst);
   }

   //
   // Iterator
   //
   class iterator;

   iterator begin() const noexcept 
   { 
      return bst.begin(); 
   }
   
   iterator end() const noexcept 
   { 
      return bst.end(); 
   }

   //
   // Access
   //
   iterator find(const T & t) 
   { 
      return bst.find(t); 
   }

   //
   // Status
   //
   bool empty() const noexcept 
   { 
      return bst.empty();
   }

   size_t size() const noexcept 
   { 
      return bst.size();
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T & t)
   {
      return bst.insert(t);
   }
   
   std::pair<iterator, bool> insert(T && t)
   {
      std::pair<iterator, bool> p(iterator(), true);
      return p;
   }

   void insert(const std::initializer_list <T> & il)
   {
      for (auto && t : il)
         insert(t);
   }
   
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      for (auto it = first; it != last; it++)
         insert(*it);
   }

   //
   // Remove
   //
   void clear() noexcept 
   {
      bst.clear();
   }
   
   iterator erase(iterator & it)
   {
      it = bst.erase(it.it);
      return it;
   }

   size_t erase(const T & t) 
   {
      auto it = find(t);

      if (it == end())
         return 0;

      erase(it);
      
      return 1;
   }
   
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
      auto it = itBegin;

      while (it != itEnd)
         it = erase(it);

      return it;
   }

private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;

public:
   // constructors, destructors, and assignment operator
   iterator() { }

   iterator(const typename custom::BST<T>::iterator & it) { this->it = it; }

   iterator(const iterator & rhs) { it = rhs.it; }

   iterator & operator = (const iterator & rhs)
   {
      it = rhs.it;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      return it != rhs.it; 
   }

   bool operator == (const iterator & rhs) const 
   { 
      return it == rhs.it;
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
      return *it; 
   }

   // prefix increment
   iterator & operator ++ ()
   {
      ++it;
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
      auto tmp = *this;
      it++;
      return tmp;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
      --it;
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
      auto tmp = *this;
      it--;
      return tmp;
   }
   
private:

   typename custom::BST<T>::iterator it;
};

/***********************************************
 * SET : EQUIVALENCE
 * See if two sets are the same size
 ***********************************************/
template <typename T>
bool operator == (const set <T> & lhs, const set <T> & rhs)
{
   return lhs.size() == rhs.size();
}

template <typename T>
inline bool operator != (const set <T> & lhs, const set <T> & rhs)
{
   return lhs.size() != rhs.size();
}

/***********************************************
 * SET : RELATIVE COMPARISON
 * See if one set is lexicographically before the second
 ***********************************************/
template <typename T>
bool operator < (const set <T> & lhs, const set <T> & rhs)
{
   return lhs.size() < rhs.size();
}

template <typename T>
inline bool operator > (const set <T> & lhs, const set <T> & rhs)
{
   return lhs.size() > rhs.size();
}

}; // namespace custom
