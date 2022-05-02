# Escape The Dungeon 

Group 83 </br>
Group Members: Davin Wu, Yeung Wong

# Introduction:
Turn-based combat card game. Player would control a prisoner in the dungeon, fighting against various monsters in order to escape from the dungeon. </br>
Players would start with 5 skill points, they can use them on either attack/defense/HP/spirit, each skill point brings a benefit of +1. </br>

# Features
All actions in the game are controlled through text-based commands through `std::input`. All output from the program will be from `std::output`
- Dynamic memory management will be used when dealing with user input (command parser using the `delete` operator)

## Pathing
There are a total of 3 paths with the same number of stations, randomly generated by the program. Players should choose one of them to start the game. There are 4 types of station, shop/campfire/Opportunity/Monster, and the player is able to see what station it is in each path.
- Data structure will be used to store the position of the player

## Battles
When stepping on a monster station, combat would be started. Players would draw 5 cards from the card library, and use them according to the card type. Those cards that haven’t been used would be reorganized into the library. Each card has its own cost of spirit, players start with 3 spirit each round (if no skill point is added to it).
- Randomness in monster generation, rewards generation
- Data structure (inventory) will be utilized
- Game state (inventory) will be serialized to the disk incrementally
- Dynamic memory management to clean up game state after each battle has been completed

## Boss
Player will encounter a boss at the end of the path. Victory in combat represents winning the game

## Shop
Players can buy new cards
- Game state (inventory) will be serialized to the disk

## Campfire
One skill point is added and the player is able to reallocate their skill points here

## Random events
A random incident would be generated, different choices are provided and the player is able to choose one of them. Each one would have its pros and cons.
- Randomness in incident generation






## Program process
- start (Background information, skill point allocation)
- Path generation, 3 lists with 10 nodes each
- Variable 'Current', move along each node in the path, from 0 to 9
- Generate event according to 'Current'
- Update status of player

## Program components
- header.h (Include all the global variables including status of player)
- player_status.cpp (Print player's status)
- start.cpp (Background information, skill point allocation)
- fight.cpp (Generate monster when called)
- campfire.cpp (Let player decide if heal HP or allocate one more skill point)
- shop.cpp (Provide new cards that player can buy)
- random_event.cpp (Provide a special effect to the player, either positive or negtaive)
- boss.cpp (Generate a boss, if boss defeated --> win, else --> lose)

