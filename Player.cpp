#include "Player.h"

// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo, int lives)
        : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name), max_ammo(max_ammo),
          current_ammo(max_ammo), lives(lives) {
    // TODO: Your code here, if you want to do further initializations
}

// Move player left within the grid boundaries
void Player::move_left() {
    if (position_col - 1 >= 0) {
        position_col=position_col-1;
    }
}

// Move player right within the grid boundaries
void Player::move_right(int grid_width) {
    int rightmost_col = position_col + spacecraft_shape[0].size() - 1;
    if (rightmost_col + 1 < grid_width) {
        position_col=position_col+1;
    }
}

// Move player up within the grid boundaries
void Player::move_up() {
    if (position_row - 1 >= 0) {
        position_row=position_row-1;
    }
}

// Move player down within the grid boundaries
void Player::move_down(int grid_height) {
    int bottommost_row = position_row + spacecraft_shape.size() - 1;
    if (bottommost_row + 1 < grid_height) {
        position_row=position_row+1;
    }
}
