#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<C:\Users\Davin\Desktop\Study\Uni stuffs\1340\Project\header.h>
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

	
void enemy_take_damage(int damage,vector<enemy> enemy_generated,int enemy_id){
		
	if (enemy_generated[enemy_id].defence >=damage){
		enemy_generated[enemy_id].defence-=damage;
	}
	else if (defence>0 && enemy_generated[enemy_id].defence <= damage){
		damage-=enemy_generated[enemy_id].defence;
		enemy_generated[enemy_id].defence=0;
		enemy_generated[enemy_id].hp-=damage;
	}
	else
	enemy_generated[enemy_id].hp-=damage;
	check_if_dead(enemy_id);
}



void battle(player_info &p,vector<enemy> &enemy_list){
	while(enemy_list.size()!=0){
		
	}
}











