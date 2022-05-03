#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include'playerinfo.hpp'
using namespace std;

vector<enemy> generate_enemies(){
			vector<enemy> enemy_generated;//the list of enemy generated
			srand(time(NULL));
			int num_enemy=(rand()%5)+1;
			for (int i=0;i<num_enemy;i++){
				enemy temp;
				srand(time(NULL)+i);
				temp.enemy_generation();//call for the helper function to generate one enemy
				enemy_generated.push_back(temp);//append this enemy to the enemy generated list
			}
			return enemy_generated;
}



void battle(){
	
}
