// DroneTree.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: TODO
//
// Write every operation recursively using the private helpers (Section 6).
// The tree does NOT own the units: delete tree nodes only, never node->unit.

#include "DroneTree.h"
#include <iostream>
using namespace std;

// Returns the node with the smallest ID in this subtree (recursive).
// Used to find the in-order successor when removing a node with two children.
// TreeNode is a PRIVATE nested type of DroneTree, so a plain function cannot name
// it. A template lets the compiler deduce the type from the argument instead.

template <typename NodeType>
static NodeType* leftmost(NodeType* node)
{
    if (node->left) {
        return leftmost(node->left);
    }
    return node;
}

DroneTree::DroneTree() : root(nullptr), count(0) {}

DroneTree::~DroneTree() { destroy(root); }

void DroneTree::destroy(TreeNode* node)
{
    // delete every node (post-order); the units are NOT deleted
    if (node) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

bool DroneTree::insert(DeliveryUnit* u)
{
    if (!u) {                       // a null unit would crash on u->getID()
        return false;
    }
    if (insertHelper(root, u)) { count++; return true; }
    return false;
}

bool DroneTree::insertHelper(TreeNode*& node, DeliveryUnit* u)
{
    // smaller IDs left, larger right; false on a duplicate
    if (!node) {
        node = new TreeNode{u, nullptr, nullptr};   // TreeNode has no constructor
        return true;
    }
    if (u->getID() < node->unit->getID()) {
        return insertHelper(node->left, u);
    }
    if (u->getID() > node->unit->getID()) {
        return insertHelper(node->right, u);
    }
    return false;
}

DeliveryUnit* DroneTree::search(int id, int& comparisons) const
{
    comparisons = 0;
    return searchHelper(root, id, comparisons);
}

DeliveryUnit* DroneTree::searchHelper(TreeNode* node, int id, int& comparisons) const
{
    // one comparison per node examined
    if (!node)
    {
        return nullptr;             // fell off the tree: not found
    }
    comparisons++;
    int nodeID = node->unit->getID();
    if (id == nodeID)
    {
        return node->unit;
    }
    if (id < nodeID)
    {
        return searchHelper(node->left, id, comparisons);
    }
    return searchHelper(node->right, id, comparisons);
}

bool DroneTree::remove(int id)
{
    if (removeHelper(root, id))
    {
        count--;
        return true;
    }
    return false;
}

bool DroneTree::removeHelper(TreeNode*& node, int id)
{
    // three cases; two children -> in-order successor
    if (!node)
    {
        return false;               // id is not in the tree
    }

    int nodeID = node->unit->getID();
    if (id < nodeID)
    {
        return removeHelper(node->left, id);
    }
    if (id > nodeID)
    {
        return removeHelper(node->right, id);
    }

    // node holds the ID to remove
    if (!node->left)                // no left child (also covers a leaf)
    {
        TreeNode* old = node;
        node = node->right;         // the parent's pointer now skips this node
        delete old;                 // the node only, never old->unit
        return true;
    }
    if (!node->right)               // only a left child
    {
        TreeNode* old = node;
        node = node->left;
        delete old;
        return true;
    }

    // two children: the in-order successor is the smallest node on the right
    TreeNode* successor = leftmost(node->right);
    int successorID = successor->unit->getID();
    node->unit = successor->unit;   // move the unit pointer up, not the unit itself
    return removeHelper(node->right, successorID);   // delete the successor's node
}

void DroneTree::inOrder() const { inOrderHelper(root); cout << endl; }
void DroneTree::preOrder() const { preOrderHelper(root); cout << endl; }
void DroneTree::postOrder() const { postOrderHelper(root); cout << endl; }

// NOTE: the exact output format is in your handout. Replace the cout line in
// each helper with whatever it asks for (for example a display function on the unit).
void DroneTree::inOrderHelper(TreeNode* node) const
{
    if (!node) 
    {
        return;
    }
    inOrderHelper(node->left);                       // left
    cout << node->unit->getID() << " ";              // visit
    inOrderHelper(node->right);                      // right
}

void DroneTree::preOrderHelper(TreeNode* node) const
{
    if (!node) 
    {
        return;
    }
    cout << node->unit->getID() << " ";              // visit
    preOrderHelper(node->left);                      // left
    preOrderHelper(node->right);                     // right
}

void DroneTree::postOrderHelper(TreeNode* node) const
{
    if (!node) 
    {
        return;
    }
    postOrderHelper(node->left);                     // left
    postOrderHelper(node->right);                    // right
    cout << node->unit->getID() << " ";              // visit
}

int DroneTree::height() const { return heightHelper(root); }

int DroneTree::heightHelper(TreeNode* node) const
{
    // empty tree 0, one node 1
    if (!node)
    {
        return 0;
    }
    int leftHeight = heightHelper(node->left);
    int rightHeight = heightHelper(node->right);
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int DroneTree::size() const { return count; }