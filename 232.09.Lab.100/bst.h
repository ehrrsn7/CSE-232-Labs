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
        BNode* root;       // root node of the binary search tree
        size_t numElements; // number of elements currently in the tree

        //
        // Helper Methods
        //
        void deleteBinaryTree(BNode*& p);
        BNode* copyBinaryTree(const BNode* pSrc);
        void assignBinaryTree(BNode*& pDest, const BNode* pSrc);

    public:
        //
        // Construct
        //
        BST() : numElements(0), root(nullptr) { }
        BST(const BST& rhs) : BST() { *this = rhs; }
        BST(BST&& rhs) : BST() { *this = std::move(rhs); }
        BST(const std::initializer_list<T>& il) : BST() { *this = il; }
        ~BST() { clear(); }

        //
        // Assign
        //
        BST& operator = (const BST& rhs);
        BST& operator = (BST&& rhs);
        BST& operator = (const std::initializer_list<T>& il);
        void swap(BST& rhs)
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
        std::pair<iterator, bool> insert(const T& t, bool keepUnique = false);
        std::pair<iterator, bool> insert(T&& t, bool keepUnique = false);

        //
        // Remove
        //
        iterator erase(iterator& it);
        void clear() noexcept;
        void deleteNode(BNode*& pDelete, bool toRight);

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
    class BST <T> ::BNode
    {
    public:
        // 
        // Construct
        //
        BNode() : data(T()) { pLeft = pRight = pParent = nullptr; }
        BNode(const T& t) : data(t) { pLeft = pRight = pParent = nullptr; }
        BNode(T&& t) : data(std::move(t)) { pLeft = pRight = pParent = nullptr; }

        //
        // Insert
        //
        void addLeft(BNode* pNode);
        void addRight(BNode* pNode);
        void addLeft(const T& t);
        void addRight(const T& t);
        void addLeft(T&& t);
        void addRight(T&& t);

        // 
        // Status
        //
        bool isRightChild(BNode* pNode) const { return this->pParent->pRight == this; }
        bool isLeftChild(BNode* pNode) const { return this->pParent->pLeft == this; }

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
    class BST <T> ::iterator
    {
        template <class KK, class VV>
        friend class map;

        template <class TT>
        friend class set;

        friend class ::TestBST; // give unit tests access to the privates
        friend class ::TestMap;
        friend class ::TestSet;

        // must give friend status to remove so it can call getNode() from it
        friend BST <T> ::iterator BST <T> ::erase(iterator& it);

    private:
        // the node
        BNode* pNode;

    public:
        // constructors and assignment
        iterator(BNode* p = nullptr) : pNode(p) { }
        iterator(const iterator& rhs) : pNode(rhs.pNode) { }

        // operators
        const T& operator * () const { return pNode->data; }

        iterator& operator = (const iterator& rhs)
        {
            this->pNode = rhs.pNode;
            return *this;
        }

        bool operator == (const iterator& rhs) const
        {
            return this->pNode == rhs.pNode;
        }

        bool operator != (const iterator& rhs) const
        {
            return this->pNode != rhs.pNode;
        }

        iterator& operator ++ ();

        iterator operator ++ (int postfix)
        {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator& operator -- ();

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
    void BST<T>::deleteBinaryTree(BST<T>::BNode*& p)
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
    typename BST<T>::BNode* BST<T>::copyBinaryTree(const BST<T>::BNode* pSrc)
    {
        if (!pSrc)
            return nullptr;

        BNode* pDest = this;

        try
        {
            // the Son of man hath not where to lay his head
            if (!pDest)
                pDest = new BNode(pSrc->data);

            // otherwise, asign the data over
            else
                pDest->data = pSrc->data;
        }
        catch (const std::exception& e)
        {
            throw "ERROR: Unable to allocate new node";
        }

        assert(pDest);

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
    void BST<T>::assignBinaryTree(BST<T>::BNode*& pDest, const BST<T>::BNode* pSrc)
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
    BST<T>& BST<T>::operator = (const BST<T>& rhs)
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
    BST<T>& BST<T>::operator = (BST<T>&& rhs)
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
    BST<T>& BST<T>::operator = (const std::initializer_list<T>& il)
    {
        // since we never replace il with contents from BST
        clear();

        for (auto&& t : il)
            insert(t);

        return *this;
    }

    /*****************************************************
     * BST :: INSERT
     * Insert a node at a given location in the tree
     ****************************************************/
    template <typename T>
    std::pair<typename BST<T>::iterator, bool> BST<T>::insert(const T& t, bool keepUnique)
    {
        std::pair<iterator, bool> pairReturn(end(), false);

        try
        {
            // if we are at a trivial state (empty tree), create root
            if (!root || empty())
            {
                assert(numElements == 0);

                root = new BNode(t);
                numElements = 1;

                pairReturn.first = iterator(root);  // where it needs to be pointing
                pairReturn.second = true;           // is ""?
                return pairReturn;
            }

            // otherwise
            BNode* pNode = root;

            bool done = false;
            while (!done)
            {
                // if the node is a match, then do nothing
                if (keepUnique && t == pNode->data)
                {
                    pairReturn.first = iterator(pNode);
                    pairReturn.second = false;
                    return pairReturn;
                }

                // if the center node is larger, go left
                if (t < pNode->data)
                {
                    // if there is a node to the left, follow it
                    if (pNode->pLeft)
                        pNode = pNode->pLeft;

                    // if we are at the leaf, then create a new node
                    else
                    {
                        pNode->addLeft(t);
                        done = true; // break
                        pairReturn.first = iterator(pNode->pLeft);
                        pairReturn.second = true;
                    }
                }

                // if the center node is smaller, go right
                else
                {
                    // if there is a node to the right, follow it
                    if (pNode->pRight)
                        pNode = pNode->pRight;

                    // if we are at the leaf, then create a new node
                    else
                    {
                        pNode->addRight(t);
                        done = true; // break
                        pairReturn.first = iterator(pNode->pRight);
                        pairReturn.second = true;
                    }
                }
            }

            // we just inserted something!
            assert(root);
            numElements++;

            // if the root moved out from under us, find it again
            while (root->pParent)
                root = root->pParent;
            assert(!root->pParent);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }

        return pairReturn;
    }

    template <typename T>
    std::pair<typename BST<T>::iterator, bool> BST<T>::insert(T&& t, bool keepUnique)
    {
        std::pair<iterator, bool> pairReturn(end(), false);

        try
        {
            // if we are at a trivial state (empty tree), create root
            if (!root || empty())
            {
                assert(numElements == 0);

                root = new BNode(std::move(t));
                numElements = 1;

                pairReturn.first = iterator(root);  // where it needs to be pointing
                pairReturn.second = true;           // is ""?
                return pairReturn;
            }

            // otherwise
            BNode* pNode = root;

            bool done = false;
            while (!done)
            {
                // if the node is a match, then do nothing
                if (keepUnique && pNode->data == t)
                {
                    pairReturn.first = iterator(pNode);
                    pairReturn.second = false;
                    return pairReturn;
                }

                // if the center node is larger, go left
                if (t < pNode->data)
                {
                    // if there is a node to the left, follow it
                    if (pNode->pLeft)
                        pNode = pNode->pLeft;

                    // if we are at the leaf, then create a new node
                    else
                    {
                        pNode->addLeft(std::move(t));
                        done = true; // break
                        pairReturn.first = iterator(pNode->pLeft);
                        pairReturn.second = true;
                    }
                }

                // if the center node is smaller, go right
                else
                {
                    // if there is a node to the right, follow it
                    if (pNode->pRight)
                        pNode = pNode->pRight;

                    // if we are at the leaf, then create a new node
                    else
                    {
                        pNode->addRight(std::move(t));
                        done = true; // break
                        pairReturn.first = iterator(pNode->pRight);
                        pairReturn.second = true;
                    }
                }
            }

            // we just inserted something!
            assert(root);
            numElements++;

            // if the root moved out from under us, find it again
            while (root->pParent)
                root = root->pParent;
            assert(!root->pParent);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }

        return pairReturn;
    }

    /*************************************************
     * BST :: ERASE
     * Remove a given node as specified by the iterator
     ************************************************/
    template <typename T>
    typename BST<T>::iterator BST<T>::erase(iterator& it)
    {
        // do nothing if there is nothing to do
        if (it == end())
            return end();

        // remember where we were
        iterator itNext(it);
        BNode* pDelete = it.pNode;

        // if there is only one child (right) or no children (how sad!)
        if (!pDelete->pLeft)
        {
            ++itNext;
            deleteNode(pDelete, true /* go right */);
        }

        // there is only one child (left)
        else if (!pDelete->pRight)
        {
            ++itNext;
            deleteNode(pDelete, false /* go 'left' */);
        }

        // otherwise, swap places with the in-order successor
        else
        {
            // find the in-order successor ('I.O.S.')
            BNode* pIOS = pDelete->pRight;
            while (pIOS->pLeft)
                pIOS = pIOS->pLeft;

            // the IOS must not have a right node. Now it will take pDelete's place.
            assert(pIOS->pLeft == nullptr);
            pIOS->pLeft = pDelete->pLeft;
            if (pDelete->pLeft)
                pDelete->pLeft->pParent = pIOS;

            // if the IOS is not a direct right sibling, then put it in the place of pDelete
            if (pDelete->pRight != pIOS)
            {
                // if the IOS has a right sibling, then it takes his place
                if (pIOS->pRight)
                    pIOS->pRight->pParent = pIOS->pParent;
                pIOS->pParent->pLeft = pIOS->pRight;

                // make IOS' right child pDelete's right child
                assert(pDelete->pRight != nullptr);
                pIOS->pRight = pDelete->pRight;
                pDelete->pRight->pParent = pIOS;
            }

            // hook up pIOS' successor
            pIOS->pParent = pDelete->pParent;
            if (pDelete->pParent && pDelete->pParent->pLeft == pDelete)
                pDelete->pParent->pLeft = pIOS;
            if (pDelete->pParent && pDelete->pParent->pRight == pDelete)
                pDelete->pParent->pRight = pIOS;

            // what if that was the root?!?!
            if (root == pDelete)
                root = pIOS;

            itNext = iterator(pIOS);
        }

        numElements--;
        delete pDelete;
        return itNext;
    }

    /**********************************************
     * DELETE NODE
     * Delete a single node (pDelete) from the tree
     * indicated by a node (pDelete)
     *    pDelete  the node to be deleted
     *    toRight  should the right branch inherit our place?
     *********************************************/
    template <typename T>
    void BST<T>::deleteNode(BNode*& pDelete, bool toRight)
    {
        // shift everything up
        BNode* pNext = (toRight) ? pDelete->pRight : pDelete->pLeft;

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
        if (root)
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
        if (!root || empty()) return end();

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
    typename BST<T>::iterator BST<T>::find(const T& t)
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
    void BST <T> ::BNode::addLeft(BNode* pNode)
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
    void BST <T> ::BNode::addRight(BNode* pNode)
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
    void BST<T> ::BNode::addLeft(const T& t)
    {
        assert(pLeft == nullptr);

        try
        {
            BNode* pNode = new BNode(t);
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
    void BST<T> ::BNode::addLeft(T&& t)
    {
        assert(pLeft == nullptr);

        try
        {
            BNode* pNode = new BNode(std::move(t));
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
    void BST <T> ::BNode::addRight(const T& t)
    {
        assert(pRight == nullptr);

        try
        {
            BNode* pNode = new BNode(t);
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
    void BST <T> ::BNode::addRight(T&& t)
    {
        assert(pRight == nullptr);

        try
        {
            BNode* pNode = new BNode(std::move(t));
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
    typename BST<T>::iterator& BST<T>::iterator::operator ++ ()
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
        assert(!pNode->pRight);
        const BNode* pTmp = pNode;

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
    typename BST<T>::iterator& BST<T>::iterator::operator -- ()
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
        assert(!pNode->pLeft);
        const BNode* pTmp = pNode;

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
