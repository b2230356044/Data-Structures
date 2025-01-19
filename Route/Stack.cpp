#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() :top(-1) {}



// Adds a province to the top of the stack
void Stack::push(int province) {
    if (top >= MAX_SIZE - 1) {  // Check if the stack is full
        std::cerr << "Error: Stack overflow! Cannot push " << province << std::endl;
        return;
    }
    top++;
    data[top] = province;
}




// Removes and returns the top province from the stack
int Stack::pop() {

    if(top==-1){
        return -1;}
        else{
            int temp=data[top];
            top--;
        return temp;
        }
    }


// Returns the top province without removing it
int Stack::peek() const {
    if(top==-1){
        return -1;
    }
    else{
        return data[top];
    }
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
   if(top==-1){
         return true;
   }else{
    return false;}
}

// Function to get the current size of the stack
int Stack::getSize() const {
    return top+1;
}
