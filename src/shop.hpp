#ifndef SHOP_HPP
#define SHOP_HPP

#include "playerinfo.hpp"

struct ShopItem {

    ShopItem(Card *_card, int cost) {
        card = _card;
        gold_cost = cost;
    }

    private:
    Card *card;
    int gold_cost;

    public:
    Card *getCard() const {
        return card;
    }

    int getGoldCost() const {
        return gold_cost;
    }
};

#endif//ESCAPE_THE_DUNGEON_SHOP_HPP
