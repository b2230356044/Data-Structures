#include "GameWorld.h"
#include "Map.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;

    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
    int realmShaperDepth = shaperTree.getDepth(realmShaper);
    if (realmShaperDepth == -1)
    {
        std::cout << "[Access Control] RealmShaper not found!" << std::endl;
        return false;
    }
    if (realmShaperDepth == 0)
    {
        return true;
    }
    int isleMinDepthAccess = mapTree.calculateMinMapDepthAccess(realmShaperDepth,shaperTree.getDepth(), mapTree.getDepth());
    if (mapTree.getIsleDepth(isle) >= isleMinDepthAccess)
    {
        hasAccess = true;
    }

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle) {
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
    // Check if the realmShaper has access
    if (!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area "
                  << isle->getName() << std::endl;
        return;
    }

    if (isle->getItem())
    {
        realmShaper->collectItem(isle->getItem());
        isle->setItem(EMPTY);
    }
    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;


    std::string currentIsleName = realmShaper->getIsleName();
    if (!currentIsleName.empty()) {
        Isle *currentIsle = mapTree.findIsle(currentIsleName);
        if (currentIsle) {
            currentIsle->decreaseShaperCount();
        }
    }
    realmShaper->setIsleName(isle->getName());
    realmShaper->loseEnergy();


    isle->increaseShaperCount();
    realmShaper->setIsleName(isle->getName());

    if (isle->getShaperCount() > isle->getCapacity()) {

        std::cout << "[Overcrowding] " << isle->getName()
                  << " self-destructed, it will be removed from the map" << std::endl;

        mapTree.remove(isle);


    }

}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    Isle isle(isleName);
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;
    if(shaper->hasEnoughEnergy()){

        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        mapTree.insert(&isle);
        isle.increaseShaperCount();
        shaper->loseEnergy();
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    } else {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }
}

void GameWorld::displayGameState()
{
    //mapTree.displayMap();
    //shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other
    // This function should call exploreArea and craft functions
    // Use displayGameState();
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);
    if (!accessFile.is_open() || !duelFile.is_open()) {
        return;
    }

    std::string line;
    int accessCount = 0;


    std::getline(accessFile, line);
    std::getline(duelFile, line);


    while (std::getline(accessFile, line)) {
        std::istringstream iss(line);
        std::string shaperName, isleName;

        if (!(iss >> shaperName >> isleName)) {
            continue;
        }


        RealmShaper *shaper = shaperTree.findPlayer(shaperName);
        Isle *isle = mapTree.findIsle(isleName);

        if (shaper && isle) {
            exploreArea(shaper, isle);
        }

            if(shaper&&!isle){
                craft(shaper, isleName);
            }

        accessCount++;


        if (accessCount % 5 == 0 && std::getline(duelFile, line)) {
            std::istringstream duelStream(line);
            std::string challengerName, opponentName;
            bool result;

            if (!(duelStream >> challengerName  >> result)) {
                continue;
            }

            RealmShaper *challenger = shaperTree.findPlayer(challengerName);

            if (challenger) {
                shaperTree.duel(challenger, result);
            } else {
                std::cerr << "[Error] Invalid duel log: " << line << std::endl;
            }
        }
    }

    while (std::getline(duelFile, line)) {
        std::istringstream duelStream(line);
        std::string challengerName, result;
        duelStream >> challengerName >> result;

        RealmShaper *challenger = shaperTree.findPlayer(challengerName);
        bool resultBool = (result == "1");
        if (challenger) {
            shaperTree.duel(challenger, resultBool);
        }
    }

    displayGameState();


    accessFile.close();
    duelFile.close();
}




void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}