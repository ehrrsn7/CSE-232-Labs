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
 *    This will contain the class definition of:
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
   friend void swap(list& lhs, list& rhs);
public:  
   // 
   // Construct
   //
   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list() 
   {
   }

   // 
   // Assign
   //
   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);

   //
   // Iterator
   //
   class  iterator;
   iterator begin()  { return iterator(); }
   iterator rbegin() { return iterator(); }
   iterator end()    { return iterator(); }

   //
   // Access
   //
   T& front();
   T& back();

   //
   // Insert
   //
   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   // 
   // Status
   //
   bool empty()  const { return !size(); }
   size_t size() const;

private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;    // pointer to the beginning of the list
   Node * pTail;    // pointer to the ending of the list

   // helper
   void remove(const Node<T> * pRemove);
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   //
   // Construct
   //
   Node()               : pNext(nullptr), pPrev(nullptr) { }
   Node(const T & data) : pNext(nullptr), pPrev(nullptr) { }
   Node(T && data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }
   ~Node() { }

   //
   // Helper Methods for insertion/push_front/back
   //
   void insertAfter(Node <T> * pNext);
   void insertBefore(Node <T> * pPrev);

   //
   // Data
   //
   T data;        // user data
   Node * pNext;  // pointer to next node
   Node * pPrev;  // pointer to previous node
};

void list<T>::Node::insertAfter(Node <T> * pNext)
{
   pNext->pNext = this->pNext;
   pNext->pPrev = this;

   if (this->pNext) 
      this->pNext->pPrev = pNext;

   this->pNext = pNext;
}

void list<T>::Node::insertBefore(Node <T> * pPrev)
{
   pPrev->pNext = this;
   pPrev->pPrev = this->pPrev;
   
   if (this->pPrev) 
      this->pPrev->pNext = pPrev;

   this->pPrev = pPrev;
}

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator()                       : p(new Node<T>)  { }
   iterator(Node * p)               : p(p)            { }
   iterator(const iterator  & rhs)  : p(rhs.p)        { }

   iterator & operator = (const iterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return this->p == rhs.p; }
   bool operator != (const iterator & rhs) const { return this->p != rhs.p; }

   // dereference operator, fetch a node
   T & operator * () { return *p->data; }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      p = p->pNext;
      return *this;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      auto tmp = *this;
      p = p->pNext;
      return tmp;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      p = p->pNext;
      return *this;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it,       T && data);
   friend iterator list <T> :: erase(const iterator & it);

private:

   typename list <T> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) 
{
   numElements = 99;
   pHead = pTail = new list <T> ::Node();
}
// Not sure what to do here. Will come back
// to it later. - Carol

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list<T>::list(Iterator first, Iterator last)
{
   auto it = first;
   auto prev = it;

   if (*it)
      pHead = new Node<T>(it.data);
   it++;

   while (it != last)
   {
      *prev->insertAfter(new Node<T>(it.data));
      numElements++;
      it++;
      prev++;
   }
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il)
{
   numElements = 99;
   pHead = pTail = new list <T> ::Node();
}

/*
list.copy-constructor(rhs) pHead ? pTail ? NULL numElements ? 0
*this ? rhs 
*/

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num)
{
   numElements = 99;
   pHead = pTail = new list <T> ::Node();
}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list() 
{
   numElements = 0;
   pHead = pTail = new list <T> ::Node();
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) 
{
   *this = rhs;
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs)
{
   numElements = 99;
   pHead = pTail = new list <T> ::Node();
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
   // prev <- tmp <- pDestination (this)
   // tmp will be our pDestination iterator
   Node <T>* tmp = this;
   Node <T>* prev = tmp;

   // loop through pSource list
   for (auto p = rhs.pHead; p; p = p->pNext)
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
         auto newNode = new Node<T>(p->data);
         newNode->pPrev = prev;
         if (prev) prev->pNext = newNode;
         
         // if pDestination was empty
         if (!this)
            this = newNode;

         // iterate prev
         prev = newNode;
         tmp = newNode->pNext;
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

   if (!rhs.pHead)
   {
      // any deallocation has been handled above
      this = nullptr;
   }

   numElements = rhs.numElements;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
   // prev <- tmp <- pDestination (this)
   // tmp will be our pDestination iterator
   Node <T>* tmp = this;
   Node <T>* prev = tmp;

   // loop through pSource list
   for (auto p = rhs.pHead; p; p = p->pNext)
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
         auto newNode = new Node<T>(p->data);
         newNode->pPrev = prev;
         if (prev) prev->pNext = newNode;
         
         // if pDestination was empty
         if (!this)
            this = newNode;

         // iterate prev
         prev = newNode;
         tmp = newNode->pNext;
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

   if (!rhs.pHead)
   {
      // any deallocation has been handled above
      this = nullptr;
   }

   numElements = rhs.numElements;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{

   //for (auto it = rhs.begin();it != l.end(); it++)
   //{
	   
   //}
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
void list <T> :: clear()
{
   Node <T> * p = pHead;
   
   while (p)
   {
      auto tmp = p;
      p = p->pNext;
      delete tmp;
   }
   
   pHead = nullptr;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_back(const T & data)
{
   pTail->insertAfter(new Node<T>(data));
   numElements++;
}

template <typename T>
void list <T> ::push_back(T && data)
{
   pTail->insertAfter(new Node<T>(std::move(data)));
   numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{
   pHead->insertBefore(new Node<T>(data));
   numElements++;
}

template <typename T>
void list <T> ::push_front(T && data)
{
   pHead->insertBefore(new Node<T>(std::move(data)));
   numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
   remove(pTail);
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
   remove(pHead);
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: front()
{
   if (empty) return nullptr;
   return *pHead->data;
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
   if (empty) return nullptr;
   return *pTail->data;
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list<T>::iterator list<T>::erase(const list<T>::iterator & it)
{
   return end();
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
typename list<T>::iterator list<T>::insert(
   list<T>:: iterator it,
   const T & data)
{
   auto newNode = new Node<T>(data);
   (*it)->insertBefore(newNode);
   numElements++;
   return list<T>::iterator(newNode);
}

template <typename T>
typename list<T>::iterator list<T>::insert(
   list<T>::iterator it,
   T && data)
{
   auto newNode = new Node<T>(std::move(data));
   (*it)->insertBefore(newNode);
   numElements++;
   return list<T>::iterator(newNode);
}

/******************************************************
 * SIZE
 * Find a given node in an unsorted linked list.
 * Return a pointer to the node if it is found, NULL otherwise.
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : a pointer to the node if it is found
 *  COST    : O(n)
 ********************************************************/
template <class T>
size_t list<T>::size() const
{
   return numElements;
}


/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
void list<T>::remove(const Node <T> * pRemove)
{
   if (!pRemove) return;

   Node <T> * parentNode = pRemove->pPrev;

   if (pRemove->pNext) // attach next to prev
      pRemove->pNext->pPrev = pRemove->pPrev;

   if (pRemove->pPrev) // attach prev to next
      pRemove->pPrev->pNext = pRemove->pNext;

   else // try parentNode -> pNext if pPrev is null
      parentNode = pRemove->pNext;

   delete pRemove;

   numElements--;
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
