#include "Map.h"
#include <stack>
#include <queue>
Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    deleteTree(root);
    root = nullptr;
}

void Map::deleteTree(MapNode* node)
{
    if (node == nullptr) {
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}


void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items
    for (Isle* isle : isles)
    {
        insert(isle);
    }


    int counter = 0;


    postOrderItemDrop(root, counter);

    int counter2 = 0;


    preOrderItemDrop(root, counter2);
    initialize = true;


}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    if (!current || !current->left)
    {
        std::cerr << "[Right Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    if (!current || !current->right)
    {
        std::cerr << "[Left Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth){
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    if (node == nullptr){
        return 0;}

    return node->height;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    // TODO: Recursively insert isle to the tree
    // returns inserted node
    rotation = false;
    if (node == nullptr)
        return new MapNode(isle);

    if (*isle < *(node->isle))
        node->left = insert(node->left, isle);
    else if (*isle > *(node->isle))
        node->right = insert(node->right, isle);
    else
        return node;

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = height(node->left) - height(node->right);
  if (balance < -1 || balance > 1) {
        rebalanceCount++;
        if (rebalanceCount % 3 == 0 && initialize) {
            populateWithItems();
            dropItemBFS();
        }
    }

    if (balance > 1 && *isle < *(node->left->isle)) {
        rotation = true;
        return rotateRight(node);
    }

    if (balance < -1 && *isle > *(node->right->isle)) {
        rotation = true;
        return rotateLeft(node);
    }

    if (balance > 1 && *isle > *(node->left->isle))
    {
        rotation = true;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && *isle < *(node->right->isle))
    {
        rotation = true;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);

    // you might need to insert some checks / functions here depending on your implementation
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;
    rotation = false;
    if (!node)
    {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return nullptr;
    }

    if (isle->getName() < node->isle->getName())
    {
        node->left = remove(node->left, isle);
    }
    else if (isle->getName() > node->isle->getName())
    {
        node->right = remove(node->right, isle);
    }
    else
    {
        if (!node->left || !node->right)
        {
            MapNode *temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }
        else
        {

            MapNode *predecessor = node->left;
            while (predecessor->right)
            {
                predecessor = predecessor->right;
            }

            node->isle = new Isle(* predecessor->isle);


            node->left = remove(node->left, predecessor->isle);
            predecessor->isle = nullptr;
        }
    }




    node->height = std::max(height(node->left), height(node->right)) + 1;
    int balance = height(node->left) - height(node->right);


    if (balance > 1 && height(node->left->left) >= height(node->left->right)) {
        rotation = true;
        return rotateRight(node);
    }


    if (balance > 1 && height(node->left->left) < height(node->left->right))
    {
        rotation = true;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && height(node->right->right) >= height(node->right->left)) {
        rotation = true;
        return rotateLeft(node);
    }

    if (balance < -1 && height(node->right->right) < height(node->right->left))
    {
        rotation = true;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    if (rotation) {
        rebalanceCount++;
        rotation = false;
        if (rebalanceCount % 3 == 0 && initialize) {
            populateWithItems();
            dropItemBFS();
        }
    }


    return node;
}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (!current)
        return;

    count++;

    if (count % 5 == 0)
    {
        current->isle->setItem(Item::EINSTEINIUM);
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}


void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (!current)
        return;

    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);


    count++;

    if (count % 3 == 0)
    {
        current->isle->setItem(Item::GOLDIUM);
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item

    if (node->isle->getItem()==Item::EMPTY)
        return node;
    MapNode *leftResult;

    if (node->left) {
       leftResult = findFirstEmptyIsle(node->left);
    }

    if (leftResult)
        return leftResult;

    return findFirstEmptyIsle(node->right);

}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;
    if (!root)
    {
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    }

    std::queue<MapNode *> bfsQueue;
    bfsQueue.push(root);

    while (!bfsQueue.empty())
    {
        MapNode *current = bfsQueue.front();
        bfsQueue.pop();
        if (findFirstEmptyIsle(current))
        {
            current->isle->setItem(Item::AMAZONITE);
            std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << current->isle->getName() << std::endl;
            return;
        }

        if (current->left)
            bfsQueue.push(current->left);
        if (current->right)
            bfsQueue.push(current->right);
    }
    std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;

}


void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    if (node == nullptr)
    {
        return -1;
    }

    MapNode *current = root;
    int depth = 0;

    while (current != nullptr)
    {
        if (current == node)
        {
            return depth;
        }

        if (node->isle < current->isle)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        depth++;
    }

    return -1;
}
// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    if (!root)
        return -1;

    int depth = 0;
    std::queue<MapNode *> queue;
    queue.push(root);

    while (!queue.empty())
    {
        int levelSize = queue.size();

        for (int i = 0; i < levelSize; i++)
        {
            MapNode *current = queue.front();
            queue.pop();

            if (current->isle == isle)
                return depth;
            if (current->left)
                queue.push(current->left);
            if (current->right)
                queue.push(current->right);
        }
        depth++;
    }

    return -1;
}

int Map::getDepth() {

    return calculateDepth(root);
}

int Map::calculateDepth(MapNode *node) {
    if (!node) {
        return 0;
    }
    int leftDepth = calculateDepth(node->left);
    int rightDepth = calculateDepth(node->right);
    return std::max(leftDepth, rightDepth) + 1;
}



void Map::populateWithItems()
{
    if (!root)
    {
        return;
    }
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM
    int counter = 0;
    // Drop EINSTEINIUM using the pre-order item drop function
    // Drop GOLDIUM using the post-order item drop function
    postOrderItemDrop(root, counter);
    int counter2=0;
    preOrderItemDrop(root, counter2);
}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    {
        if (!root)
            return nullptr;

        std::queue<MapNode *> bfsQueue;
        bfsQueue.push(root);

        while (!bfsQueue.empty())
        {
            MapNode *current = bfsQueue.front();
            bfsQueue.pop();


            if (*current->isle == isle)
                return current->isle;

            if (current->left)
                bfsQueue.push(current->left);
            if (current->right)
                bfsQueue.push(current->right);
        }

        return nullptr;
}}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    {
        if (!root)
            return nullptr;

        std::queue<MapNode *> bfsQueue;
        bfsQueue.push(root);

        while (!bfsQueue.empty()) {
            MapNode *current = bfsQueue.front();
            bfsQueue.pop();


            if (current->isle->getName() == name)
                return current->isle;


            if (current->left)
                bfsQueue.push(current->left);
            if (current->right)
                bfsQueue.push(current->right);
        }

        return nullptr;
    }
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    if (!root)
        return nullptr;

    std::queue<MapNode *> bfsQueue;
    bfsQueue.push(root);

    while (!bfsQueue.empty())
    {
        MapNode *current = bfsQueue.front();
        bfsQueue.pop();


        if (*current->isle == isle)
            return current;


        if (current->left)
            bfsQueue.push(current->left);
        if (current->right)
            bfsQueue.push(current->right);
    }

    return nullptr;}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    if (!root)
        return nullptr;

    std::queue<MapNode *> bfsQueue;
    bfsQueue.push(root);

    while (!bfsQueue.empty())
    {
        MapNode *current = bfsQueue.front();
        bfsQueue.pop();


        if (current->isle->getName() == name)
            return current;


        if (current->left)
            bfsQueue.push(current->left);
        if (current->right)
            bfsQueue.push(current->right);
}
    return nullptr;}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename) {
    // TODO: Write the tree to filename output level by level
    if (!root) {
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::queue<MapNode *> bfsQueue;
    bfsQueue.push(root);

    while (!bfsQueue.empty()) {
        int levelSize = bfsQueue.size();
        bool isFirstInLevel = true;

        for (int i = 0; i < levelSize; ++i) {
            MapNode *current = bfsQueue.front();
            bfsQueue.pop();


            if (!isFirstInLevel) {
                outFile << " ";
            }
            isFirstInLevel = false;


            if (current && current->isle) {
                outFile << current->isle->getName();
            } else {
                outFile << "NULL";
            }


            if (current->left) {
                bfsQueue.push(current->left);
            }
            if (current->right) {
                bfsQueue.push(current->right);
            }
        }


        outFile << "\n";
    }

    outFile.close();
}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
    if (!root)
    {
        std::cout << "[Output] Tree is empty. No isles to write." << std::endl;
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }


    std::stack<MapNode *> nodeStack;
    MapNode *current = root;

    while (!nodeStack.empty() || current != nullptr)
    {

        while (current != nullptr)
        {
            nodeStack.push(current);
            current = current->left;
        }


        current = nodeStack.top();
        nodeStack.pop();
        if (current->isle) {
            outFile << current->isle->getName() << std::endl;
        }
        current = current->right;
    }

    outFile.close();
}