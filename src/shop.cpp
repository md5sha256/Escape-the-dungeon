#include "shop.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void shop_item_print() {
}

void buy_item(Player &p, Card *card, int gold_cost) {
    if (p.getGold() >= gold_cost) {
        p.getInventory().push_back(card);
        p.modifyGold(-gold_cost);
        cout << "\"Yes! it would definately help your journey in the dungeon!\"" << endl
             << "You have " << p.getGold() << " left." << endl;
    } else
        cout << "\"Doesn't look like your wallet is ready for this..\"" << endl;
}

void shop(Player &p) {
    string input;
    vector<ShopItem> goods;
    cout << p.getName() << " found a house, with a signboard written:\"7/24 business\"" << endl;
    vector<ShopItem> item;
    item=generate_shop_item();
	cout<<"You have "<<p.gold<<" gold."<<endl;
    int buy_count=0;//the number of time player enters /buy command.
    while (true) {
        cout<<"Would you like to take a look inside? (/buy) or leave (/skip): ";
        cin >> input;
        if (input == "/buy") {
            buy_count+=1;
            if (buy_count==0)//Welcome from shop owner for the first time entering the shop
            cout << "\"Oh, a human! Haven't seen one in quite a few years, what can I help you?\""<<endl<<"said a little Goblin standing in front of a shelf.";
            cout << "\"Free free to look around and see if there is anything interests you.\""<<endl;
            shop_item_print();
            while (true) {
                cout << "Enter the item ID that you want to buy or enter /back to cancel the action: ";
                cin >> input;
                if (input == "6" || input == "1" || input == "2" || input == "3" || input == "4" || input == "5") {
                    int num;
                    stringstream ss(input);
                    ss >> num;
                    num -= 1;
                    cout << item[num].getCard()->getId() << item[num].getGoldCost();
                    buy_item(p, item[num].getCard(), item[num].getGoldCost());
                } else if (input == "/back")
                    break;
                else
                    cout << "Enter a valid ID or command!" << endl;
            }
        } else if (input == "/skip") {
            p.incrementPosition();
            break;
        } else
            cout << "Please enter a valid command!" << endl;
    }
}
