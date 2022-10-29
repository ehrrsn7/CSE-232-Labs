/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST           : A class that represents a binary search tree
 *        BST::iterator : An iterator through BST
 * Authors
 *    Hunter Powell
 *    Elijah Harrison
 *    Carol Mercau
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

template <class TT>
class set;
template <class KK, class VV>
class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);

public:
   //
   // Construct
   //
   BST() : numElements(0), root(nullptr) { }
   BST(const BST &  rhs) { *this = rhs; }
   BST(      BST && rhs) { *this = std::move(rhs); }
   BST(const std::initializer_list<T>& il) { *this = il; }
   ~BST() { }

   //
   // Assign
   //
   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs)
   {
      std::swap(root, rhs.root);
      std::swap(numElements, rhs.numElements);
   }

   //
   // Iterator
   //
   class iterator;
   iterator begin() const noexcept;
   iterator end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //
   iterator find(const T& t);

   // 
   // Insert
   //
   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   //
   iterator erase(iterator& it);
   void clear() noexcept;

   // 
   // Status
   //
   bool empty()  const noexcept { return !numElements; }
   size_t size() const noexcept { return numElements; }

private:

   class BNode;
   BNode * root;       // root node of the binary search tree
   size_t numElements; // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode()            : data(T())     { pLeft = pRight = pParent = nullptr; }
   BNode(const T & t) : data(t)       { pLeft = pRight = pParent = nullptr; }
   BNode(T && t) : data(std::move(t)) { pLeft = pRight = pParent = nullptr; }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft (      T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return this->pParent->pRight == this; }
   bool isLeftChild( BNode * pNode) const { return this->pParent->pLeft == this; }

   //
   // Data
   //
   T data;         // Actual data stored in the BNode
   BNode* pLeft;   // Left child - smaller
   BNode* pRight;  // Right child - larger
   BNode* pParent; // Parent
   bool isRed;     // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;
   
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;
   
public:
   // constructors and assignment
   iterator(BNode * p = nullptr)  : pNode(p) { }
   iterator(const iterator & rhs) : pNode(rhs.pNode) { }
   
   iterator & operator = (const iterator & rhs)
   {
      this->pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return this->pNode == rhs.pNode;
   }
   
   bool operator != (const iterator & rhs) const
   {
      return this->pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const { return pNode->data; }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix);
   iterator & operator -- ();
   iterator   operator -- (int postfix);

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/

/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   clear();

   for (iterator it = begin(); it != end(); it++)
      insert(*it);

   numElements = rhs.numElements;

   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   clear();

   for (iterator it = begin(); it != end(); it++)
      insert(*it);

   numElements = rhs.numElements;

   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   clear();

   for (auto it : il)
   {
      insert(it);
      numElements++;
   }

   return *this;
}

template <typename T>
void insert(typename BST<T> :: BNode * pThis, typename BST<T> :: BNode * pNew)
{
   // TODO
   /*
   auto pParent = pThis->pParent;
   auto pGranny = pParent->pParent;
   auto pSibling = (pParent->pLeft == pThis) ? pParent->pRight : pParent->pLeft;
   auto pAunt = (pGranny->pLeft == pParent) ? pGranny->pRight : pGranny->pLeft;

   bool condition = pParent->isRed;
   condition &= !pGranny->isRed;
   condition &= !pSibling->isRed;
   condition &= !pAunt->isRed;
   condition &= pParent->pLeft == pNew;
   condition &= pGranny->pRight == pParent;

   if (condition)
   {
      pGranny->pRight = pNew->pLeft;
      pParent->pLeft = pNew->pRight;

      if (!pGranny->pParent)
         pNew->pParent = nullptr;
      else if (pGranny->pParent->pRight == pGranny)
         pGranny->pParent->pRight = pNew;
      else
         pGranny->pParent->pLeft = pNew;

      pNew->pLeft = pGranny;
      pNew->pRight = pParent;

      pGranny->isRed = true;
      pNew->isRed = false;
   }
   */
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   BNode * pNew = new BNode(t);
   // custom::insert(root, pNew);
   std::pair<iterator, bool> pairReturn(iterator(pNew), keepUnique);
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   auto pNew = new BNode(std::move(t));
   // custom::insert(root, pNew);
   std::pair<iterator, bool> pairReturn(iterator(pNew), keepUnique);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{
   auto prev = it;
   return ++it;
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   auto it = begin();
   while (it != end())
      it = erase(it);
   numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if (empty()) return end();
   
   auto p = root;

   while (p->pLeft)
      p = p->pLeft;

   return iterator(p);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   // TODO
   /*auto p = root;
   
   while (!p)
   {
      if (p->data == t) return iterator(p);
      else if (t < p->data) p = p->pLeft;
      else p = p->pRight;
   }*/
   
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   if (pNode)
      pNode->pParent = this;
   this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   if (pNode)
      pNode->pParent = this;
   this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   BNode* pNode = new BNode(t);
   pNode->pParent = this;
   this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   BNode* pNode = new BNode(std::move(t));
   pNode->pParent = this;
   this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   BNode* pNode = new BNode(t);
   pNode->pParent = this;
   this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   BNode* pNode = new BNode(std::move(t));
   pNode->pParent = this;
   this->pRight = pNode;
}



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   if (!this->pNode) return *this;
   iterator & tmp = *this;
   (*this)++;
   return tmp;
}

template <typename T>
typename BST <T> :: iterator BST <T> :: iterator :: operator ++ (int postfix)
{
   // TODO
   if (!this->pNode) return *this;

   auto pCurrent = this->pNode;

   // Have a right child
   if (pCurrent->pRight)
   {
      pCurrent = pCurrent->pRight;
      while (pCurrent->pLeft)
         pCurrent = pCurrent->pLeft;
   }
   
   // no right child and we are our parent's left child
   else if (!pCurrent->pRight && pCurrent->pParent->pLeft == pCurrent)
      pCurrent = pCurrent->pParent;
   
   // No right child and we are our parent's right child
   else if (!pCurrent->pRight && pCurrent->pParent->pRight == pCurrent)
   {
      while (pCurrent->pParent && pCurrent->pParent->pRight == pCurrent)
         pCurrent = pCurrent->pParent;
      pCurrent = pCurrent->pParent;
   }

   this->pNode = pCurrent;
   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   if (!this->pNode) return *this;
   iterator & tmp = *this;
   (*this)--;
   return tmp;
}

template <typename T>
typename BST <T> :: iterator BST <T> :: iterator :: operator -- (int postfix)
{
   // TODO
   if (!this->pNode) return *this;

   auto pCurrent = this->pNode;

   // Have a left child
   if (pCurrent->pLeft)
   {
      pCurrent = pCurrent->pLeft;
      while (pCurrent->pRight)
         pCurrent = pCurrent->pRight;
   }
   
   // no left child and we are our parent's right child
   else if (!pCurrent->pLeft && pCurrent->pParent->pRight == pCurrent)
      pCurrent = pCurrent->pParent;
   
   // No left child and we are our parent's left child
   else if (!pCurrent->pLeft && pCurrent->pParent->pLeft == pCurrent)
   {
      while (pCurrent->pParent && pCurrent->pParent->pLeft == pCurrent)
         pCurrent = pCurrent->pParent;
      pCurrent = pCurrent->pParent;
   }

   this->pNode = pCurrent;
   return *this;
}

} // namespace custom
