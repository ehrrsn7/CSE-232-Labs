/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Authors
 *    Hunter Powell
 *    Carol Mercau
 *    Elijah Harrison
 ************************************************************************/
 
#pragma once
 
#include <cassert>     // for ASSERT
#include <iostream>    // for NULL
using namespace std;
 
/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
 
template <typename T>
class Node 
{
public:
   // member variables
   T data;
   Node <T> * pPrev;
   Node <T> * pNext;
   
   // constructors
   Node()               : pPrev(nullptr), pNext(nullptr), data(T())  { }
   Node(const T & data) : pPrev(nullptr), pNext(nullptr), data(data) { }
   Node(const Node <T> & n)   { *this = n; }
   Node(Node <T> && n)        { *this = move(n); }
   ~Node()                    { }
   
   void insertAfter(Node <T> * pNext)
   {
      pNext->pNext = this->pNext;
      pNext->pPrev = this;
   
      if (this->pNext) this->pNext->pPrev = pNext;
   
      this->pNext = pNext;
   }
   
   void insertBefore(Node <T> * pPrev)
   {
      pPrev->pNext = this;
      pPrev->pPrev = this->pPrev;
      
      if (this->pPrev) this->pPrev->pNext = pPrev;

      this->pPrev = pPrev;
   }
   
   Node <T> * operator = (const Node <T> & rhs)
   {
      this->data  = rhs.data;
      this->pPrev = rhs.pPrev;
      this->pNext = rhs.pNext;
   }
      
   Node <T> * operator = (Node <T> && rhs)
   {
      this->data  = move(rhs.data);
      this->pPrev = move(rhs.pPrev);
      this->pNext = move(rhs.pNext);
   }
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource)
{
   if (!pSource) return nullptr;

   // get pDestination (pHead)
   Node <T> * pDestination = new Node<T>(pSource->data);
   
   // tmp will be our pDestination iterator
   Node <T> * tmp = pDestination;
   
   // loop through all items in the pSource list
   for (Node <T> * p = pSource->pNext; p; p = p->pNext)
   {
      // allocate new nodes for each of those items
      Node <T> * newNode = new Node <T>(p->data);

      // tie them to each other as they go on
      tmp->pNext = newNode;
      newNode->pPrev = tmp;

      // shift tmp to the next node
      tmp = newNode;
   }
   
   return pDestination; // return pDestination
}
 
/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   // prev <- tmp <- pDestination
   // tmp will be our pDestination iterator
   Node <T> * tmp = pDestination;
   Node <T> * prev = tmp;

   // loop through pSource list
   for (auto p = pSource; p; p = p->pNext)
   {
      // use tmp to check if pDestination list has allocated value which corresponds to pSource value
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
         // if pDestination still points at null,
         // point to the first element of our list we
         // are creating
         if (!pDestination)
         {
            pDestination = newNode;
         }
         
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
   
   if (!pSource)
   {
      // any deallocation has been handled above
      pDestination = nullptr;
   }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 ***********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   std::swap(pLHS, pRHS);
}
 
/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove)
{
   if (!pRemove) return nullptr;

   Node <T> * parentNode = pRemove->pPrev;

   if (pRemove->pNext) // attach next to prev
      pRemove->pNext->pPrev = pRemove->pPrev;

   if (pRemove->pPrev) // attach prev to next
      pRemove->pPrev->pNext = pRemove->pNext;

   else // try parentNode -> pNext if pPrev is null
      parentNode = pRemove->pNext;

   delete pRemove;
   return parentNode;
}
 
/**********************************************
 * INSERT
 * Insert a new node the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                         const T & t,
                         bool after = false)
{
   Node <T> * newNode = new Node<T>(t);

   if (pCurrent)
   {
      if (after) pCurrent->insertAfter(newNode);
      else pCurrent->insertBefore(newNode);
   }

   return newNode;
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
inline size_t size(const Node <T> * pHead)
{
   size_t size = 0;
   for (auto p = (Node <T> *)pHead; p; p = p->pNext)
      size++;
   return size;
}
 
/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   for (auto p = pHead; p; p = p->pNext)
      out << p->data << ' ';
   return out;
}

/*****************************************************
 * CLEAR
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
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
 
