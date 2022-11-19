#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance() const;
    void setBalance(int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;
protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{
}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert(const std::pair<const Key, Value>& new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    // Add helper functions here
    virtual void balanceUpdates(AVLNode<Key, Value>* node, bool bIsNewNodeLeftChild);

    bool needsLLRotation(AVLNode<Key, Value>* node);
    bool needsRRRotation(AVLNode<Key, Value>* node);
    bool needsRLRotation(AVLNode<Key, Value>* node);
    bool needsLRRotation(AVLNode<Key, Value>* node);
    bool needsLLRotationRemove(AVLNode<Key, Value>* node);
    bool needsRRRotationRemove(AVLNode<Key, Value>* node);

    bool internalInsert(AVLNode<Key, Value>* rt, const std::pair<const Key, Value>& keyValuePair);
    void calculateSetNewBalance(AVLNode<Key, Value>* node);
    void leftRotation(AVLNode<Key, Value>* node);
    void rightRotation(AVLNode<Key, Value>* node);
};

template<class Key, class Value>
bool AVLTree<Key, Value>::internalInsert(AVLNode<Key, Value>* rt, const std::pair<const Key, Value>& keyValuePair)
{
    if (rt->getKey() == keyValuePair.first) {
        // update value
        rt->setValue(keyValuePair.second);
        return false;
    }
    else if (keyValuePair.first < rt->getKey()) {
        if (rt->getLeft() == nullptr) {
            rt->setLeft(new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, rt));
            return true;
        }
        else {
            internalInsert(rt->getLeft(), keyValuePair);
        }
    }
    else if (keyValuePair.first > rt->getKey()) {
        if (rt->getRight() == nullptr) {
            rt->setRight(new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, rt));
            return true;
        }
        else {
            internalInsert(rt->getRight(), keyValuePair);
        }
    }
    return true;
}


template<class Key, class Value>
bool AVLTree<Key, Value>::needsLLRotation(AVLNode<Key, Value>* node)
{
    if (node != nullptr && node->getLeft() != nullptr) {
        if (node->getBalance() == -2 && node->getLeft()->getBalance() == -1)
        {
            return true;
        }
    }
    return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::needsLLRotationRemove(AVLNode<Key, Value>* node)
{
    if (node != nullptr && node->getLeft() != nullptr) {
        if (node->getBalance() == -2 && (node->getLeft()->getBalance() == -1 || node->getLeft()->getBalance() == 0))
        {
            return true;
        }
    }
    return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::needsRRRotationRemove(AVLNode<Key, Value>* node)
{
    if (node != nullptr && node->getRight() != nullptr) {
        if (node->getBalance() == 2 && (node->getRight()->getBalance() == 1 || node->getRight()->getBalance() == 0))
        {
            return true;
        }
    }
    return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::needsRRRotation(AVLNode<Key, Value>* node)
{
    if (node != nullptr && node->getRight() != nullptr) {
        if (node->getBalance() == 2 && node->getRight()->getBalance() == 1)
        {
            return true;
        }
    }
    return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::needsRLRotation(AVLNode<Key, Value>* node)
{
    if (node != nullptr && node->getRight() != nullptr) {
        if (node->getBalance() == 2 && node->getRight()->getBalance() == -1)
        {
            return true;
        }
    }
    return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::needsLRRotation(AVLNode<Key, Value>* node)
{
    if (node != nullptr && node->getLeft() != nullptr) {
        if (node->getBalance() == -2 && node->getLeft()->getBalance() == 1)
        {
            return true;
        }
    }
    return false;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item)
{
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    }
    else {
        bool bCreated = internalInsert((AVLNode<Key, Value>*)(this->root_), new_item);

        if (!bCreated)
            return;
        AVLNode<Key, Value>* newNode = (AVLNode<Key, Value>*)BinarySearchTree<Key, Value>::internalFind(new_item.first);

        bool bIsNewNodeLeftChild = false;
        if (newNode == newNode->getParent()->getLeft()) {
            bIsNewNodeLeftChild = true;
        }
        std::cout << "new key : " << newNode->getKey() << std::endl;

        // only update balance if parent's # of children 
        // went from 0 -> 1
        bool bUpdateBalance = false;
        if (bIsNewNodeLeftChild) {
            if (newNode->getParent()->getRight() == nullptr) {
                bUpdateBalance = true;
            }
        }
        else {
            if (newNode->getParent()->getLeft() == nullptr) {
                bUpdateBalance = true;
            }
        }
        if (bUpdateBalance)
            balanceUpdates(newNode->getParent(), bIsNewNodeLeftChild);
        std::cout << "after all updates: " << std::endl;
        this->print();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::calculateSetNewBalance(AVLNode<Key, Value>* node)
{
    if (node == nullptr) {
      return;
    }
    int right = BinarySearchTree<Key, Value>::getHeight(node->getRight());
    int left = BinarySearchTree<Key, Value>::getHeight(node->getLeft());
    node->setBalance(right - left);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotation(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* newTop = node->getLeft();
    AVLNode<Key, Value>* newBottom = node;
    // store newTop's old right child
    AVLNode<Key, Value>* oldRightOfTop = newTop->getRight();
    // change parent references
    AVLNode<Key, Value>* parent = node->getParent();
    if (parent != nullptr) {
      if (node == parent->getLeft()) {
          parent->setLeft(newTop);
      }
      else {
          parent->setRight(newTop);
      }
    }
    else {
      this->root_ = newTop;
    }
    newTop->setParent(parent);
    newBottom->setParent(newTop);

    // set newTop's right to newBottom (left remains same)
    newTop->setRight(newBottom);
    
    // set newBottom's left to oldRightOfTop (right remains same)
    newBottom->setLeft(oldRightOfTop);

    calculateSetNewBalance(newTop);
    calculateSetNewBalance(newBottom);
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotation(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* newTop = node->getRight();
    AVLNode<Key, Value>* newBottom = node;

    // store newTop's old left child
    AVLNode<Key, Value>* oldLeftOfTop = newTop->getLeft();

    // change parent references
    AVLNode<Key, Value>* parent = node->getParent();
    if (parent != nullptr) {
      if (node == parent->getLeft()) {
          parent->setLeft(newTop);
      }
      else {
          parent->setRight(newTop);
      }
    }
    else {
      this->root_ = newTop;
    }
    newTop->setParent(parent);
    newBottom->setParent(newTop);

    // set newTop's left to newBottom (right remains same)
    newTop->setLeft(newBottom);

    // set newBottom's right to oldLeftOfTop (left remains same)
    newBottom->setRight(oldLeftOfTop);

    calculateSetNewBalance(newTop);
    calculateSetNewBalance(newBottom);
}
template<class Key, class Value>
void AVLTree<Key, Value>::balanceUpdates(AVLNode<Key, Value>* node, bool bIsNewNodeLeftChild)
{
    std::cout << "updating balance for node: " << node->getKey() << std::endl;
    // update balance
    if (bIsNewNodeLeftChild) {
        //node->updateBalance(-1);
        calculateSetNewBalance(node);
    }
    else {
        //node->updateBalance(1);
        calculateSetNewBalance(node);
    }

    std::cout << "new balance: " << std::to_string(node->getBalance()) << std::endl;
    bool bHasParent = true;
    if (node->getParent() == nullptr) {
        bHasParent = false;
    }
    AVLNode<Key, Value>* parent = nullptr;
    if (bHasParent) {
        parent = node->getParent();
    }
    // check if node is left or right child of parent
    bool bIsLeft = false;
    if (parent != nullptr && parent->getLeft() == node) {
        bIsLeft = true;
    }
    // if node is unbalanced:
    if (unbalanced(node)) {
        if (needsLLRotation(node)) {
            std::cout << "LL at " << node->getKey() << std::endl;
            rightRotation(node);
            //return;
        }
        else if (needsRRRotation(node)) {
            std::cout << "RR at " << node->getKey() << std::endl;
            leftRotation(node);
            //return;
        }
        else if (needsRLRotation(node)) {
            std::cout << "RL at " << node->getKey() << std::endl;
            rightRotation(node->getRight());
            leftRotation(node);
            //return;
        }
        else if (needsLRRotation(node)) {
            std::cout << "LR at " << node->getKey() << std::endl;
            leftRotation(node->getLeft());
            rightRotation(node);
            //return;
        }
    }

    if (!bHasParent) {
        return;
    }

    // if height of left != height of right, continue balance
   // if (BinarySearchTree<Key, Value>::getHeight(node->getLeft()) != BinarySearchTree<Key, Value>::getHeight(node->getRight()))
    balanceUpdates(parent, bIsLeft);
}

template<class Key, class Value>
bool unbalanced(AVLNode<Key, Value>* node)
{
    int8_t two = 2;
    int8_t negtwo = -2;
    if (node->getBalance() == two || node->getBalance() == negtwo) {
        return true;
    }
    return false;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  // TODO
  AVLNode<Key, Value>* node = (AVLNode<Key, Value>*)BinarySearchTree<Key, Value>::internalFind(key);
  if (node != nullptr) {
    AVLNode<Key, Value>* parent = node->getParent();
    
    bool bDeleteFromLeft = false;
    if (parent != nullptr) {
      if (parent->getLeft() == node) {
        bDeleteFromLeft = true;
      }
    }
    BinarySearchTree<Key, Value>::remove(key);
    calculateSetNewBalance(parent);

    if (parent != nullptr) {
      if (!bDeleteFromLeft) {
        if (needsLLRotationRemove(parent)) {
          std::cout << "LL at " << parent->getKey() << std::endl;
          rightRotation(parent);
        }
        else if (needsLRRotation(parent)) {
          std::cout << "LR at " << parent->getKey() << std::endl;
          leftRotation(parent->getLeft());
          rightRotation(parent);
        }
      }
      else {
        if (needsRRRotationRemove(parent)) {
          std::cout << "RR at " << parent->getKey() << std::endl;
          leftRotation(parent);
        }
        else if (needsRLRotation(parent)) {
          std::cout << "RL at " << parent->getKey() << std::endl;
          rightRotation(node->getRight());
          leftRotation(node);
        }
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}



#endif
