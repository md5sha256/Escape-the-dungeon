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
        cout << "Thank you for your purchase!" << endl
             << "You have " << p.getGold() << " left." << endl;
    } else
        cout << "Sorry, you don't have enough gold" << endl;
}

void shop(Player &p) {
    string input;
    vector<ShopItem> goods;
    cout << p.getName() << " found a shop. There is different card that can be bought." << endl;
    vector<ShopItem> item;
    while (true) {
        shop_item_print();
        cout << "You have " << p.getGold() << " gold." << endl
             << "Would you like to buy? (/Buy) or skip (/skip): ";
        cin >> input;
        if (input == "/buy") {
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
