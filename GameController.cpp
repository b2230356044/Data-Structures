#include "GameController.h"
#include <fstream>
#include <algorithm>
#include "GameController.h"
#include <fstream>
#include <iostream>
#include <algorithm>

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);

}


// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    std::ifstream file(commands_file);
    if (!file.is_open()) {
        std::cerr << "Unable to open commands file: " << commands_file << std::endl;
        return;
    }

    std::string command;
    game->game_time = 0;

    while (std::getline(file, command)) {
        game->update_space_grid();

        if (game->game_over || game->player->lives <= 0) {
            break;
        }


        if (game->game_over || game->player->lives <= 0) {
            std::cout << "Game Over!" << std::endl;
            std::cout << "Tick: " << game->game_time << std::endl;
            std::cout << "Lives: " << game->player->lives << std::endl;
            std::cout << "Ammo: " << game->player->current_ammo << std::endl;
            std::cout << "Score: " << game->current_score << std::endl;
            break;
        }


        if (command == "PRINT_GRID") {
            game->print_space_grid();
        } else if (command == "MOVE_UP") {
            if (game->player->position_row > 0) {
                game->player->move_up();
            }
        } else if (command == "MOVE_DOWN") {
            if (game->player->position_row < AsteroidDash::space_grid.size() - 1) {
                game->player->move_down(AsteroidDash::space_grid.size());
            }
        } else if (command == "MOVE_LEFT") {
            if (game->player->position_col > 0) {
                game->player->move_left();
            }
        } else if (command == "MOVE_RIGHT") {
            if (game->player->position_col < AsteroidDash::space_grid[0].size() - 1) {
                game->player->move_right(AsteroidDash::space_grid[0].size());
            }
        } else if (command == "SHOOT") {
            game->shoot();
        } else if (command == "NOP") {

        } else {
            std::cerr << "Invalid command: " << command << std::endl;
        }


        game->game_time++;
        game->current_score++;

        if (game->leaderboard.head_leaderboard_entry) {
            if (game->current_score > game->leaderboard.head_leaderboard_entry->score) {
                game->leaderboard.head_leaderboard_entry->score = game->current_score;
            }
        }


    }

    file.close();



    if (game->game_over || game->player->lives <= 0) {
        std::cout << "GAME OVER!" << std::endl;
    } else {
        std::cout << "GAME FINISHED! No more commands!" << std::endl;
    }
    std::cout << "Tick: " << game->game_time << std::endl;
    std::cout << "Lives: " << game->player->lives << std::endl;
    std::cout << "Ammo: " << game->player->current_ammo << std::endl;
    std::cout << "Score: " << game->current_score << std::endl;
    std::cout << "High Score: " << "0"<< std::endl;
    std::cout << "Player: " << game->player->player_name << std::endl;


    game->print_space_grid();
    std::cout << std::endl;
    std::cout << "Leaderboard" << std::endl;
    std::cout << "----------" << std::endl;
    game->leaderboard.print_leaderboard();


    game->leaderboard.write_to_file(game->leaderboard_file_name);
}

// Destructor to delete dynamically allocated member variables
GameController::~GameController() {
    delete game;
}
