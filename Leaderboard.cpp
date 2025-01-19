#include "Leaderboard.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
  int count=0;
  ifstream file(filename);
  if(file.is_open()){
    string line;
    while(getline(file,line)){
        if(count<=10){
      string playerName;
      unsigned long score;
      time_t lastPlayed;
      stringstream ss(line);
      ss>>playerName>>score>>lastPlayed;
      LeaderboardEntry* newEntry = new LeaderboardEntry(score,lastPlayed,playerName);
      insert(newEntry);
      count++;

    }}
    file.close();
  } else {
    std::cout << "Unable to open file: " << filename << std::endl;
  }
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    ofstream file(filename);
    if(file.is_open()){
        LeaderboardEntry* current = head_leaderboard_entry;
        while(current!=nullptr){
        file<<current->player_name<<" "<<current->score<<" "<<current->last_played<<std::endl;
        current = current->next;
        }
        file.close();
    } else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    LeaderboardEntry* current = head_leaderboard_entry;
    while(current!=nullptr){
        std::cout<<current->player_name<<" "<<current->score<<" "<<current->last_played<<std::endl;
        current = current->next;
    }
}




//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if(head_leaderboard_entry==nullptr){
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry* current = head_leaderboard_entry;
        LeaderboardEntry* prev = nullptr;
        while(current!=nullptr){
            if(new_entry->score>current->score){
                if(prev==nullptr){
                    new_entry->next = current;
                    head_leaderboard_entry = new_entry;
                    return;
                } else {
                    prev->next = new_entry;
                    new_entry->next = current;
                    return;
                }
            }
            prev = current;
            current = current->next;
        }
        prev->next = new_entry;
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    LeaderboardEntry* current = head_leaderboard_entry;
    while(current!=nullptr){
        LeaderboardEntry* next = current->next;
        delete current;
        current = next;
    }
}
