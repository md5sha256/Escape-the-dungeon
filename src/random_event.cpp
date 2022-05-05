#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include"playerinfo.hpp"
using namespace std;

int bad_end = 0;

void event1(player_info &p){
	cout<<p.name<<" saw a pond of acidic mucus, inside it are some human skulls and left behind jewellery"<<endl<<"You can choose to: 1. go into the pond and pick up those jewllery || 2. Leave"<<endl;
	cout<<"Your choice(Enter the number): ";
	string input;
	
	while (true){
		cin>>input;
		if (input=="1"){//if player choose to go into the pond
		int damage=(rand()%5)+1;//deal 1-5 damage to player
		p.take_damage(damage);
		if (p.check_if_dead()){
			cout<<p.name<<" endures the severe pain brought by strong acid, picked up the jewelry in the pond, but never got the chance to go back to the shore again"<<endl;
			bad_end=1;
		}
		else
		{	
			int gold=(rand()%70+30);
			cout<<p.name<<" brought back "<<gold<<" gold from the pond, but suffered "<<damage<<" damage"<<endl;
			p.gold+=gold;//give reward
		}
		break;
	}
	else if (input=="2"){
		p.position+=1;//move to the next
		break;
	}
	else
	cout<<"Input a valid number!"<<endl;
	}
}

void event2(player_info &p){
	cout<<p.name<<" found a Trevi fountain "<<endl<<"You can choose to: 1. Throw 10 gold into the Fountain || 2. Leave"<<endl;
	cout<<"Your choice(Enter the number): ";
	string input;
	
	while (true){
		cin>>input;
		if (input=="1"){//if player choose spend gold
		int luck=(rand()%2);//50/50 chance of getting reward
		if (luck==1){
			cout<<p.name<<" feels power filling the body"<<endl;
			int value=(rand()%3)+1;
			int type=(rand()%3);
			if (type==0){//boost HP 
			p.hp+=value;
			}
			else if (type==1)//boost attack
			p.attack+=value;
			else//boost spirit
			p.spirit+=value;
		}
		else
		{	
			cout<<p.name<<" waits for minitus, nothing happens."<<endl;
			cout<<"Enter /next to move to the next point on the map."<<endl;
		}
		break;
	}
	else if (input=="2"){
		p.position+=1;//move to the next
		break;
	}
	else
	cout<<"Input a valid number!"<<endl;
	}
}

void event3(player_info &p){
	cout<<p.name<<" saw a golden statue, seems to worth quite a lot of money"<<endl<<"You can choose to: 1. take the statue || 2. Leave"<<endl;
	cout<<"Your choice(Enter the number): ";
	string input;
	
	while (true){
		cin>>input;
		if (input=="1"){//if player choose take the statue
		int trap=(rand()%3);//1/3 chance of initiate a trap
		if (trap==1){
			cout<<p.name<<" took the status and triggered a device, the last thing he/she remember, is a giant rock falling from the roof..."<<endl;
			bad_end=1;
		}
		else
		{	
			cout<<p.name<<" took the status and wondered why such a precious would be placed here "<<endl;
			cout<<"It worth 100 gold!"<<endl;
			p.gold+=100;//give reward
		}
		break;
	}
	else if (input=="2"){
		cout<<p.name<<" decided not to take the statue."<<endl;
		p.position+=1;//move to the next
		break;
	}
	else
	cout<<"Input a valid number!"<<endl;
	}
}

void event4(player_info &p){//transmit the player,can be forward or backward;
	cout<<p.name<<" came to the folk of this road. "<<endl<<"You can choose to: 1. Go for the right || 2. Go for the left"<<endl;
	cout<<"Your choice(Enter the number): ";
	string input;
	int left=(rand()%5)-2;//either jump 2 points on the map or back 2 points (for the left)
	int right=(rand()%5)-2;//same as above, but for the right
	while (true){
		cin>>input;
		if (input=="1"){//if player choose to go right
		cout<<p.name<<" decided to go right."<<endl;
		cout<<"You are transferred "<<right<<" from the current position("<<p.position<<")."<<endl;
		p.position+=right;
		cout<<"Your position now: "<<endl;
		p.print_map();
		break;
	}
		else if (input=="2"){
		cout<<"You are transferred "<<left<<" from the current position("<<p.position<<")."<<endl;
		p.position+=left;
		cout<<"Your position now: "<<endl;
		p.print_map();
		break;
	}
	else
	cout<<"Input a valid number!"<<endl;
	}
} 


void random_event(player_info &p){
	srand(time(NULL));
	int choice=rand()%4;
	if (choice==0)
	event1(p);
	if (choice==1)
	event2(p);
	if (choice==2)
	event3(p);
	if (choice==3)
	event4(p);
}
