#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    this->rear=-1;
    this->front=-1;
    std::fill(this->data, this->data + MAX_QUEUE_SIZE, 0);
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure
    // Add the province
    if(front==(rear+1)%MAX_QUEUE_SIZE){
        return;}
    if(front==-1){
        front++;
        rear++;
    }else{
        rear=(rear+1)%MAX_QUEUE_SIZE;
    }
    data[rear]=province;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if (front == -1) {
        return -1;
    }
    if (front == rear) {
        int temp = data[front];
        front=rear=-1;
        return temp;
    }else {
        int temp = data[front];
        front = (front + 1) % MAX_QUEUE_SIZE;
        return temp;
    }

}

// Returns the front province without removing it
int Queue::peek() const {
    if(front==-1){
        return -1;
    }
    else{
        return data[front];
    }
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    if(front==-1){
        return true;
    }
    return false;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cerr << "Queue overflow! Cannot enqueue priority province: " << province << std::endl;
        return;  // Queue is full
    }

    // Decrement front for priority enqueue
    if (front == -1) {
        // If the queue is empty, initialize front and rear
        front = 0;
        rear = 0;
        data[front] = province;
    } else {
        // Adjust front for circular behavior
        front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
        data[front] = province;
    }
}




