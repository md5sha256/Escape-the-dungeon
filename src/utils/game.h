#ifndef GAME_H
#define GAME_H

#include "../playerinfo.h"
#include "battle.h"
#include "database.h"
#include "registry.h"
#include "string"

/**
 * Represents the main instance which controls
 * code execution and program flow for the game
 */
class GameClient {

    public:
    virtual ~GameClient() = default;

    /**
     * Check whether the game is running
     * @return
     */
    virtual bool isRunning() noexcept(true) = 0;

    /**
     * Initialize the game without loading any data
     */
    virtual void init() noexcept(false) = 0;

    /**
     * Start the game, data loading will be done here
     */
    virtual void start() noexcept(false) = 0;

    /**
     * Prompt to the client that it should shutdown.
     * <strong>NOTE:</strong> this will not stop the client
     * immediately
     * @see <code>Client#shutdown()</code>
     */
    virtual void stop() noexcept(true) = 0;

    /**
     * Shutdown the client and perform data saving
     */
    virtual void shutdown() noexcept(false) = 0;

    /**
     * Get the pointer to the <code>Player</code> object
     * @return
     */
    [[nodiscard]] virtual Player *getPlayer() const noexcept(true) = 0;

    /**
     * Get the pointer to the <code>BattleHandler</code> object
     * @return
     */
    [[nodiscard]] virtual BattleHandler *getBattleHandler() const noexcept(true) = 0;

    /**
     * Get the pointer to the card template registry
     * @return
     */
    [[nodiscard]] virtual Registry<int, CardTemplate*> *getCardTemplates() const noexcept(true) = 0;

    /**
     * Get the pointer to the database
     * @return
     */
    [[nodiscard]] virtual Database *getDatabase() const noexcept(true) = 0;

    /**
     * Reset the <code>Player</code>'s progress. This function should both
     * reset the <code>Database</code> and run <code>initPlayer</code>
     * <br>
     * <strong>NOTE</strong> The previous player object will be destroyed and replaced with
     * a new object and it's corresponding pointer.
     */
    virtual void resetProgress() noexcept(false) = 0;


};

/**
 * Instantiate a new game client on the heap
 * @param rootDir The directory in which data is stored
 * @return Returns the pointer to the game client
 */
GameClient *newGameClient(const std::string &rootDir) noexcept(true);

#endif//GAME_H
