#include"start.hpp"
#include<iostream>
#include<string>
using namespace std;

void start(player_info &p){
	cout<<"Please enter your name: ";
	cin>>p.name;
	string input;
	cout<<"Please allocate your skill points:"<<endl;
	p.allocate_sp();
	path g;
	g.generate();
	cout<<"Please select your path to the entrance."<<endl;
	cout<<"Path 1:"<<endl;
	for(int i=0;i<10;i++){
		if (g.path1[i]==0)
		cout<<"Battle>>";
		if (g.path1[i]==1)
		cout<<"Campfire>>";
		if (g.path1[i]==2)
		cout<<"Shop>>";
		if (g.path1[i]==3)
		cout<<"Event>>";
		if (g.path1[i]==4)
		cout<<"Boss>>Escape"<<endl;
	}
	cout<<"Path 2:"<<endl;
	for(int i=0;i<10;i++){
		if (g.path2[i]==0)
		cout<<"Battle>>";
		if (g.path2[i]==1)
		cout<<"Campfire>>";
		if (g.path2[i]==2)
		cout<<"Shop>>";
		if (g.path2[i]==3)
		cout<<"Event>>";
		if (g.path2[i]==4)
		cout<<"Boss>>Escape"<<endl;
	}
	cout<<"Path 3:"<<endl;
	for(int i=0;i<10;i++){
		if (g.path3[i]==0)
		cout<<"Battle>>";
		if (g.path3[i]==1)
		cout<<"Campfire>>";
		if (g.path3[i]==2)
		cout<<"Shop>>";
		if (g.path3[i]==3)
		cout<<"Event>>";
		if (g.path3[i]==4)
		cout<<"Boss>>Escape"<<endl;
	}
	cout<<"Enter a path number: ";
	cin>>input;
	while(p.path.size()==0){
		if (input=="1")
	p.path=g.path1;
	else if (input=="2")
	p.path=g.path2;
	else if (input=="3")
	p.path=g.path3;
	else
	cout<<"Please enter a valid number!"<<endl;
	}
	
}
