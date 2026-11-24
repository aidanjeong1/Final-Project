// Queue.h
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: TODO
//
// A first-in first-out queue built on your own singly linked list (Section 4.1).
// Templates must be written entirely in the header file.

#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
class Queue
{
private:
    struct Node
    {
        T data;
        Node* next;
    };
    Node* frontNode;
    Node* backNode;
    int count;

public:
    Queue() : frontNode(nullptr), backNode(nullptr), count(0) {}

    ~Queue()
    {
        // TODO: free every node (hint: clear())
        clear();
    }

    // A queue owns its nodes, so copying is not allowed
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const T& item)
    {
        // TODO: add a new node at the back
        Node* newNode = new Node{item, nullptr};
        if(isEmpty()){
            frontNode = newNode;
        }
        else{
            backNode->next = newNode;
        }
        backNode = newNode;
        count++; 
    }

    bool dequeue(T& item)
    {
        // TODO: remove the front node, copy its data into item, return true; false if empty
        if(isEmpty()){
            return false;
        }
        Node* oldFront = frontNode;
        item = oldFront->data;
        frontNode = oldFront->next;
        if(frontNode == nullptr){
            backNode = nullptr;
        }
        delete oldFront;
        count--;
        return true;
    }

    bool front(T& item) const
    {
        // TODO: copy the front data into item without removing it
        if(isEmpty()){
            return false;
        }
        item = frontNode->data;
        return true;
    }

    bool isEmpty() const { return frontNode == nullptr; }
    int size() const { return count; }

    void clear()
    {
        // TODO: remove every node
        
        while(frontNode != nullptr){
            Node* oldNode = frontNode;
            frontNode = oldNode->next;
            delete oldNode;
        }
        backNode = nullptr;
        count = 0;
    }
};

#endif