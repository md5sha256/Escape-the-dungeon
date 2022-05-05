#ifndef PLAYERINFO_HPP
#define PLAYERINFO_HPP
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct player_info {
    string name;
    int hp = 10;
    int attack = 1;
    int defence = 0;
    int spirit = 3;
    int skill_points = 5;
    int gold = 0;
    vector<int> inventory_cards = {0, 0, 0, 1, 1, 2, 2, 2, 3, 3};
    vector<int> in_hand_cards;
    vector<int> path;
    vector<int> bagpack;
    int position = 0;

    void print_ahs() {
        cout << "HP: " << hp << endl;
        cout << "Attack: " << attack << endl;
        cout << "Spirit: " << spirit << endl;
    }

    void print_all_status() {
        print_ahs();
        cout << "Spirit: " << spirit << endl;
        cout << "skill_points: " << skill_points << endl;
    }

    void print_in_hand_cards() {
        for (int i = 0; i < in_hand_cards.size(); i++) {
            cout << in_hand_cards[i] << " ";
        }
    }

    void print_inventory_cards() {
        for (int i = 0; i < inventory_cards.size(); i++) {
            cout << inventory_cards[i] << " ";
        }
    }

    void print_map() {
        for (int i = 0; i < 10; i++) {
            if (i != position) {
                if (path[i] == 0)
                    cout << "Battle>>";
                else if (path[i] == 1)
                    cout << "Campfire>>";
                else if (path[i] == 2)
                    cout << "Shop>>";
                else if (path[i] == 3)
                    cout << "Event>>";
                else
                    cout << "Boss>>";
            } else {//highlight the current postion of player on map
                if (path[i] == 0)
                    cout << "**Battle**>>";
                else if (path[i] == 1)
                    cout << "**Campfire**>>";
                else if (path[i] == 2)
                    cout << "**Shop**>>";
                else if (path[i] == 3)
                    cout << "**Event**>>";
                else
                    cout << "**Boss**>>";
            }
        }
        cout << "Escape" << endl;//print the final station on the map
    }
    void allocate_sp() {
        string input;
        cout << "You have " << skill_points << " skill points at the moment" << endl;
        cout << "You can boost either your 'Attack', 'HP' or 'Spirit'." << endl;
        while (skill_points != 0) {
            cout << "Input a number(1: Attack, 2: HP, 3: Spirit): ";
            cin >> input;
            if (input == "1") {
                attack += 1;
                skill_points -= 1;
                print_ahs();
            } else if (input == "2") {
                hp += 1;
                skill_points -= 1;
                print_ahs();
            } else if (input == "3") {
                spirit += 1;
                skill_points -= 1;
                print_ahs();
            } else if (input == "quit") {
                break;
            } else
                cout << "Please enter a valid number." << endl;
        }
    }
        
        
    bool check_if_dead() {
        if (hp <= 0) {
            return true;
        } else
            return false;
    }
	
	void take_damage(int damage){
		if (defence>0 && defence >=damage){
			defence-=damage;
		}
		else if (defence>0 && defence <= damage){
			damage-=defence;
			defence=0;
			hp-=damage;
		}
		else
		hp-=damage;
    }

    void generate_in_hand_cards() {
        srand(time(NULL));
        for (int i = 0; i < 5; i++) {                   //total of 5 cards should be generated
            int j = rand() % (inventory_cards.size());  //find a random index in inventory cards
            in_hand_cards.push_back(inventory_cards[j]);//use the index to find the specific card generated
        }
    }
};

struct path {
    vector<int> path1;
    vector<int> path2;
    vector<int> path3;
    //path id: 0 for battle,1 for campfire, 2 for shop,3 for random event
    void generate() {
        srand(time(NULL) + 1);
        path1.push_back(0);          //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        for (int i = 0; i < 8; i++) {//generate the first path
            int j = rand() % 5;
            if (j == 3) {//limit the appearance of shop,campfire and random event to 1/4 probability
                int j = (rand() % 3) + 1;
                path1.push_back(j);
            } else
                path1.push_back(0);
        }
        srand(time(NULL) + 2);
        path2.push_back(0);          //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        for (int i = 0; i < 8; i++) {//generate the second path
            int j = rand() % 5;
            if (j == 3) {//limit the appearance of shop,campfire and random event to 1/4 probability
                int j = (rand() % 3) + 1;
                path2.push_back(j);
            } else
                path2.push_back(0);
        }
        srand(time(NULL) + 3);//generate the third path
        path3.push_back(0);   //player should encounter battle at the beginning of each path, for earning enough money to buy stuff in shop
        for (int i = 0; i < 8; i++) {
            int j = rand() % 5;
            if (j == 4) {//limit the appearance of shop,campfire and random event to 1/5 probability
                int j = (rand() % 3) + 1;
                path3.push_back(j);
            } else
                path3.push_back(0);
        }
        path1.push_back(4);//boss generation at the end of each path, with ID of 5
        path2.push_back(4);
        path3.push_back(4);
    }
};

struct shop_item{
	int card_id;
	int gold_cost;
};


vector<shop_item> generate_shop_item(){
	vector<shop_item> item;
	for (int i=4;i<10;i++){
		shop_item temp;
		temp.card_id=i;
		temp.gold_cost=(rand()%40)+20;//cost of cards varies each time;
		item.push_back(temp);
	}
	return item;
}

struct enemy{
	string name;
	int attack;
	int defence;
	int hp;
	
	void enemy_generation(){
		srand(time(NULL));
		attack=(rand()%5)+1;
		defence=(rand()%5)+1;
		hp=(rand()%4)+1;
		//naming of enemy
		if ((attack+defence)>=8)//powerful enemy
		name="Elite ";
		else if ((attack+defence)>=5)//strong enemy
		name="Adult ";
		else//weak enemy
		name="Baby";
		if (defence*2<=attack)//if attack is twice as great as defence
		name+="Zombie";
		else if (attack*2<=defence)//if defence is twice as great as attack
		name+="Skeleton";
		else//if somewhere balance
		name+="Shrem";
	}
	
	bool check_if_dead(){
		if(hp<=0){
			return true;
		}
		else
		return false;
	}
	
	void enemy_take_damage(int damage){
		if (defence>0 && defence >=damage){
			defence-=damage;
		}
		else if (defence>0 && defence <= damage){
			damage-=defence;
			defence=0;
			hp-=damage;
		}
		else
		hp-=damage;
	}
};

void initPlayer(player_info &p);
void shop(player_info p, vector<shop_item> goods);
vector<enemy> generate_enemies();
void random_event(player_info &p);
void boss(player_info p);
void campfire(player_info &p);
void event1(player_info &p);
void event2(player_info &p);
void event3(player_info &p);
void event4(player_info &p);

#endif//PLAYERINFO_HPP
