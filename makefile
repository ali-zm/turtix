turtix: main.o babies.o diamond.o enemy1.o enemy2.o floor.o game.o star.o turtle.o portal.o
	g++ main.o babies.o diamond.o enemy1.o enemy2.o floor.o game.o star.o turtle.o portal.o -o turtix -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

main: main.cpp
	g++ -c main.cpp
game: game.cpp
	g++ -c game.cpp
babies: babies.cpp
	g++ -c babies.cpp
diamond: diamond.cpp
	g++ -c diamond.cpp
enemies: enemy1.cpp enemy2.cpp
	g++ -c enemy1.cpp enemy2.cpp
floor: floor.cpp
	g++ -c floor.cpp
star: star.cpp
	g++ -c star.cpp
portal: portal.cpp
	g++ -c portal.cpp
turtle: turtle.cpp
	g++ -c turtle.cpp
clean:
	rm *.o