#include<iostream>
#include<algorithm>
#include<string>
#include"start.cpp"
#include"playerinfo.hpp"

using namespace std;


int main(){
	player_info p;
	start(p);
	while(p.position!=10){
		if(p.path[p.position]==0){//when battle is encountered
			vector<enemy> enemy_generated;
			enemy_generated=generate_enemies();
		}
		else if(p.path[p.position]==1){//when campfire is encountered
			campfire(p);
		}
		else if(p.path[p.position]==2){//when shop is encountered

		}
		else if(p.path[p.position]==3){//when random event is encountered

		}
		else if(p.path[p.position]==4){//when boss is encountered

		}

		p.position+=1;
	}
}
