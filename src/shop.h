#ifndef SHOP_H
#define SHOP_H

#include "playerinfo.h"
#include "utils/game.h"

/**
 * Represents an item in the shop
 */
struct ShopItem {

    private:
    Card *card;
    int gold_cost;


    public:
    ShopItem(Card *_card, int cost) {
        card = _card;
        gold_cost = cost;
    }

    ~ShopItem() {
        delete card;
    }

    /**
     * Get the pointer of the card of this item
     * @return
     */
    [[nodiscard]] Card *getCard() const {
        return card;
    }

    /**
     * Get the gold cost of this item
     * @return
     */
    [[nodiscard]] int getGoldCost() const {
        return gold_cost;
    }
};

/**
 * Represents a shop holding items
 */
class Shop {

    private:
    int size;
    protected:
    std::vector<ShopItem*> items;

    void removeItem(int index) {
        if (index < 0 || index > size - 1) {
            return;
        }
        items.erase(items.begin() + index);
    }

    /**
     * Instantiate a new shop with a given size.
     * <strong>NOTE:</strong> call <code>Shop#generateItems</code>
     */
    public:
    explicit Shop(const int &_size) {
        size = _size;
    }

    virtual ~Shop() {
        for (const auto &ptr : items) {
            // Delete the individual ShopItem instances as they are no longer needed
            delete ptr;
        }
        items.clear();
    }

    /**
     * Get the max size for this shop when fully stocked
     * @return A positive integer of the original shop size
     */
    [[nodiscard]] int getOriginalSize() const noexcept(true) {
        return size;
    }

    /**
     * Get the current size of the shop (aka the number of elements in stock)
     * @return Return the number of item the shop currently has
     */
    [[nodiscard]] int getSize() const noexcept(true) {
        return (int) items.size();
    }

    /**
     * Populate this shop fully with <code>ShopItem</code>s
     * This function will destroy (the point and instance) of any
     * existing <code>ShopItem</code>
     * @param client
     */
    virtual void generateItems(GameClient *client) = 0;

    /**
     * Attempt to buy the <code>ShopItem</code> at the provided
     * index as the <code>Player</code>
     * This method will emit messages to std::cout to inform the
     * player of the purchase result. Upon a successful purchase
     * this function will remove the ShopItem by calling <code>Shop#remove(int)</code>
     *
     * @param player The player object buying the item
     * @param index The index of the item to buy
     * @throws std::invalid_argument If the index is out of bounds
     */
    virtual void buyItem(Player *player, int index) = 0;

    /**
     * Print a greeting message to the player
     * @param player The player object
     */
    virtual void greetPlayer(Player *player) = 0;

    /**
     * Display the <code>ShopItem</code> at the given index
     * to std::cout in a user friendly manner.
     * @param client The game client
     * @param player The player
     * @param index The index of the item
     * @throws std::invalid_argument If the index is out of bounds
     */
    virtual void printItem(GameClient *client, Player *player, const int &index) = 0;

};

/**
 * Instantiate a new Shop on the heap
 * @param size The non-negative size of the shop (aka the number of items it should hold)
 * @return Return a pointer to the newly instantiated shop
 */
Shop *newShop(const int &size);



#endif//SHOP_H
