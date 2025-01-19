#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>
#include <functional>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (auto &shaper : realmShapers)
    {
        delete shaper;
    }
    realmShapers.clear();
}


void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    realmShapers.clear();
    for (auto &shaper : shapers) {
        insert(shaper);
    }
}


int ShaperTree::getSize()
{if(realmShapers.empty()){
        return 0;}

    return static_cast<int>(realmShapers.size());
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;
    if (index >= 0 && index < static_cast<int>(realmShapers.size()))
    {
        isValid = true;
    }

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    int index = findIndex(shaper);
    if (index == -1) {
        return -1;
    }

    int lastIndex = realmShapers.size() - 1;


   for(size_t i=index;i<lastIndex;i++){
      realmShapers[i] = realmShapers[i+1];
       }



    realmShapers.pop_back();
    delete shaper;
    return index;

}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    for (int i = 0; i < realmShapers.size(); ++i) {
        if (realmShapers[i] == shaper) {
            return i;
        }
    }
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{// return depth of the node in the tree if found
    // else
   if(realmShapers.size() == 0){
       return -1;}
    int index = findIndex(shaper);
    if (index == -1) {
        return -1;
    }

    int depth = static_cast<int>(std::floor(std::log2(index + 1)));
    return depth;
}



int ShaperTree::getDepth()
{
    {
        if (realmShapers.empty())
        {
            return -1;
        }


        return static_cast<int>(std::floor(std::log2(realmShapers.size()))+1);
    }
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;
    int challengerIndex = findIndex(challenger);
    if (challengerIndex == -1)
    {
        return *challenger;
    }
    int parentIndex = (challengerIndex - 1) / 2;
    if (parentIndex < 0 || parentIndex >= static_cast<int>(realmShapers.size()))
    {
        std::cerr << "[Duel] No parent to duel with!" << std::endl;
        return *challenger;
    }

    RealmShaper* opponent = realmShapers[parentIndex];
    RealmShaper* victor = nullptr;
    RealmShaper* loser = nullptr;

    if (result)
    {
        victor = challenger;
        loser = opponent;


        std::swap(realmShapers[challengerIndex], realmShapers[parentIndex]);
    }
    else
    {
        victor = opponent;
        loser = challenger;
    }


    victor->gainHonour();
    loser->loseHonour()  ;

    std::cout << "[Duel] " << victor->getName() << " won the duel" << std::endl;
    std::cout << "[Honour] New honour points: "
              << victor->getName() << "-" << victor->getHonour() << " "
              << loser->getName() << "-" << loser->getHonour() << std::endl;


    if (loser->getHonour() <= 0)
    {
        std::cout << "[Duel] " << loser->getName() << " lost all honour, delete" << std::endl;
        remove(loser);
    }

    return *victor;
}





RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper
    {
        int index = findIndex(shaper);
        if (index == -1)
        {
            std::cerr << "[Error] Shaper not found in the tree!" << std::endl;
            return parent;
        }

        if (index == 0)
        {

            return parent;
        }

        int parentIndex = (index - 1) / 2;
        return realmShapers[parentIndex];

}}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int index_low = findIndex(player_low);
    int index_high = findIndex(player_high);

    if (index_low == -1 || index_high == -1) {
        return;
    }

    std::swap(realmShapers[index_low], realmShapers[index_high]);
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found
    for(int i=0;i<realmShapers.size();i++){
        if(realmShapers[i]->getName() == shaper.getName()){
            foundShaper = realmShapers[i];
            break;
        }
    }

    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found
    for(int i=0;i<realmShapers.size();i++){
        if(realmShapers[i]->getName() == name){
            foundShaper = realmShapers[i];
            break;
        }
    }

    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well

    // Helper function for recursive traversal

    if (realmShapers[index] == nullptr) {
        return {};
    }

    if (index >= realmShapers.size()||index<0)
    {
        return {};
    }


    std::vector<std::string> leftTraversal = inOrderTraversal(2 * index + 1);
    result.insert(result.end(), leftTraversal.begin(), leftTraversal.end());

    result.push_back(realmShapers[index]->getName());

    std::vector<std::string> rightTraversal = inOrderTraversal(2 * index + 2);
    result.insert(result.end(), rightTraversal.begin(), rightTraversal.end());
    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector
    // Define and implement as many helper functions as necessary for recursive implementation

    if (realmShapers[index] == nullptr) {
        return {};
    }

    if (index >= realmShapers.size()||index<0)
    {
        return {};
    }

    result.push_back(realmShapers[index]->getName());
    std::vector<std::string> leftTraversal = preOrderTraversal(2 * index + 1);
    result.insert(result.end(), leftTraversal.begin(), leftTraversal.end());

    std::vector<std::string> rightTraversal = preOrderTraversal(2 * index + 2);
    result.insert(result.end(), rightTraversal.begin(), rightTraversal.end());

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector
    // Define and implement as many helper functions as necessary for recursive implementation

    if (realmShapers[index] == nullptr) {
        return {};
    }


    if (index >= realmShapers.size()||index<0)
    {
        return {};
    }
    std::vector<std::string> leftTraversal = inOrderTraversal(2 * index + 1);
    result.insert(result.end(), leftTraversal.begin(), leftTraversal.end());

    std::vector<std::string> rightTraversal = inOrderTraversal(2 * index + 2);
    result.insert(result.end(), rightTraversal.begin(), rightTraversal.end());

    result.push_back(realmShapers[index]->getName());

    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file
    // Define and implement as many helper functions as necessary for recursive implementation


    if (index >= realmShapers.size()||index<0)
    {
        return;
    }


    outFile << realmShapers[index]->getName() << std::endl;


    preOrderTraversal(2 * index + 1, outFile);


    preOrderTraversal(2 * index + 2, outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file
    // Define and implement as many helper functions as necessary
    if (realmShapers.empty())
    {
        return; // If the tree is empty, exit the function
    }

    std::queue<int> q;
    q.push(0);

    while (!q.empty())
    {
        int currentIndex = q.front();
        q.pop();


        outFile << realmShapers[currentIndex]->getName() << std::endl;


        int leftChild = 2 * currentIndex + 1;
        if (leftChild < realmShapers.size())
        {
            q.push(leftChild);
        }


        int rightChild = 2 * currentIndex + 2;
        if (rightChild < realmShapers.size())
        {
            q.push(rightChild);
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2*index+1;  // TODO: Calculate left index
    int right = 2*index+2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
    if(realmShapers.empty()){
        return;}

        std::ofstream outputFile(filename);
        if (!outputFile.is_open())
        {
            std::cerr << "[Error] Failed to open file: " << filename << std::endl;
            return;
        }


        std::queue<int> q;
        q.push(0);

        while (!q.empty())
        {
            int currentIndex = q.front();
            q.pop();


            outputFile << realmShapers[currentIndex]->getName() << std::endl;


            int leftChild = 2 * currentIndex + 1;
            if (leftChild < realmShapers.size())
            {
                q.push(leftChild);
            }


            int rightChild = 2 * currentIndex + 2;
            if (rightChild < realmShapers.size())
            {
                q.push(rightChild);
            }
        }

        outputFile.close();

        std::cout << "[Output] Shapers have been written to " << filename << " according to rankings." << std::endl;
    }



void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;
    if(realmShapers.empty()){
        return;
    }
    std::ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        std::cerr << "[Error] Failed to open file: " << filename << std::endl;
        return;
    }


    std::function<void(int)> preOrder = [&](int index) {
        if (index >= realmShapers.size())
        {
            return;
        }


        outputFile << realmShapers[index]->getName()<< std::endl;


        preOrder(2 * index + 1);


        preOrder(2 * index + 2);
    };


    preOrder(0);

    outputFile.close();


    std::cout << "[Output] Tree has been written to " << filename << " in pre-order." << std::endl;
}
