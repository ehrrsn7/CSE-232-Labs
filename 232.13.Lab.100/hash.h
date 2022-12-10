/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Carol Mercau
 *    Elijah Harrison
 *    Hunter Powell
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T>
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
public:
   //
   // Construct
   //
   unordered_set() : numElements(0)                   { }
   unordered_set(unordered_set &  rhs)                { *this = rhs;     }
   unordered_set(unordered_set && rhs)                { *this = std::move(rhs); }
   unordered_set(const std::initializer_list<T>& il)  { *this = il; }
   
   template <class Iterator>
   unordered_set(Iterator first, Iterator last) : numElements(0) { insert(first, last); }
   
   unordered_set(const T & t, size_t num)
   {
      reserve(num);
      for (size_t i = 0; i < num; i++)
         insert(t);
      assert(numElements == num);
   }

   //
   // Assign
   //
   unordered_set & operator = (unordered_set & rhs)
   {
      numElements = rhs.numElements;
      for (size_t i = 0; i < 10; i++)
         buckets[i] = rhs.buckets[i];
      return *this;
   }
   
   unordered_set & operator = (unordered_set && rhs)
   {
      // numElements = std::move(rhs.numElements);
      numElements = 0;
      std::swap(numElements, rhs.numElements);
      for (size_t i = 0; i < 10; i++)
         buckets[i] = std::move(rhs.buckets[i]);
      return *this;
   }
   
   unordered_set & operator=(const std::initializer_list<T>& il)
   {
      clear();
      reserve(il.size);
      for (auto & i : il)
         insert(i);
      return *this;
   }
   
   void swap(unordered_set & rhs)
   {
      std::swap(numElements, rhs.numElements);
      for (size_t i = 0; i < 10; i++)
         std::swap(buckets[i], rhs.buckets[i]);
   }

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   
   iterator begin();
   iterator end() { return iterator(&buckets[10], &buckets[10], buckets[0].end()); }

   local_iterator begin(size_t iBucket) { return local_iterator(buckets[iBucket].begin()); }
   local_iterator end(size_t iBucket)   { return local_iterator(buckets[iBucket].end()); }

   //
   // Access
   //
   size_t bucket(const T & t) { return hash(t) % bucket_count(); }   
   size_t hash(  const T & t) { return (size_t)t; }
   iterator find(const T & t);
   
   //
   // Reserve
   //
   void reserve(size_t num) { numElements = num; }

   //
   // Insert
   //
   custom::pair<iterator, bool> insert(const T & t);
   
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      for (auto it = first; it != last; it++)
         insert(*it);
   }
   
   void insert(const std::initializer_list<T> & il)
   {
      for (auto & t : il)
         insert(t);
   }

   // 
   // Remove
   //
   void clear() noexcept
   {
      auto it = begin();
      while (it != end())
      {
         it = erase(*it);
      }
      assert(!numElements);
   }
   
   iterator erase(const T & t);

   //
   // Status
   //
   size_t size() const { return numElements; }
   bool empty() const { return !numElements; }
   size_t bucket_count() const { return 10; }
   size_t bucket_size(size_t i) const { return buckets[i].size(); }

private:

   custom::list<T> buckets [10]; // exactly 10 buckets (empty lists)
   int numElements;              // number of elements in the Hash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   iterator() : pBucket(nullptr), pBucketEnd(nullptr) { }
   iterator(const iterator & rhs) { *this = rhs; }

   iterator(typename custom::list<T> * pBucket,
            typename custom::list<T> * pBucketEnd,
            typename custom::list<T>::iterator itList)
   : pBucket(pBucket), pBucketEnd(pBucketEnd), itList(itList) { }
   
   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
      pBucket = rhs.pBucket;
      pBucketEnd = rhs.pBucketEnd;
      itList = rhs.itList;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const { return itList == rhs.itList; }
   bool operator != (const iterator & rhs) const { return itList != rhs.itList; }
   
   // 
   // Access
   //
   T & operator * () { return *itList; }

   //
   // Arithmetic
   //
   iterator & operator ++ ();
   
   iterator operator ++ (int postfix)
   {
      auto tmp = *this;
      itList++;
      return tmp;
   }

private:
   custom::list<T> *pBucket;
   custom::list<T> *pBucketEnd;
   typename list<T>::iterator itList;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator() : itList() { }
   local_iterator(const typename custom::list<T>::iterator & itList) : itList(itList) { }
   local_iterator(const local_iterator & rhs) { *this = rhs; }

   //
   // Assign
   //
   local_iterator & operator = (const local_iterator & rhs)
   {
      itList = rhs.itList;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator & rhs) const { return itList != rhs.itList; }
   bool operator == (const local_iterator & rhs) const { return itList == rhs.itList; }

   // 
   // Access
   //
   T & operator * () { return *itList; }

   // 
   // Arithmetic
   //
   local_iterator & operator ++ ()
   {
      ++itList;
      return *this;
   }

   local_iterator operator ++ (int postfix)
   {
      auto tmp = *this;
      itList++;
      return tmp;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::erase(const T & t)
{
   // find element to be erased
   auto itErase = find(t);
   if (itErase == end())
      return itErase;
   
   // determine the return value
   auto itReturn = itErase;
   ++itReturn;
   
   // erase the element from the bucket
   itErase.pBucket->erase(itErase.itList);
   numElements--;

   // return iterator to the next element
   return itReturn;
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T>
custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T & t)
{
   typedef custom::pair<custom::unordered_set<T>::iterator, bool> ReturnPair;
   
   // see if the element already exists. if so, then return out
   auto it = find(t);
   if (it != end())
      return ReturnPair(it, false /* did not insert */);
   
   // rehash/reserve more space if we are already at the limit (115 option)
   // if (min_buckets_required(numElements + 1) > bucket_count())
   // {
   //    reserve(numElements * 2);
   //    iBucket = bucket(element);
   // }
   
   // actually insert the new element on the back of the bucket
   auto iBucket = bucket(t);
   buckets[iBucket].push_back(t);
   numElements++;
   
   // return the results
   return ReturnPair(iterator( // iterator pointing to new element
      &buckets[iBucket],       // list<T>* pBucket
      &buckets[10],            // list<T>* pBucketEnd
      buckets[iBucket].find(t) // list<T>::iterator itList
   ), true /* did insert */);
}

template <typename T>
typename custom::unordered_set<T>::iterator custom::unordered_set<T>::begin()
{
   // look for first non-empty bucket
   for (size_t i = 0; i < 10; i++)
   {
      if (!buckets[i].empty())
      {
         // return begin() from the first non-empty bucket
         return iterator(
            &buckets[i],       // list<T>* pBucket
            &buckets[10],       // list<T>* pBucketEnd
            buckets[i].begin() // list<T>::iterator itList
         );
      }
   }
   
   // return end() if all buckets are empty
   return end();
}

/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::find(const T & t)
{
   // get bucket corresponding to t
   auto pBucket = &buckets[bucket(t)];
   
   // find t in bucket
   auto it = pBucket->find(t);
   if (it != pBucket->end())
   {
      auto pBucketEnd = &buckets[bucket_count()];
      return iterator(pBucket, pBucketEnd, it);
   }
   
   // return end() if not found
   return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator & unordered_set<T>::iterator::operator ++ ()
{
   // don't advance if at the end
   if (pBucket == pBucketEnd)
      return *this;
   
   // advance list iterator, return if not end
   ++itList;
   if (itList != pBucket->end())
      return *this;
   
   // if end, start at the next bucket
   ++pBucket;
   while (pBucket != pBucketEnd && pBucket->empty())
      ++pBucket;
   if (pBucket != pBucketEnd)
      itList = pBucket->begin();

   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T>
void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
{
   lhs.swap(rhs);
}

}
