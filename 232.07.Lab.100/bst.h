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
   
private:
   //
   // Member Variables
   //
   class BNode;
   BNode * root;       // root node of the binary search tree
   size_t numElements; // number of elements currently in the tree
   
   //
   // Helper Methods
   //
   void deleteBinaryTree(BNode * & p);
   BNode * copyBinaryTree(const BNode * pSrc);
   void assignBinaryTree(BNode * & pDest, const BNode * pSrc);

public:
   //
   // Construct
   //
   BST() : numElements(0), root(nullptr) { }
   BST(const BST &  rhs) : BST() { *this = rhs; }
   BST(      BST && rhs) : BST() { *this = std::move(rhs); }
   BST(const std::initializer_list<T>& il) : BST() { *this = il; }
   ~BST() { clear(); }

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
   std::pair<iterator, bool> insert(const T &  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T && t, bool keepUnique = false);

   //
   // Remove
   //
   iterator erase(iterator& it);
   void clear() noexcept;
   void deleteNode(BNode * & pDelete, bool toRight);

   //
   // Status
   //
   bool empty()  const noexcept { return !numElements; }
   size_t size() const noexcept { return numElements; }
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
   
   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   // the node
   BNode * pNode;

public:
   // constructors and assignment
   iterator(BNode * p = nullptr)  : pNode(p) { }
   iterator(const iterator & rhs) : pNode(rhs.pNode) { }

   // operators
   const T & operator * () const { return pNode->data; }

   iterator & operator = (const iterator & rhs)
   {
      this->pNode = rhs.pNode;
      return *this;
   }

   bool operator == (const iterator & rhs) const
   {
      return this->pNode == rhs.pNode;
   }

   bool operator != (const iterator & rhs) const
   {
      return this->pNode != rhs.pNode;
   }

   iterator & operator ++ ();

   iterator operator ++ (int postfix)
   {
      auto tmp = *this;
      ++(*this);
      return tmp;
   }

   iterator & operator -- ();

   iterator operator -- (int postfix)
   {
      auto tmp = *this;
      --(*this);
      return tmp;
   }
};


/*****************************************************************
 ****************************** BST ******************************
 *****************************************************************/

/*****************************************************************
 * Private BST Helper Methods
 *****************************************************************/

/*****************************************************
 * DELETE BINARY TREE (recursive)
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void BST<T>::deleteBinaryTree(BST<T>::BNode * & p)
{
   if (!p)
      return;

   deleteBinaryTree(p->pLeft);
   deleteBinaryTree(p->pRight);

   delete p;
   p = nullptr;
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
typename BST<T>::BNode * BST<T>::copyBinaryTree(const BST<T>::BNode * pSrc)
{
   if (!pSrc)
      return nullptr;
   
   BNode * pDest = new BNode(pSrc->data);

   pDest->pLeft = copyBinaryTree(pSrc->pLeft);
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;

   pDest->pRight = copyBinaryTree(pSrc->pRight);
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;

   return pDest;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void BST<T>::assignBinaryTree(BST<T>::BNode * & pDest, const BST<T>::BNode * pSrc)
{
   if (!pSrc)
   {
      deleteBinaryTree(pDest);
      return;
   }

   if (!pDest)
      pDest = new BNode(pSrc->data);

   else
      pDest->data = pSrc->data;

   assignBinaryTree(pDest->pLeft, pSrc->pLeft);
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;

   assignBinaryTree(pDest->pRight, pSrc->pRight);
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
}

/*****************************************************************
 * Public BST Methods
 *****************************************************************/

/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST<T> & BST<T>::operator = (const BST<T> & rhs)
{
   assignBinaryTree(root, rhs.root);

   assert(this->root == nullptr || this->root->pParent == nullptr);

   numElements = rhs.numElements;

   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST<T> & BST<T>::operator = (BST<T> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST<T> & BST<T>::operator = (const std::initializer_list<T>& il)
{
   for (auto it : il)
   {
      insert(it);
      numElements++;
   }

   return *this;
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST<T>::iterator, bool> BST<T>::insert(const T & t, bool keepUnique)
{
   BNode* pNew = new BNode(t);

   // case 1
   if (empty())
   {
      // If we are the first node we are root and roots are always black
      root = pNew;
      numElements++;
      root->isRed = false;

      // quick exit
      return std::pair<iterator, bool>(iterator(pNew), keepUnique);
   }
   
   // TODO: (delet this) avoiding our problems
   return std::pair<iterator, bool>(iterator(pNew), keepUnique);

   // add the node

   // define
   BNode* pCurrent = root;
   BNode* pLeft = root->pLeft;
   BNode* pRight = root->pRight;
   BNode* pParent = nullptr;

   while (pCurrent)
   {
      // Check if we have children
      
      // Check if we move left or right
      // Moving left
      if (pCurrent->data < pNew->data)
      { 
         // Move right
         pParent = pCurrent;
         pCurrent = pRight;

         if (pCurrent)
         {
            pRight = pCurrent->pRight;
            pLeft = pCurrent->pLeft;
         }

         else
         {
            pRight = nullptr;
            pLeft = nullptr;
         }
      }

      else
      {
         // Moving left
         pParent = pCurrent;
         pCurrent = pLeft;

         if (pCurrent)
         {
            pLeft = pCurrent->pLeft;
            pRight = pCurrent->pRight;
         }

         else
         {
            pLeft = nullptr;
            pRight = nullptr;
         }
      }
   }

   if (pParent->data < pNew->data)
   {
      // Now we can add the node to the tree      
      pParent->addRight(pNew);
   }

   else if (pParent->data == pNew->data)
   {
      // If it already exists then we can return without it
      keepUnique = false;
      return std::pair<iterator, bool>(iterator(pNew), keepUnique);
   }

   else
   {
      // pParent->data > pNew->data
      pParent->addLeft(pNew);
   }

   // re-balance the tree

   // define
   BNode* pGranny = pParent->pParent;

   BNode* pAunt = nullptr;
   if (pGranny)
   {
      if (pGranny->pLeft == pParent)
         pAunt = pGranny->pRight;
      else
         pAunt = pGranny->pLeft;
   }

   BNode* pSibling = nullptr;
   if (pParent)
   {
      if (pParent->pLeft)
         pSibling = pRight;
      else
         pSibling = pLeft;
   }

   // case 3
   if (pParent->isRed && !pGranny->isRed && pAunt->isRed)
   {  
      pParent->isRed = false; // black
      pAunt->isRed = false;   // black
      pGranny->isRed = true;  // red
   }
   
   // case 4a
   if (pParent->isRed && !pGranny->isRed &&
      !pSibling->isRed && !pAunt->isRed &&
      pParent->pLeft == pNew &&
	  pGranny->pLeft == pParent)
   {
      pParent->pParent = pGranny->pParent;
      pGranny->pParent = pParent;

      pParent->addRight(pGranny);
      pGranny->addLeft(pSibling);
      
      pGranny->isRed = true;  // red
      pParent->isRed = false; // black
   }

   // case 4b
   if (pParent->isRed && !pGranny->isRed && 
      !pSibling->isRed && !pAunt->isRed && 
      pParent->pRight == pNew && 
      pGranny->pRight == pParent)
   {
      pParent->pParent = pGranny->pParent;
      pGranny->pParent = pParent;

      pParent->addLeft(pGranny);
      pGranny->addRight(pSibling);
      
      pGranny->isRed = true;  // red
      pParent->isRed = false; // black
   }

   // case 4c
   if (pParent->isRed && !pGranny->isRed &&
	   pParent->pRight == pNew &&
      pGranny->pLeft == pParent)
   {
	   pGranny->pLeft = pNew->pRight;
	   pParent->pRight = pNew->pLeft;

	   if (!pGranny->pParent)
		   pNew->pParent = nullptr;
	   else if (pGranny->pParent->pLeft == pGranny)
		   pGranny->pParent->pLeft = pNew;
	   else
		   pGranny->pParent->pRight = pNew;

	   pNew->pRight = pGranny;
	   pNew->pLeft = pParent;

	   pGranny->isRed = true;  // red
	   pNew->isRed = false;    // black
   }

   // case 4d
   if (pParent->isRed && !pGranny->isRed && 
      !pSibling->isRed && !pAunt->isRed && 
      pParent->pLeft == pNew && 
      pGranny->pRight == pParent)
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
      
      pGranny->isRed = true;  // red
      pNew->isRed = false;    // black
   }

   // return
   return std::pair<iterator, bool>(iterator(pNew), keepUnique);
}

template <typename T>
std::pair<typename BST<T>::iterator, bool> BST<T>::insert(T && t, bool keepUnique)
{
   BNode* pNew = new BNode(t);

   // case 1
   if (empty())
   {
      // If we are the first node we are root and roots are always black
      root = pNew;
      numElements++;
      root->isRed = false;

      // quick exit
      return std::pair<iterator, bool>(iterator(pNew), keepUnique);
   }
   
   // TODO: (delet this) avoiding our problems
   return std::pair<iterator, bool>(iterator(pNew), keepUnique);

   // add the node

   // define
   BNode* pCurrent = root;
   BNode* pLeft = root->pLeft;
   BNode* pRight = root->pRight;
   BNode* pParent = nullptr;

   while (pCurrent)
   {
      // Check if we have children
      
      // Check if we move left or right
      // Moving left
      if (pCurrent->data < pNew->data)
      {
         // Move right
         pParent = pCurrent;
         pCurrent = pRight;

         if (pCurrent)
         {
            pRight = pCurrent->pRight;
            pLeft = pCurrent->pLeft;
         }

         else
         {
            pRight = nullptr;
            pLeft = nullptr;
         }
      }

      else
      {
         // Moving left
         pParent = pCurrent;
         pCurrent = pLeft;

         if (pCurrent)
         {
            pLeft = pCurrent->pLeft;
            pRight = pCurrent->pRight;
         }

         else
         {
            pLeft = nullptr;
            pRight = nullptr;
         }
      }
   }

   if (pParent->data < pNew->data)
   {
      // Now we can add the node to the tree
      pParent->addRight(pNew);
   }

   else if (pParent->data == pNew->data)
   {
      // If it already exists then we can return without it
      keepUnique = false;
      return std::pair<iterator, bool>(iterator(pNew), keepUnique);
   }

   else
   {
      // pParent->data > pNew->data
      pParent->addLeft(pNew);
   }

   // re-balance the tree

   // define
   BNode* pGranny = pParent->pParent;

   BNode* pAunt = nullptr;
   if (pGranny)
   {
      if (pGranny->pLeft == pParent)
         pAunt = pGranny->pRight;
      else
         pAunt = pGranny->pLeft;
   }

   BNode* pSibling = nullptr;
   if (pParent)
   {
      if (pParent->pLeft)
         pSibling = pRight;
      else
         pSibling = pLeft;
   }

   // case 3
   if (pParent->isRed && !pGranny->isRed && pAunt->isRed)
   {
      pParent->isRed = false; // black
      pAunt->isRed = false;   // black
      pGranny->isRed = true;  // red
   }
   
   // case 4a
   if (pParent->isRed && !pGranny->isRed &&
      !pSibling->isRed && !pAunt->isRed &&
      pParent->pLeft == pNew &&
     pGranny->pLeft == pParent)
   {
      pParent->pParent = pGranny->pParent;
      pGranny->pParent = pParent;

      pParent->addRight(pGranny);
      pGranny->addLeft(pSibling);
      
      pGranny->isRed = true;  // red
      pParent->isRed = false; // black
   }

   // case 4b
   if (pParent->isRed && !pGranny->isRed &&
      !pSibling->isRed && !pAunt->isRed &&
      pParent->pRight == pNew &&
      pGranny->pRight == pParent)
   {
      pParent->pParent = pGranny->pParent;
      pGranny->pParent = pParent;

      pParent->addLeft(pGranny);
      pGranny->addRight(pSibling);
      
      pGranny->isRed = true;  // red
      pParent->isRed = false; // black
   }

   // case 4c
   if (pParent->isRed && !pGranny->isRed &&
      pParent->pRight == pNew &&
      pGranny->pLeft == pParent)
   {
      pGranny->pLeft = pNew->pRight;
      pParent->pRight = pNew->pLeft;

      if (!pGranny->pParent)
         pNew->pParent = nullptr;
      else if (pGranny->pParent->pLeft == pGranny)
         pGranny->pParent->pLeft = pNew;
      else
         pGranny->pParent->pRight = pNew;

      pNew->pRight = pGranny;
      pNew->pLeft = pParent;

      pGranny->isRed = true;  // red
      pNew->isRed = false;    // black
   }

   // case 4d
   if (pParent->isRed && !pGranny->isRed &&
      !pSibling->isRed && !pAunt->isRed &&
      pParent->pLeft == pNew &&
      pGranny->pRight == pParent)
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
      
      pGranny->isRed = true;  // red
      pNew->isRed = false;    // black
   }

   // return
   return std::pair<iterator, bool>(iterator(pNew), keepUnique);
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator & it)
{
   if (!it.pNode)
      return end();
   
   deleteNode(it.pNode, true);
   
   return ++it;
   
//   // no children
//   if (!it.pNode->pLeft && !it.pNode->pRight)
//   {
//      if (it.pNode->pParent && it.pNode->pParent->pRight == it.pNode)
//         it.pNode->pParent->pRight = nullptr;
//      if (it.pNode->pParent && it.pNode->pParent->pLeft == it.pNode)
//         it.pNode->pParent->pLeft = nullptr;
//   }
//
//   // one child
//
//   // left child
//   else if (it.pNode->pLeft && !it.pNode->pRight)
//   {
//      it.pNode->pLeft->pParent = it.pNode->pParent;
//      if (it.pNode->pParent && it.pNode->pParent->pRight == it.pNode)
//         it.pNode->pParent->pRight = it.pNode->pLeft;
//      if (it.pNode->pParent && it.pNode->pParent->pLeft == it.pNode)
//          it.pNode->pParent->pRight = it.pNode->pLeft;
//   }
//
//   // right child
//   else if (!it.pNode->pLeft && it.pNode->pRight)
//   {
//      it.pNode->pRight->pParent = it.pNode->pParent;
//      if (it.pNode->pParent && it.pNode->pParent->pRight == it.pNode)
//         it.pNode->pParent->pRight = it.pNode->pRight;
//      if (it.pNode->pParent && it.pNode->pParent->pLeft == it.pNode)
//         it.pNode->pParent->pLeft = it.pNode->pRight;
//   }
//
//   // two children
//   else if (it.pNode->pLeft && it.pNode->pRight)
//   {
//      // define
//      auto pParent = it.pNode;
//      auto pSuccessor = pParent->pRight;
//      while (pSuccessor->pLeft)
//         pSuccessor = pSuccessor->pLeft;
//
//      auto pSuccessorRight = pSuccessor->pRight;
//      auto pSuccessorLeft = pSuccessor->pLeft;
//
//      // replace and delete
//      pSuccessor->pLeft = pParent->pLeft;
//      pSuccessor->pRight = pParent->pRight;
//
//      pParent->pLeft->pParent = pSuccessor;
//      pParent->pRight->pParent = pSuccessor;
//
//      delete pParent;
//
//      // reorder
//      if (true /*D had a right child*/)
//      {
//         // right child takes parent's old place
//
//         // what if D had left child?
//            // set as right child's (now parent) new left child
//            // what if right child had two children?
//               // make it the left-most node of the right child
//      }
//   }
//
//   else
//   {
//      throw "unknown error occoured.";
//   }
//
//   delete it.pNode;
}

/**********************************************
 * DELETE NODE
 * Delete a single node (pDelete) from the tree
 * indicated by a node (pDelete)
 *    pDelete  the node to be deleted
 *    toRight  should the right branch inherit our place?
 *********************************************/
template <typename T>
void BST<T>::deleteNode(BNode * & pDelete, bool toRight)
{
   // shift everything up
   BNode * pNext = (toRight) ? pDelete->pRight : pDelete->pLeft;

   // if we are not the parent, hook ourselves into the existing tree
   if (pDelete != root)
   {
      if (pDelete->pParent->pLeft == pDelete)
      {
         pDelete->pParent->pLeft = nullptr;
         pDelete->pParent->addLeft(pNext);
      }
      else
      {
         pDelete->pParent->pRight = nullptr;
         pDelete->pParent->addRight(pNext);
      }
   }

   // otherwise
   else
   {
      this->root = pNext;
      pNext->pParent = nullptr;
   }
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST<T>::clear() noexcept
{
   deleteBinaryTree(root);
   numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::begin() const noexcept
{
   if (empty()) return end();
   assert(root != nullptr);
   
   auto p = root;
   if (p->pLeft)
   {
      while (p->pLeft)
         p = p->pLeft;
   }

   return iterator(p);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::find(const T & t)
{
   auto p = root;
   
   while (p)
   {
      if (p->data == t) 
         return iterator(p);
      
      else if (t < p->data) 
         p = p->pLeft;
      
      else 
         p = p->pRight;
   }
   
   return end();
}

/*****************************************************************
 **************************** B NODE *****************************
 *****************************************************************/
 
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
   assert(pLeft == nullptr);

   try
   {
      BNode * pNode = new BNode(t);
      addLeft(pNode);
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   assert(pLeft == nullptr);

   try
   {
      BNode * pNode = new BNode(std::move(t));
      addLeft(pNode);
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   assert(pRight == nullptr);

   try
   {
      BNode * pNode = new BNode(t);
      addRight(pNode);
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   assert(pRight == nullptr);

   try
   {
      BNode * pNode = new BNode(std::move(t));
      addRight(pNode);
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
}


/*****************************************************************
 *************************** ITERATOR ****************************
 *****************************************************************/

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST<T>::iterator & BST<T>::iterator::operator ++ ()
{
   // do nothing if we have nothing
   if (!pNode)
      return *this;

   // if there is a right node, take it
   if (pNode->pRight)
   {
      // go right
      pNode = pNode->pRight;

      // jig left - there might be more left-most children
      while (pNode->pLeft)
         pNode = pNode->pLeft;

      return *this;
   }

   // There are no right children, the left are done
   assert (!pNode->pRight);
   const BNode * pTmp = pNode;

   // go up
   pNode = pNode->pParent;
   
   // if the parent is nullptr, we are done.
   if (!pNode)
      return *this;

   // if we are the left child, go to the parent.
   if (pNode->pLeft == pTmp)
      return *this;
   
   // go up as long as we are the right child.
   while (pNode && pNode->pRight == pTmp)
   {
      pTmp = pNode;
      pNode = pNode->pParent;
   }

   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST<T>::iterator & BST<T>::iterator::operator -- ()
{
   // do nothing if we have nothing
   if (!pNode)
      return *this;

   // if there is a left node, take it
   if (pNode->pLeft)
   {
      // go left
      pNode = pNode->pLeft;

      // jig right - there might be more right-most children
      while (pNode->pRight)
         pNode = pNode->pRight;

      return *this;
   }

   // There are no left children, the right are done
   assert (!pNode->pLeft);
   const BNode * pTmp = pNode;

   // go up
   pNode = pNode->pParent;
   
   // if the parent is nullptr, we are done.
   if (!pNode)
      return *this;

   // if we are the right child, go to the parent.
   if (pNode->pRight == pTmp)
      return *this;
   
   // go up as long as we are the left child.
   while (pNode && pNode->pLeft == pTmp)
   {
      pTmp = pNode;
      pNode = pNode->pParent;
   }

   return *this;
}

} // namespace custom
