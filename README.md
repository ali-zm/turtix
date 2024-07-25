
# Turtix Game

This is a C++ game developed using the SFML library. It offers an interactive gameplay experience where you are to control a turtle trying to collect stars and diamonds and avoid having intersection with enemies.

## Prerequisites
To play turtix, you need the followings:

- c++ compiler
- SFML library



## Start playing
First clone the current repo: 

`git clone github.com/ali-zm/turtix.git`

Then change your directory into the game directory. And after that you need to run the makefile to build the game:

`make`

Now you should run the output file and pass the input folder into it:

`./turtix inputs`




## Game Description
This game has three different levels. In each a map will be loaded on your screen. you can use the following controls to navigate the turtle:
- Up arrow
- Right arrow
- Left arrow
Your mission is to free the the turtle's children by jumping on the head of them. Also you should be aware of the enemies. If you have any intersection with them from right or left side, you will loose of your lives. If you hit them from the top, they will die. Each of these two types of enemies have a particular feature:
- Casco: When their color turn into red, you will not be able to kill them!
- Bisko: They have two lives!



 


## Screenshots

![App Screenshot](https://github.com/ali-zm/turtix/blob/main/screenshots/1.png "Turtix")

![App Screenshot](https://github.com/ali-zm/turtix/blob/main/screenshots/2.png "Turtix")

## Contact

ali.zm.1383@gmail.com
