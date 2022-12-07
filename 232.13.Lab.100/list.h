/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class de`ition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Authors
 *    Hunter Powell
 *    Carol Mercau
 *    Elijah Harrison
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList;        // forward declaration for unit tests
class TestHash;

namespace custom
{
/**************************************************
* LIST
* Just like std::list
**************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list & lhs, list & rhs);
public:
   // 
   // Construct
   //
   list();
   list(size_t num, const T & t);
   list(size_t num)                          : list(num, T()) { }
   list(list <T> & rhs)                       : list() { *this = rhs; }
   list(list <T> && rhs)                      : list() { *this = std::move(rhs); }
   list(const std::initializer_list<T> & il)  : list() { *this = il; }
   ~list() { }
   template <class Iterator>
   list(Iterator first, Iterator last);

   // 
   // Assign
   //
   list <T> & operator = (list & rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T> & il);

   //
   // Iterator
   //
   class iterator;
   iterator begin() { return iterator(pHead); }
   iterator rbegin() { return iterator(pTail); }
   iterator end() { return iterator(nullptr); }
   iterator find(const T &);

   //
   // Access
   //
   T & front();
   T & back();

   //
   // Insert
   //
   void push_front(const T & data);
   void push_front(T && data);
   void push_back(const T & data);
   void push_back(T && data);
   iterator insert(iterator it, const T & data, bool after = false);
   iterator insert(iterator it, T && data, bool after = false);

   //
   // Remove
   //
   void pop_front() { erase(pHead); }
   void pop_back() { erase(pTail); }
   void clear();
   iterator erase(const iterator & it);

   // 
   // Status
   //
   bool empty()  const { return !size(); }
   size_t size() const { return numElements; }

private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list

};

/*************************************************
* NODE
* the node class.  Since we do not validate any
* of the setters, there is no point in making them
* private.  This is the case because only the
* List class can make validation decisions
*************************************************/
template <typename T>
class list <T> ::Node
{
public:
   //
   // Construct
   //
   Node()               : pNext(nullptr), pPrev(nullptr) { }
   Node(const T & data) : pNext(nullptr), pPrev(nullptr), data(data) { }
   Node(T && data)      : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }
   ~Node() { }

   //
   // Data
   //
   T data;        // user data
   Node * pNext;  // pointer to next node
   Node * pPrev;  // pointer to previous node

   /*************************************************
   * NODE::INSERT
   * Helper methods for inserting before and after
   ************************************************/
   void insertAfter(Node * newNode)
   {
      if (!this || !newNode) return; // one of the tests has NULL for the this pointer for some reason

      // attach new node to this and this->pNext
      newNode->pNext = pNext;
      newNode->pPrev = this;

      // attach pNext to new node (must be done before next step)
      if (pNext)
         pNext->pPrev = newNode;

      // attach this->pPrev to new node
      pNext = newNode;
   }

   void insertBefore(Node * newNode)
   {
      if (!this || !newNode) return;

      // attach new node to this and this->pPrev
      newNode->pNext = this;
      newNode->pPrev = pPrev;

      // attach pPrev to new node (must be done before next step)
      if (pPrev)
         pPrev->pNext = newNode;

      // attach this->pPrev to new node
      pPrev = newNode;
   }

};

/*************************************************
* LIST ITERATOR
* Iterate through a List, non-constant version
************************************************/
template <typename T>
class list <T> ::iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator() : p(nullptr) { }
   iterator(Node * p) : p(p) { }
   iterator(const iterator & rhs) : p(rhs.p) { }

   iterator & operator = (const iterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return this->p == rhs.p; }
   bool operator != (const iterator & rhs) const { return this->p != rhs.p; }

   // dereference operator, fetch a node
   T & operator * () { return (*p).data; }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      if (p) p = p->pNext;
      return *this;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      iterator & tmp = *this;
      if (p) p = p->pNext;
      return tmp;
   }

   // postfix decrement
   iterator operator -- (int postfix)
   {
      if (p) p = p->pPrev;
      return *this;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      iterator & tmp = *this;
      if (p) p = p->pPrev;
      return tmp;
   }

   // two friends who need to access p directly
   friend iterator list <T> ::insert(iterator it, const T & data, bool after);
   friend iterator list <T> ::insert(iterator it, T && data, bool after);
   friend iterator list <T> ::erase(const iterator & it);

private:

   typename list <T> ::Node * p;
};

/*****************************************
* LIST :: DEFAULT constructors
****************************************/
template <typename T>
list <T> ::list()
{
   numElements = 0;
   pHead = pTail = nullptr;
}

/*****************************************
* LIST :: NON-DEFAULT constructors
* Create a list initialized to a value of size num
****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) : list()
{
   if (!num) return;

   Node * p = pHead = new Node(t);

   for (size_t i = 1; i < num; i++)
   {
      auto newNode = new Node(t);
      p->insertAfter(newNode);
      assert(p->pNext);
      p = p->pNext;
   }

   pTail = p;
   numElements = num;
}

/*****************************************
* LIST :: ITERATOR constructors
* Construct Range specified by iterators
****************************************/
template <typename T>
template <class Iterator>
list<T>::list(Iterator first, Iterator last) : list()
{
   if (first == last) return;

   auto destinationIt = begin();
   Iterator sourceIt = first;

   while (sourceIt != last)
   {
      insert(destinationIt, *sourceIt, true);
      destinationIt++;
      sourceIt++;
   }
}

/**********************************************
* LIST :: assignment operator
* Copy one list onto another
*     INPUT  : a list to be copied
*     OUTPUT :
*     COST   : O(n) with respect to the number of nodes
*********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
   Node * pDestination = this->pHead;
   Node * pSource = rhs.pHead;

   // prev <- tmp <- pDestination (this)
   // tmp will be our pDestination iterator
   Node * tmp = pDestination;
   Node * prev = tmp;

   // loop through pSource list
   for (Node * p = pSource; p; p = p->pNext)
   {
      // check tmp against pDestination list to see if space is already allocated for next pSource value
      if (tmp)
      {
            // copy source
            tmp->data = p->data;

            // iterate prev and tmp
            prev = tmp;
            tmp = tmp->pNext;
      }

      else
      {
         // allocate new node and attach
         Node * newNode = new Node(p->data);
         newNode->pPrev = prev;
         if (prev) prev->pNext = newNode;

         // if pDestination was empty
         if (!pDestination)
            pDestination = newNode;

         // iterate prev
         prev = newNode;
         tmp = newNode->pNext;
         this->pTail = prev;
      }
   }

   // delete remaining nodes from pDestination
   if (tmp)
   {
      // if emptyToStandard
      if (tmp == prev)
            tmp = tmp->pNext;

      // sever ties with the list assigned thus far
      else
            prev->pNext = nullptr;

      // loop through the extraneous nodes and delete them using prev
      while (tmp->pNext)
      {
         delete prev;
         prev = nullptr;
         prev = tmp;
         tmp = tmp->pNext;
         delete prev;
         prev = nullptr;
      }

      // the last one is the only one left over
      delete tmp;
      tmp = nullptr;
   }

   if (!pSource)
   {
      // any deallocation has been handled above
      pDestination = nullptr;
   }

   this->numElements = rhs.numElements;

   rhs.pHead = pSource;
   this->pHead = pDestination;
   this->pTail = prev;
   return *this;
}

/**********************************************
* LIST :: assignment operator - MOVE
* Copy one list onto another
*     INPUT  : a list to be moved
*     OUTPUT :
*     COST   : O(n) with respect to the size of the LHS
*********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> && rhs)
{
   this->clear();
   this->pHead = std::move(rhs.pHead);
   rhs.pHead = nullptr;
   this->pTail = std::move(rhs.pTail);
   rhs.pTail = nullptr;
   this->numElements = std::move(rhs.numElements);
   rhs.numElements = 0;
   return *this;
}

/**********************************************
* LIST :: assignment operator
* Copy one list onto another
*     INPUT  : a list to be copied
*     OUTPUT :
*     COST   : O(n) with respect to the number of nodes
*********************************************/
template <typename T>
list <T> & list <T> :: operator = (const std::initializer_list<T> & il)
{
   if (il.size() == 0) return *this;
   // initialize pHead at a placeholder location
   Node * it = pHead = new Node();

   for (auto element : il)
   {
      it->insertAfter(new Node(element));
      it = it->pNext;
   }
   // shift pHead to pHead->pNext and delete placeholder
   auto tmp = pHead->pNext;
   std::swap(tmp, pHead);
   delete tmp;
   if (pHead) pHead->pPrev = nullptr;
   pTail = it;

   numElements = il.size();

   return *this;
}

/**********************************************
* LIST :: CLEAR
* Remove all the items currently in the linked list
*     INPUT  :
*     OUTPUT :
*     COST   : O(n) with respect to the number of nodes
*********************************************/
template <typename T>
void list <T> ::clear()
{
   auto it = begin();
   while (it != end())
      it = erase(it);

   // declare our deletions to the world
   // https://youtu.be/T_d3teq6pWw?t=26
   pHead = pTail = nullptr;
   numElements = 0;
}

/*********************************************
* LIST :: PUSH BACK
* add an item to the end of the list
*    INPUT  : data to be added to the list
*    OUTPUT :
*    COST   : O(1)
*********************************************/
template <typename T>
void list <T> ::push_back(const T & data)
{
   insert(end(), data, true);
}

template <typename T>
void list <T> ::push_back(T && data)
{
   insert(end(), std::move(data), true);
}

/*********************************************
* LIST :: PUSH FRONT
* add an item to the head of the list
*     INPUT  : data to be added to the list
*     OUTPUT :
*     COST   : O(1)
*********************************************/
template <typename T>
void list <T> ::push_front(const T & data)
{
   insert(begin(), data);
}

template <typename T>
void list <T> ::push_front(T && data)
{
   insert(begin(), std::move(data));
}

/*********************************************
* LIST :: FRONT
* retrieves the first element in the list
*     INPUT  :
*     OUTPUT : data to be displayed
*     COST   : O(1)
*********************************************/
template <typename T>
T & list <T> ::front()
{
   if (empty())
      throw "ERROR: unable to access data from an empty list";

   return pHead->data;

}

/*********************************************
* LIST :: BACK
* retrieves the last element in the list
*     INPUT  :
*     OUTPUT : data to be displayed
*     COST   : O(1)
*********************************************/
template <typename T>
T & list <T> ::back()
{
   if (empty())
      throw "ERROR: unable to access data from an empty list";

   return pTail->data;
}


/******************************************
* LIST :: FIND
* find an item from the list
*     INPUT  : an element
*     OUTPUT : iterator to the item's location
*     COST   : ???
******************************************/
template <typename T>
typename list<T>::iterator list<T>::find(const T & data)
{
   auto it = begin();
   while (it != end())
      if (data == *it++)
         return it;
   return end();
}

/******************************************
* LIST :: ERASE
* remove an item from the middle of the list
*     INPUT  : an iterator to the item being removed
*     OUTPUT : iterator to the new location
*     COST   : O(1)
******************************************/
template <typename T>
typename list<T>::iterator list<T>::erase(const list<T>::iterator & it)
{
   if (!it.p) return end();

   auto itNext = end();

   // Attach next to previous
   if (it.p->pNext)
   {
      it.p->pNext->pPrev = it.p->pPrev;
      itNext = it.p->pNext;
   }
   else
      pTail = pTail->pPrev;

   // attach previous to next
   if (it.p->pPrev)
      it.p->pPrev->pNext = it.p->pNext;
   else
      pHead = pHead->pNext;

   delete it.p;
   numElements--;
   return itNext;
}

/******************************************
* LIST :: INSERT
* add an item to the middle of the list
*     INPUT  : data to be added to the list
*              an iterator to the location where it is to be inserted
*     OUTPUT : iterator to the new item
*     COST   : O(1)
******************************************/
template <typename T>
typename list<T>::iterator list<T>::insert(list<T>::iterator it, const T & data, bool after)
{
   Node * newNode = new Node(data);

   if (it == end())
      pTail->insertAfter(newNode);

   else if (after)
      it.p->insertAfter(newNode);

   else
      it.p->insertBefore(newNode);

   if (!newNode->pPrev)
      pHead = newNode;

   if (!newNode->pNext)
      pTail = newNode;

   numElements++;

   return list<T>::iterator(newNode);
}

template <typename T>
typename list<T>::iterator list<T>::insert(list<T>::iterator it, T && data, bool after)
{
   Node * newNode = new Node(std::move(data));

   if (it == end())
      pTail->insertAfter(newNode);

   else if (after)
      it.p->insertAfter(newNode);

   else
      it.p->insertBefore(newNode);

   if (!newNode->pPrev)
      pHead = newNode;

   if (!newNode->pNext)
      pTail = newNode;

   numElements++;

   return list<T>::iterator(newNode);
}

/**********************************************
* SWAP
* Swap the list from LHS to RHS
*   COST   : O(1)
*********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
   std::swap(&lhs, &rhs);
}

//#endif
}; // namespace custom
