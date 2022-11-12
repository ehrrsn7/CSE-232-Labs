/***********************************************************************
 * Header:
 *    map
 * Summary:
 *    Our custom implementation of a std::map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    Hunter Powell
 *    Elijah Harrison
 *    Carol Mercau
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend void swap(map<KK, VV> & lhs, map<KK, VV> & rhs);
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map()                                           { }
   map(const map &  rhs) : bst(rhs.bst)            { }
   map(map && rhs)       : bst(std::move(rhs.bst)) { }
   map(const std::initializer_list<Pairs> & il)    { *this = il; }
  ~map()                                           { }

   template <class Iterator>
   map(Iterator first, Iterator last) { insert(first, last); }

   //
   // Assign
   //
   map & operator = (const map & rhs);
   map & operator = (map && rhs);
   map & operator = (const std::initializer_list <Pairs> & il);
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin() { return bst.begin(); }
   iterator end()   { return bst.end(); }

   // 
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K & k) const;
         V & at (const K & k);
   iterator find(const K & k) { return bst.find(k); }

   //
   // Insert
   //
   pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
      auto returnPair = bst.insert(rhs, true /* keep unique */);
      return pair<iterator, bool>(returnPair.first, returnPair.second);
   }

   pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
      auto returnPair = bst.insert(std::move(rhs), true /* keep unique */);
      return pair<iterator, bool>(returnPair.first, returnPair.second);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      for (auto it = first; it != last; it++)
         bst.insert(*it, true /* keep unique */);
   }

   void insert(const std::initializer_list<Pairs> & il)
   {
      for (auto && t : il)
         bst.insert(t, true /* keep unique */);
   }

   //
   // Remove
   //
   void clear() noexcept { bst.clear(); }
   size_t erase(const K & k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty()  const noexcept { return !size(); }
   size_t size() const noexcept { return bst.numElements; }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //
   iterator() { }
   iterator(const typename BST <pair <K, V>>::iterator & rhs) : it(rhs) { }
   iterator(const iterator & rhs) : it(rhs.it) { }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
      it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const { return it == rhs.it; }
   bool operator != (const iterator & rhs) const { return it != rhs.it; }

   // 
   // Access
   //
   const pair <K, V> & operator * () const { return *it; }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
      ++it;
      return *this;
   }

   iterator operator ++ (int postfix)
   {
      auto tmp = *this;
      it++;
      return tmp;
   }

   iterator & operator -- ()
   {
      --it;
      return *this;
   }

   iterator  operator -- (int postfix)
   {
      auto tmp = *this;
      it--;
      return tmp;
   }

private:

   // Member variable
   typename BST < pair <K, V >>  :: iterator it;   
};

 /*****************************************************
  * MAP :: COPY ASSIGNMENT OPERATOR
  * Copy assign all the values from another map to this
  ****************************************************/
template <typename K, typename V>
map<K, V> & map<K, V>::operator = (const map<K, V> & rhs)
{
   bst = rhs.bst;
   return *this;
}

/*****************************************************
 * MAP :: MOVE ASSIGNMENT OPERATOR
 * Move all the values from another map to this
 ****************************************************/
template <typename K, typename V>
map<K, V> & map<K, V>::operator = (map<K, V> && rhs)
{
   bst = std::move(rhs.bst);
   return *this;
}

/*****************************************************
 * MAP :: INITIALIZER LIST ASSIGNMENT OPERATOR
 * Assign all the values from an initializer list to this
 ****************************************************/
template <typename K, typename V>
map<K, V> & map<K, V>::operator = (const std::initializer_list<Pairs> & il)
{
   bst = il;
   return *this;
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 * 
 ****************************************************/
template <typename K, typename V>
V & map<K, V>::operator [] (const K & key)
{
   // iterator
   auto it = iterator(bst.find(key));

   // key not found
   if (it == end())
      // add key
      it = iterator(bst.insert(Pairs(key, V())/* let = set v */).first);

   // value
   return (V &)(*it).second;
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V & map<K, V>::operator [] (const K & key) const
{
   // iterator
   auto it = iterator(bst.find(key));

   // key not found
   if (it == end())
      // add key
      it = iterator(bst.insert(Pairs(key, V())/* let = set v */).first);

   // value
   return (V &)(*it).second;
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V & map <K, V>::at(const K & key)
{
   // iterator
   auto it = iterator(bst.find(key));

   // key not found
   if (it == end())
      // handle missing key error
      throw std::out_of_range("invalid map<K, T> key");

   // value
   return (V &)(*it).second;
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V & map<K, V>::at(const K & key) const
{
   // iterator
   auto it = iterator(bst.find(key));

   // key not found
   if (it == end())
      // handle missing key error
      throw std::out_of_range("invalid map<K, T> key");

   // value
   return (V &)(*it).second;
}

/*****************************************************
 * MAP :: ERASE
 * Find and erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K & k)
{
   auto it = find(k);

   if (it == end())
      return 0;

   erase(it);
   
   return 1;
}

/*****************************************************
 * MAP :: ERASE
 * Erase one element specified by iterator
 * (Note: defined here below because iterator type 
 * needs to have already been defined)
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   return bst.erase(it.it);
}

/*****************************************************
 * MAP :: ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   auto it = first;

   while (it != last)
      it = erase(it);

   return it;
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map<K, V> & lhs, map<K, V> & rhs)
{
   lhs.bst.swap(rhs.bst);
}

}; //  namespace custom

