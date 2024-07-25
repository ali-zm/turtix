#if !defined(__MANUAL__)
#define __MANUAL__

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/Audio.hpp>

using namespace std;

enum Direction
{
    LEFT,
    UP,
    RIGHT,
    DOWN,
    NONE
};
struct Coordinate
{
    float x;
    float y;
};
struct FloorData
{
    Coordinate coordinate;
    int length;
};

const float INITIAL_HOR_SPEED = 10.0f;
const float INITIAL_VER_SPEED = -5.0f;
const float INITIAL_HOR_SPEED_WHILE_IN_AIR = 50.0f;
const float SIDE_COLLISION_SPEED = 5.0f;
const float GRAVITY = 0.0403;
const float WINDOW_ASPECT_SCALE = 2.5f;
const float BACKGROUND_SCALE = 2.0f;
const float OBJECT_SCALE = 1.8f;
const float TURTLE_SCALE = 1.8f;
const float VIEW_WIDTH = 1000;
const float VIEW_HEIGHT = 700;
const float ZOOM_SCALE = 2.0f;
const int delta_x[5] = {-1, 0, 1, 0, 0};
const int delta_y[5] = {0, -1, 0, 1, 0};
const int NOT_HIITTING_INDEX = -1;
const int WINDOW_HOR_SIZE = 2500;
const int WINDOW_VER_SIZE = 1750;
const int STEP_SIZE = 5;
const int NO_FLOOR_UNDER_INDEX = -1;
const int NUM_OF_ROUNDS = 3;
const int FREEZE_TIME = 10000;
const int HIT_FORBIDDEN_TIME = 1000;
const int UNFREEZE_TIME = 15000;
const int TOTAL_AMOUNT_OF_TURTLE_LIVES = 3;
const int TOTAL_AMOUNT_OF_ENEMY_LIVES = 2;
const int STAR_BONUS = 5;
const int DIAMOND_BONUS = 10;
const int MAXIMUM_JUMP_COUNT = 350 / (-INITIAL_VER_SPEED);
const sf::Color brown(150, 75, 0);
const sf::Color grey(80, 80, 80);
const sf::Color paleGreen(193, 253, 193);

const string MAPS_ADD[3] = {"/Level1.csv", "/Level2.csv", "/Level3.csv"};

#endif