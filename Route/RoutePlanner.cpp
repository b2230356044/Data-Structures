#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
using namespace std;
// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially

    // Load priority provinces
    // Load restricted provinces
    this->map.loadDistanceData(distance_data);
    map.resetVisited();
    this->loadPriorityProvinces(priority_data);
    this->loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file (filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;}
    if(file.is_open()){
        string line;
        while (getline(file,line)) {
            size_t start = line.find('(');
            size_t end = line.find(')');


            int province = std::stoi(line.substr(start + 1, end - start - 1));
            if (province < 0 || province >= 81) {
                std::cerr << "Error: Province index out of range: " << province + 1 << std::endl;
                continue;
            }


            priorityProvinces[numPriorityProvinces] = province;
            numPriorityProvinces++;
        }
    }
}



// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t start = line.find('(');
            size_t end = line.find(')');
            int province = std::stoi(line.substr(start + 1, end - start - 1));
            if (province < 0 || province >= 81) {
                std::cerr << "Error: Province index out of range: " << province + 1 << std::endl;
                continue;
            }
            weatherRestrictedProvinces[numWeatherRestrictedProvinces] = province;
            numWeatherRestrictedProvinces++;
        }
    }
}


// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int i=0;i<numPriorityProvinces;i++){
        if(province==priorityProvinces[i]){
            return true;
        }

    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int i=0;i<numWeatherRestrictedProvinces;i++){
        if(weatherRestrictedProvinces[i]==province){
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // Mark the starting city as visited
    // Add the starting city to the route
    // Push the starting city to the stack
    // Explore from the starting city
    map.markAsVisited(startingCity);
    route.push_back(startingCity);
    stack.push(startingCity);

    // Start exploration
    enqueueNeighbors(startingCity);

    exploreFromProvince(startingCity);

    while (!isExplorationComplete()) {
        if(!queue.isEmpty()){
        exploreFromProvince(stack.peek());}
       else if (!stack.isEmpty()) {
            backtrack();
        }
    }
    displayResults();


}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {

    while (!queue.isEmpty()&&!isExplorationComplete()) {
        int nextProvince = queue.dequeue();


        if (map.isVisited(nextProvince)) {
            continue;
        }


        map.markAsVisited(nextProvince);
        route.push_back(nextProvince);
        stack.push(nextProvince);
        totalDistanceCovered += map.distanceMatrix[province][nextProvince];


        enqueueNeighbors(nextProvince);
    }


}





void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints

    for (int i = 0; i < MAX_SIZE; i++) {
        if (isWeatherRestricted(i)) {
            std::cout << "Weather restricted province: " << cities[i] << " skipping" << std::endl;
            continue;
        }
        if (i == province || map.isVisited(i)) {
            continue;
        }

        if (map.isWithinRange(province, i, maxDistance) && !map.isVisited(i)) {


            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);

            }
            else {
                queue.enqueue(i);

                }
            }
        }
    }

void RoutePlanner::backtrack() {
    // If you reach a dead-end province
    if (stack.isEmpty()) {
        std::cout << "Stack is empty, no more backtracking possible." << std::endl;
        return;
    }
        stack.pop();
        if (!stack.isEmpty()) {
            int currentTop = stack.peek();
            enqueueNeighbors(currentTop);
        }
    }






bool RoutePlanner::isExplorationComplete() const {
    if(queue.isEmpty()&&stack.isEmpty()){
        return true;
    }
    return false;
}

void RoutePlanner::displayResults() const {
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary
    cout<<"----------------------------"<<endl;
    cout<<"Journey Completed!"<<endl;
    cout<<"----------------------------"<<endl;
    cout<<"Total Number of Provinces Visited: "<<route.size()<<endl;
    cout<<"Total Distance Covered: "<<totalDistanceCovered<<endl;
    cout<<"Route Taken: ";
    for(int i=0;i<route.size();i++){
        cout<<cities[route[i]]<<" -> ";
    }
    cout<<"End"<<endl;
    cout<<"\nPriority Provinces Status:"<<endl;
    int notVisitedPriority=0;
    int visitedPriority=0;
    for(int i=0;i<numPriorityProvinces;i++){
        if(map.isVisited(priorityProvinces[i])){
            cout<<"-"<<cities[priorityProvinces[i]]<<" (Visited)"<<endl;
            visitedPriority++;
        }else{
            cout<<"-"<<cities[priorityProvinces[i]]<<" (Not Visited)"<<endl;
            notVisitedPriority++;
        }

    }
    if(notVisitedPriority==0){
        cout<<"Total Priority Provinces Visited: "<< visitedPriority<<" out of "<<numPriorityProvinces<<endl;
        cout<<" Success: All priority provinces were visited."<<endl;}
    else{
        cout<<"Total Priority Provinces Visited: "<< visitedPriority<<" out of "<<numPriorityProvinces<<endl;
        cout<<"Warning: Not all priority provinces were visited."<<endl;
    }
        }





