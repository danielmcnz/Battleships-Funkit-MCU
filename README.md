# Battleships - Funkit Project
## Daniel Mcgregor & Chester Hall-Fernandez
### 19/10/22


This is a 2 player game using the UC Funkit development board. It aims to accurately represent the popular board game 'battleships'.  

Upon starting the game, the words 'battleships' scroll across the screen. To begin a player presses the navswitch. The ship maps get randomly assigned based of an internal library, and the player who pressed the button begins on attack. 


### Attack
On attach, there are two screens, which can be cycled between using the standalone button. The first screen is 'your guesses', which shows you where you have fired a shot. There is a blinking 'cursor', which shows you where you currently are aimed, which can be controlled using navigation switch up/down/left/right A bright LED indicates a hit, a faint LED indicates a miss and no LED indicates that there has been no guess. To fire a shot, the player presses down the navigation switch. 

The second screen shows the enemy guesses. As above, a bright LED indicates a hit, a faint LED indicates a miss and no LED indicates that there has been no guess. However there is no cursor.

To end your attack round, you fire a shot, in there 'your guesses' screen and the player roles get switched. 


### Defend 
Like attack, defend has two 'screens'. The first shows your randomly assigned ships, this is what you first see when you get set to defend. The second, like attack shows enemy guesses. The purpose of the defend stage is to wait for the opponents turn, and also review how well the enemy is doing against you. The defend phase ends when it detects an incoming shot from the enemy board. 

### Winning 
The game is won when one player has destroyed all another players boats. This is done by correctly guessing all the players boat positions. It does not matter how many misses there are. There are a total of 10 pixels assigned to boats (1 x 4, 1 x 3, 2 x 2). Therefore it takes a minimum of 10 attack turns per player to end a game. 

### Alterations
The player maps are stored under the 'assignment/group_606/starting_positions.h' header file. This can easily be altered to add more or less maps and change how many boats are present. 
