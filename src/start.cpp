#include<iostream>
#include<string>
#include"playerinfo.hpp"
using namespace std;

void start(player_info &p){
	cout<<"Please enter your name: ";
	cin>>p.name;
	string input;
	cout<<"Please allocate your skill points:"<<endl;
	cout<<"You have "<<p.skill_points<<" skill points at the moment"<<endl;
	cout<<"You can boost either your 'Attack', 'HP' or 'Spirit'."<<endl;
	while (p.skill_points!=0){
		cout<<"Input a number(1: Attack, 2: HP, 3: Spirit): ";
		cin>>input;
		if (input=="1"){
			p.attack+=1;
			p.skill_points-=1;
			p.print_ahs();
		}
		else if (input=="2"){
			p.hp+=1;
			p.skill_points-=1;
			p.print_ahs();
		}
		else if (input=="3"){
			p.spirit+=1;
			p.skill_points-=1;
			p.print_ahs();
		}
		else
		cout<<"Please enter a valid number."<<endl;
	}
}
