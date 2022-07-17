# CGSynch
_A program for analysing combinatorial games and synchronized games._

This application was written for my bachelor thesis, and can be used to analyze [combinatorial](http://en.wikipedia.org/wiki/Combinatorial_game_theory) and synchronized games.
Combinatorial games are two-player deterministic games in which both players have perfect information and take turns making move, until either player is unable to and thus loses.
Synchronized games are two-players deterministic games where both players must simultaneously choose a move, which is then preformed simultaneously.
As a result, the players no longer have perfect information of the game state, which introduces additional complexity in analyzing these games.

A link to my thesis, which goes much more in-depth on both the math behind combinatorial and synchronized game theory as well as how this program works, will be posted here when it is published on the website of Leiden University.

This program supports entering games using numbers and by providing its left and right option sets (for combinatorial games). Additionally, positions from the games Hackenbush, Push, Shove, Cherries and Stack Cherries can be analyzed both in combinatorial and synchronized sense.
If you want to extend this program to analyze a different game, instructions can be found [in this pdf](Extending%20CGSynch%20for%20your%20own%20rulesets.pdf).
