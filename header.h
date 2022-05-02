#ifndef HEADER_H
#define HEADER_H
#include<string>
using namespace std;

extern struct player_info{
	int hp=10;
	int attack=1;
	int defence=0;
	int spirit=3;
	vector<string> inventory_cards;
	vector<string> in_hand_cards;
	
	print_in_hand_cards(){
		for (int i=0;i<in_hand_cards.size();i++){
			cout<<in_hand_cards[i]<<" ";
		}
	}
	
	print_inventory_cards(){
		for (int i=0;i<inventory_cards.size();i++){
			cout<<inventory_cards[i]<<" ";
		}
	}
	
};

void start(player_info p);
void fight(player_info p);
void campfire(player_info p);
void shop(player_info p);
void random_event(player_info p);
void boss(player_info p);


#endif
