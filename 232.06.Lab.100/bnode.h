/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Hunter Powell
 *    Elijah Harrison
 *    Carol Mercau
 ************************************************************************/

 // cursor parking lot
 // /----------------------/
 // |                      |
 // |                      |
 // |                      |
 // /----------------------/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   BNode()
   {
      data = T();
      pLeft = pRight = pParent = nullptr;
   }
   BNode(const T & t)
   {
      data = t;
      pLeft = pRight = pParent = nullptr;
   }
   BNode(T && t)
   {
      data = std::move(t);
      pLeft = pRight = pParent = nullptr;
   }

   //
   // Data
   //
   BNode <T> * pLeft;
   BNode <T> * pRight;
   BNode <T> * pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T> * p)
{
   if (p == NULL)
      return 0;
   size_t sizeL = size(p->pLeft);
   size_t sizeR = size(p->pRight);
   return sizeL + 1 + sizeR;
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pAdd)
      pAdd->pParent = pNode;
   pNode->pLeft = pAdd;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight(BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pAdd)
      pAdd->pParent = pNode;
   pNode->pRight = pAdd;
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, const T & t)
{
   BNode<T> * pAdd = new BNode<T>(t);
   pAdd->pParent = pNode;
   pNode->pLeft = pAdd;
}

template <class T>
inline void addLeft(BNode <T> * pNode, T && t)
{
   BNode<T> * pAdd = new BNode<T>(std::move(t));
   pAdd->pParent = pNode;
   pNode->pLeft = pAdd;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight(BNode <T> * pNode, const T & t)
{
   BNode<T> * pAdd = new BNode<T>(t);
   pAdd->pParent = pNode;
   pNode->pRight = pAdd;
}

template <class T>
void addRight(BNode <T> * pNode, T && t)
{
   BNode<T> * pAdd = new BNode<T>(std::move(t));
   pAdd->pParent = pNode;
   pNode->pRight = pAdd;
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/

/*
CLEAR(bnode)
   if bnode == NULL: return
   clear(bnode->left)
   clear(bnode->right)
   delete bnode
*/

template <class T>
void clear(BNode <T> * & pThis)
{
   if (!pThis)
      return;
   clear(pThis->pLeft);
   clear(pThis->pRight);
   delete pThis;
   pThis = nullptr;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T> * & pLHS, BNode <T> * & pRHS)
{
   auto tmp = pLHS;
   pLHS = pRHS;
   pRHS = tmp;
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc)
{
   if (!pSrc)
      return nullptr;
   
   BNode<T> * pDest = new BNode<T>(pSrc->data);

   pDest->pLeft = copy(pSrc->pLeft);
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;

   pDest->pRight = copy(pSrc->pRight);
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
void assign(BNode <T> * & pDest, const BNode <T> * pSrc)
{
   if (!pSrc)
   {
      clear(pDest);
      return;
   }

   if (!pDest)
      pDest = new BNode<T>(pSrc->data);

   else
      pDest->data = pSrc->data;

   assign(pDest->pLeft, pSrc->pLeft);
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;

   assign(pDest->pRight, pSrc->pRight);
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
}
