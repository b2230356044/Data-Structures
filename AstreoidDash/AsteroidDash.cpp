#include "AsteroidDash.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

vector<vector<int>> AsteroidDash::space_grid;

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    ifstream file(input_file);
    if(file.is_open()){
        string line;
        while(getline(file,line)){
            std::vector<int> row;
            for (char c : line) {
                if (c == '0') {
                    row.push_back(c - '0');
                }
            }
            if (!row.empty()) {
                space_grid.push_back(row);
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << input_file << std::endl;
    }
}
// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    ifstream file(player_file_name);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << player_file_name << std::endl;
        return;
    }

    string line;
    vector<vector<bool>> player_shape;
    int startRow = 0, startCol = 0;
    int currentRow = 0;

    while (getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

        if (currentRow == 0) {
                startRow = line[0] - '0';
                startCol = line[1] - '0';

        } else {
            vector<bool> row(line.size());
            for (size_t j = 0; j < line.size(); ++j) {
                row[j] = (line[j] == '1');
            }
            player_shape.push_back(row);
        }
        ++currentRow;
    }

    file.close();
    if (player_shape.empty() || currentRow == 0) {
        std::cerr << "Player shape is invalid or file is empty: " << player_file_name << std::endl;
        return;
    }
    constexpr int MAX_AMMO = 10;
    constexpr int PLAYER_LIVES = 3;
    player = new Player(player_shape, startRow, startCol, player_name, MAX_AMMO, PLAYER_LIVES);
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    ifstream file(input_file);
    if (file.is_open()) {
        string line;
        CelestialObject *current = nullptr;
        while (getline(file, line)) {

            if (line.empty()) {
                continue;
            }


            vector<vector<bool>> shape;
            if (line.front() == '[' || line.front() == '{') {
                char closing_bracket = (line.front() == '[') ? ']' : '}';
                do {
                    vector<bool> row;
                    for (char c : line) {
                        if (c == '0' || c == '1') {
                            row.push_back(c == '1');
                        }
                    }
                    shape.push_back(row);
                } while (getline(file, line) && line.front() != closing_bracket && line.front() != 's' && !line.empty());
            }


            int startRow = -1, startTime = -1;
            string effect;
            ObjectType objectType = ASTEROID;


            do{
                if (line.front() == 's') {
                    startRow = stoi(line.substr(2));
                } else if (line.front() == 't') {
                    startTime = stoi(line.substr(2));
                } else if (line.front() == 'e') {
                    effect = line.substr(2);
                    if (effect == "life") {
                        objectType = LIFE_UP;
                    } else if (effect == "ammo") {
                        objectType = AMMO;
                    }
                }
            }while (getline(file, line) && !line.empty()) ;
            CelestialObject *newObject = new CelestialObject(shape, objectType, startRow, startTime);


            if (!celestial_objects_list_head) {
                celestial_objects_list_head = newObject;
                current = celestial_objects_list_head;
            } else {
                current->next_celestial_object = newObject;
                CelestialObject *prev_rotation = current;
                do {
                    prev_rotation->next_celestial_object = newObject;
                    prev_rotation = prev_rotation->right_rotation;
                } while (prev_rotation && prev_rotation != current);

                current = current->next_celestial_object;
            }

        }

    }

    file.close();

}

// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    if (!space_grid.empty()) {
        for (const auto& row : space_grid) {
            for (int cell : row) {
                if (cell == 1) {
                    std::cout << occupiedCellChar;
                } else {
                    std::cout << unoccupiedCellChar;
                }
            }
            std::cout << std::endl;
        }
    }

}




// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid(){
    for (auto& row : space_grid) {
        fill(row.begin(), row.end(), 0);
    }
    for (size_t i = 0; i < player->spacecraft_shape.size(); ++i) {
        for (size_t j = 0; j < player->spacecraft_shape[i].size(); ++j) {
            if (player->spacecraft_shape[i][j]) {
                if (player->position_row + i < space_grid.size() &&
                    player->position_col + j < space_grid[0].size() &&
                    player->position_row + i >= 0 &&
                    player->position_col + j >= 0) {
                    space_grid[player->position_row + i][player->position_col + j] = 1;
                }
            }
        }
    }

    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr) {
        if (current->time_of_appearance <= game_time) {
            int horizontal_position = space_grid[0].size() - 1 - (game_time - current->time_of_appearance);

                for (size_t i = 0; i < current->shape.size(); ++i) {
                    for (size_t j = 0; j < current->shape[i].size(); ++j) {
                        if (current->shape[i][j]) {
                            int row = current->starting_row + i;
                            int col = horizontal_position + j;

                            if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[0].size()) {
                                if (space_grid[row][col] == 1) {
                                    handle_collision_with_player(current);
                                } else {
                                    space_grid[row][col] = 1;
                                }
                            }
                        }

                }
            }
        }
        current = current->next_celestial_object;
    }
}

void AsteroidDash::handle_collision_with_player(CelestialObject *object) {
    if (object->object_type == ASTEROID) {
        player->lives--;
        if (player->lives <= 0) {
            game_over = true;
        }
    } else if (object->object_type == LIFE_UP) {
        player->lives++;
    } else if (object->object_type == AMMO) {
        player->current_ammo = player->max_ammo;
    }

    object->time_of_appearance = -1;
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    if (player->current_ammo > 0) {
        player->current_ammo--;


        int projectile_row = player->position_row;
        int projectile_col = player->position_col + player->spacecraft_shape[0].size();
        if (projectile_row >= 0 && projectile_row < space_grid.size() &&
            projectile_col >= 0 && projectile_col < space_grid[0].size()) {
            space_grid[projectile_row][projectile_col] = 1;

        }
    }
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    CelestialObject* current = celestial_objects_list_head;
    while (current) {
        CelestialObject* next = current->next_celestial_object;
        delete current;
        current = next;
    }
}

