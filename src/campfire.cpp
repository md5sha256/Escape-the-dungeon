#include<iostream>
#include<string>
#include"playerinfo.hpp"

void campfire(player_info &p){
	string input;
	cout<<p.name<<" found a place to rest"<<endl<<"He/she started a campfire";
	cout<<"Note: You obtained 2 additional skill points, you may want to allocate them."<<endl;
	p.allocate_sp();
}
