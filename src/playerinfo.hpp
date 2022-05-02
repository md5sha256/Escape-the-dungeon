#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct player_info {
    int hp = 10;
    int attack = 1;
    int defence = 0;
    int spirit = 3;
    vector<string> inventory_cards;
    vector<string> in_hand_cards;

    void print_in_hand_cards() {
        for (auto &in_hand_card : in_hand_cards) {
            cout << in_hand_card << " ";
        }
    }

    void print_inventory_cards() {
        for (auto &inventory_card : inventory_cards) {
            cout << inventory_card << " ";
        }
    }
};

void start(player_info p);
void fight(player_info p);
void campfire(player_info p);
void shop(player_info p);
void random_event(player_info p);
void boss(player_info p);

#endif//HEADER_H
