#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<sstream>
#include<C:\Users\Davin\Desktop\Study\Uni stuffs\1340\Project\header.h>
using namespace std;



void shop_item_print(){
	
}


void buy_item(player_info &p, int card_id, int gold_cost){
	if (p.gold>=gold_cost){
		p.inventory_cards.push_back(card_id);
		p.gold-=gold_cost;
		cout<<"Thank you for your purchase!"<<endl<<"You have "<<p.gold<<" left."<<endl;
	}
	else
		cout<<"Sorry, you don't have enough gold"<<endl;
}







void shop(player_info &p){
	string input;
	vector<shop_item> goods;
	cout<<p.name<<" found a shop. There is different card that can be bought."<<endl;
	vector<shop_item> item;
	item=generate_shop_item();
	while (true){
		shop_item_print();
		cout<<"You have "<<p.gold<<" gold."<<endl<<"Would you like to buy? (/Buy) or skip (/skip): ";
		cin>>input;
		if (input=="/buy"){
			while (true){
				cout<<"Enter the item ID that you want to buy or enter /back to cancel the action: ";
				cin>>input;
				if (input=="6" || input=="1" || input=="2" || input=="3" || input=="4" || input=="5")
				{	
					int num;
					stringstream ss(input);
					ss>>num;
					num-=1;
					cout<<item[num].card_id<<item[num].gold_cost;
					buy_item(p,item[num].card_id,item[num].gold_cost);
				}
				else if (input=="/back")
				break;
				else
				cout<<"Enter a valid ID or command!"<<endl;
			}
		}
		else if (input=="/skip"){
		p.position+=1;
		break;
		}
		else
		cout<<"Please enter a valid command!"<<endl;
	}
}




