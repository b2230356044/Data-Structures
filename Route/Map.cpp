#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

Map::Map() {
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited
    for(int i=0;i<MAX_SIZE;i++){
        for(int j=0;j<MAX_SIZE-1;j++){
            distanceMatrix[i][j]=0;
        }
    }
    for(int i=0;i<MAX_SIZE;i++){
        visited[i]=false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
     ifstream file(filename);
     if(file.is_open()){
         string line;
         int row=0;
            while(getline(file,line)){
                stringstream ss(line);
                string cell;
                int col=0;
                while(getline(ss,cell,',')){
                   distanceMatrix[row][col]=stoi(cell);
                    col++;
                }
                row++;
            }
     }

}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
  if(distanceMatrix[provinceA][provinceB]<=maxDistance){
      return true;
  }else{
    return false; }
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    visited[province]=true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    if(visited[province]==true){
        return true;
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
  for(int i=0;i<MAX_SIZE;i++){
      visited[i]= false;
  }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int count=0;
    for(int i=0;i<MAX_SIZE;i++){
        if(visited[i]==true){
            count++;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    return distanceMatrix[provinceA][provinceB];
}