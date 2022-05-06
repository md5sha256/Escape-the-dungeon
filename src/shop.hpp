#ifndef SHOP_HPP
#define SHOP_HPP

#include "playerinfo.hpp"
#include "utils/game.hpp"

struct ShopItem {

    ShopItem(Card *_card, int cost) {
        card = _card;
        gold_cost = cost;
    }

    private:
    Card *card;
    int gold_cost;

    public:
    [[nodiscard]] Card *getCard() const {
        return card;
    }

    [[nodiscard]] int getGoldCost() const {
        return gold_cost;
    }
};

class Shop {

    private:
    int size;
    protected:
    std::vector<ShopItem> items;

    explicit Shop(const int &_size) {
        size = _size;
    }

    void removeItem(int index) {
        if (index < 0 || index > size - 1) {
            return;
        }
        items.erase(items.begin() + index);
    }

    public:
    virtual ~Shop() = default;

    [[nodiscard]] int getOriginalSize() const noexcept(true) {
        return size;
    }

    [[nodiscard]] int getSize() const noexcept(true) {
        return (int) items.size();
    }


    std::vector<ShopItem> getItems() {
        return items;
    }

    virtual void generateItems(GameClient *client) = 0;

    virtual void buyItem(Player *player, int index) = 0;

    virtual void greetPlayer(Player *player) = 0;

    virtual void printItem(GameClient *client, Player *player, const int &index) = 0;

};

Shop *newShop(const int &size);



#endif//SHOP_HPP
