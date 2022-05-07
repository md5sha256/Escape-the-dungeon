#include "shop.h"
#include "utils/cardtemplates.h"
#include "utils/game.h"
#include <iostream>
#include <string>

/**
 * Implementation of the Shop class defined in shop.h
 */
class SimpleShop : public Shop {

    /**
     * Generate a shop item
     * This item has a weighting of the following:
     * 10% chance of legendary templates
     * 30% chance of rare templates
     * 60% chance of common templates
     * This function will create a new <code>ShopItem</code> on the heap
     * and populate it with a random, fully initialized <code>Card</code>
     * @param client The game client
     * @return Return a pointer to the newly instantiated <code>ShopItem</code> object
     * @throws std::invalid_argument Thrown if the <code>CardTemplate</code> could not be found.
     * Normally, this error should **never** be thrown unless there is some data corruption with
     * global variables or if template registration in the <code>GameClient</code> was messed up
     */
    static ShopItem* generateShopItem(GameClient *client) noexcept(true) {
        // LEGENDARY CARDS
        int chanceLegendary = 90;
        int priceLegendary = 200;
        std::vector<int> legendary = {REVIVE_CARD, WIN_BATTLE_CARD};
        // RARE CARDS
        int chanceRare = 70;
        int priceRare = 40;
        std::vector<int> rare = {TELEPORT_CARD, SKILL_POINT_CARD};

        // COMMON CARDS
        int priceCommon = 10;
        std::vector<int> common = {DAMAGE_CARD};

        // Get a random number between 0 and 100
        int random = randIntPercent();
        int templateId;
        int price;
        if (random >= chanceLegendary) {
            templateId = randomElement(legendary);
            price = priceLegendary;
        } else if (random >= chanceRare) {
            templateId = randomElement(rare);
            price = priceRare;
        } else {
            templateId = common[0];
            price = priceCommon;
        }
        // Create a new card on the heap
        Card *item = client->getDatabase()->createCard(templateId);
        // Lookup the card template from the ID
        CardTemplate* cardTemplate = *client->getCardTemplates()->get(templateId).value();
        // Initialize the card with random values
        cardTemplate->initCard(item);
        // Create and return a pointer to the new ShopItem instance
        return new ShopItem{item, price};
    }

    public:
    explicit SimpleShop(const int &size) : Shop(size) {

    }


    void generateItems(GameClient *client) override {
        // Delete all existing items
        items.clear();
        // Reserve enough space
        items.reserve(getOriginalSize());
        for (int i = 0; i < getOriginalSize(); i++) {
            // Add the newly generated shop item
            items.push_back(generateShopItem(client));
        }
    }

    void buyItem(Player *player, int index) override {
        // Validate the index
        if (index < 0 || index > getSize() - 1) {
            throw std::invalid_argument("Invalid item index: " + std::to_string(index));
        }
        ShopItem *item = items[index];
        // Check if the player has enough money
        if (player->getGold() >= item->getGoldCost()) {
            // Add the card to the inventory
            player->addCardToInventory(item->getCard());
            // Deduce from the player's gold balance
            player->modifyGold(-item->getGoldCost());
            // Emit friendly message
            std::cout << "\"Yes! it would definitely help your journey in the dungeon!\"" << std::endl
                      << "You have " << player->getGold() << " gold left." << std::endl;
            removeItem(index);
        } else {
            // Player doesn't have enough money; emit friendly message
            std::cout << "\"Doesn't look like your wallet is ready for this..\"" << std::endl;
        }
    }

    void greetPlayer(Player *player) override {
        std::string input;
        std::cout << player->getName() << " found a house, with a signboard written:\"7/24 business\"" << std::endl;
        std::cout << "You have " << player->getGold() << " gold." << std::endl;
        if (getOriginalSize() == getSize()) {
            // Initial greeting
            std::cout << "\"Oh, a human! Haven't seen one in quite a few years, what can I help you with?\"" << std::endl
                      << "said a little Goblin standing in front of a shelf." << std::endl;
        } else {
            // Separate message for if the player has previously bought something from the shop
            std::cout << "\"Back for more eh?\"" << std::endl;
            std::cout << "said the little Goblin standing in front of the shelf." << std::endl;
        }
        printf("\"%s %d %s\"\n", "We've got", getSize(), "items for sale today!");
        std::cout << "\"Feel free to look around and see if there is anything interests you.\"" << std::endl;
    }

    void printItem(GameClient *client, Player *player, const int &index) override {
        // Validate the index
        if (index < 0 || index > getSize() - 1) {
            throw std::invalid_argument("Invalid index: " + std::to_string(index));
        }
        // Locate the ShopItem and the Card
        ShopItem *item = items[index];
        Card* card = item->getCard();
        Optional<CardTemplate*> optionalTemplate = client->getCardTemplates()->get(card->getTemplateId());
        if (optionalTemplate.isEmpty()) {
            // This error should NEVER be thrown but exists to catch an illegal state
            throw std::logic_error("Invalid card template: " + std::to_string(card->getTemplateId()));
        }
        // Newline so it looks nicer
        std::cout << std::endl;
        // Print a header so the player knows which index this item is at
        printf("%s %d\n", "Item", index + 1);
        CardTemplate* cardTemplate = *optionalTemplate.value();
        // Display the card
        cardTemplate->displayCard(card);
        // Display the cost
        printf("%s: %d\n", "Cost", item->getGoldCost());
    }

};

/// Proxy the <code>SimpleShop</code> ctor
Shop* newShop(const int &size) {
    return new SimpleShop{size};
}
