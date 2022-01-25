# Block-Dodger
Link to video demostration of the game:


https://www.youtube.com/watch?v=eGWgO8izN5U

# Project Description

My custom lab project is a game called Block Dodger. This game is represented by an 8x8 LED matrix that uses three shift registers to output rows of red LED sequences that the user will have to dodge. These rows will shift downward or towards a player at a constant speed throughout a round. The user will be placed on the bottom row of the LED matrix and will be represented by a single blue LED. The user will be able to move left and right by using a joystick. Throughout the game, a Nokia 5110 LCD will display certain pixels that will create different types of animations. Such include a check mark when the user passes a round/level, a skull when the user fails to avoid/dodge a block (red LED), a text that displays what level is currently being played, and a button symbol that tells the user to press the button so the game can start/restart. The game will consist of four total rounds and all need to be passed to beat the game. 

# User Guide
Rules: The player must avoid every single red LED sequence in a round in order to pass the round. The player must complete all four rounds successfully in order to beat the game. If the player fails to avoid a block during the game, the player will lose and all progress will be reset. The player will lose the game if a block hits their blue LED as it is shifting downwards or if the player moves left or right and makes contact with a block. 

# Controls

The player will have the press a button when indicated by the Nokia 5110 LCD screen in order to start or restart the game. The player will also have control over a joystick that can be moved left or right to move their player (blue LED) on the board. 

# Source File Description

•	File 1 – main.c: This c file contains all the code that makes my game run such as all the Tick () functions and the main function.

•	File 2 – timer.h: This file contains the functions needed that allows for the timer to work and make sure all my time periods work.

•	File 3 – Nokia.h: This file contains functions that will be used in the c file so that my Nokia 5110 LCD can output certain displays on the screen anytime I call a function in this file. 

