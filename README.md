# SkyrimKnapsackAlgorithm
 A dynamic programming algorithm applied to Bethesda's TESV: Skyrim.

A common dilemma for anyone playing Skyrim is they don't know what they should take with them, and what they should leave behind.  Having too many items in their inventory could mean they are overencumbered, and not able to fast travel.

While most people choose to follow the rule of only keeping things worth ten times their value, I figured that the Knapsack Algorithm is likely more efficient at getting the most out of an inventory.

This project is a proof of concept to show that the Knapsack Algorithm is an effective tool help players in Skyrim, as well as many other RPGs.

It could eventually be implemented as a plugin to the game, and is written in C++ to allow for that, but this would require a lot of reverse engineering that I don't have the skills for (yet).

The knapsack.cpp file contains all of the code used to run the algorithm, and as an external tool from the game, it also has code to read input from a players inventory, and cross-reference the data with the data.csv file.

data.csv is a list of most of the items in Skyrim that have a weight of over zero.  It was created by scraping the Skyrim Wiki, https://skyrim.fandom.com/wiki/Skyrim_Wiki, with the wikiscraper.py file, based on my previous python scraper project.  The database is not perfect, but ideally if this were implemented as a plugin would become unnecessary.

The input for knapsack.cpp is the output of using the in-game console command "player.showinventory".  A folder of sample inventories is included.