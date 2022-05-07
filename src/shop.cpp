#include "shop.hpp"
#include <iostream>
#include <string>
#include "utils/game.hpp"
#include "utils/cardtemplates.hpp"

class SimpleShop : public Shop {

    static ShopItem generateShopItem(GameClient *client) {
        // LEGENDARY CARDS
        int chanceLegendary = 100 - 10;
        int priceLegendary = 200;
        int legendary[] = {REVIVE_CARD, WIN_BATTLE_CARD};
        // RARE CARDS
        int chanceRare = 100 - 30;
        int priceRare = 40;
        int rare[] = {TELEPORT_CARD, SKILL_POINT_CARD};

        // COMMON CARDS
        int chanceCommon = 100 - 60;
        int priceCommon = 10;
        int common[] = {DAMAGE_CARD};

        int random = rand() % 101;
        int templateId;
        int price;
        if (random >= chanceLegendary) {
            int size = sizeof legendary;
            int index = random % size;
            templateId = legendary[index];
            price = priceLegendary;
        } else if (random >= chanceRare) {
            int size = sizeof rare;
            int index = random % size;
            templateId = rare[index];
            price = priceRare;
        } else if (random >= chanceCommon) {
            templateId = common[0];
            price = priceCommon;
        } else {
            throw std::invalid_argument("Improper card setup detected!");
        }
        Card *item = client->getDatabase()->createCard(templateId);
        return ShopItem{item, price};
    }

    public:
    explicit SimpleShop(const int &size) : Shop(size) {

    }

    void generateItems(GameClient *client) override {
        items.clear();
        items.reserve(getSize());
        for (int i = 0; i < getOriginalSize(); i++) {
            items[i] = generateShopItem(client);
        }
    }

    void buyItem(Player *player, int index) override {
        if (index < 0 || index > getSize() - 1) {
            throw std::invalid_argument("Invalid item index: " + std::to_string(index));
        }
        ShopItem item = items[index];
        if (player->getGold() >= item.getGoldCost()) {
            player->addCardToInventory(item.getCard());
            player->modifyGold(-item.getGoldCost());
            std::cout << "\"Yes! it would definitely help your journey in the dungeon!\"" << std::endl
                      << "You have " << player->getGold() << " left." << std::endl;
            removeItem(index);
        } else {
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
            std::cout << "\"Back for more eh?\"" << std::endl;
            std::cout << "said the little Goblin standing in front of the shelf." << std::endl;
        }
        printf("\"%s %d %s\"\n", "We've got", getSize(), "items for sale today!");
        std::cout << "\"Feel free to look around and see if there is anything interests you.\"" << std::endl;
    }

    void printItem(GameClient *client, Player *player, const int &index) override {
        if (index < 0 || index > getSize() - 1) {
            throw std::invalid_argument("Invalid index: " + std::to_string(index));
        }
        ShopItem item = items[index];
        Card* card = item.getCard();
        Optional<CardTemplate*> optionalTemplate = client->getCardTemplates()->get(card->getTemplateId());
        if (optionalTemplate.isEmpty()) {
            throw std::logic_error("Invalid card template: " + std::to_string(card->getTemplateId()));
        }
        std::cout << std::endl;
        printf("%s %d", "Item", index);
        CardTemplate* cardTemplate = *optionalTemplate.value();
        cardTemplate->displayCard(card);
        printf("%s: %d\n", "Cost", item.getGoldCost());
    }

};

Shop* newShop(const int &size) {
    return new SimpleShop{size};
}