# CS498-FinalProject
This is a closed world 2.5D survival game. The player is spawned on an unexplored island and he needs to survive. There are many different resources on the map, including minerals, plants. Player needs to collect them. There are also wild animals in certain areas, and players can defeat them to get meat and other materials. The game has a hunger system, so the player needs to eat food to avoid being in a hungry state, otherwise HP will slowly drop. The game has a crafting system, and players can use collections to make different weapons, tools, consumables, so they can defeat stronger enemies and increase the efficiency of collecting resources. Consumables can add buffs to the player or recover HP.

The world has day and night, and they have different influences on the player. At night, the vision is limited, and certain types of enemies are spawned. These enemies will actively attack the player, and they will become stronger and stronger as the number of days increases. These enemies disappear in the daytime. Killing these enemies also gains loot, and they can be used to build better weapons. Therefore, it is important for players to keep their weapon updated so they can counter stronger enemies. After a certain number of days, a final boss is spawned. Player needs to kill it to pass the game. 

Change:
Remove the crafting system and tool system. 
Simplify the collecting system. Now materials can be collected with one button pressed instead of requiring specific tools. Fewer kinds of resources. (May be completely removed based on further implementation)
Simplify weapon system. Player has one gun and one knife when the game starts. Players can upgrade weapons by consuming specific materials to increase ATK, attack range, etc. (Can be easily applied by changing some attributes, instead of making different new weapons.)
Consumables are dropped when players kill animals and enemies. No need for crafting them now. 
Simplify Bag UI. Only need to show the numbers of items, no need to interact now. 
