// Stack.h
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: TODO
//
// A last-in first-out stack built on your own singly linked list (Section 4.1).
// Templates must be written entirely in the header file.

#ifndef STACK_H
#define STACK_H

template <typename T>
class Stack
{
private:
    struct Node
    {
        T data;
        Node* next;
    };
    Node* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}

    ~Stack()
    {   
        // TODO: free every node (hint: clear())
        clear();
        
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& item)
    {
        // TODO: add a new node on top
        Node* newNode = new Node{item, topNode};
        topNode = newNode;
        count++;

    }

    bool pop(T& item)
    {
        // TODO: remove the top node, copy its data into item, return true; false if empty
        if(topNode == nullptr){
            return false;
        }
        else{
            Node* temp = topNode;
            item = topNode->data;
            topNode = topNode->next;
            delete temp;
            count--;
            return true;
        }

        
    }

    bool top(T& item) const
    {
        // TODO: copy the top data into item without removing it
        if(topNode == nullptr){
            return false;
        }
        else{
            item = topNode->data;
            return true;
        }
    }

    bool isEmpty() const { return topNode == nullptr; }
    int size() const { return count; }

    void clear()
    {
        // TODO: remove every node
        while(topNode != nullptr){
            Node* oldTop = topNode;
            topNode = topNode->next;
            delete oldTop;
        }
        count = 0;
    }
};

#endif