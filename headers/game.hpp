#if !defined(__GAME__)
#define __GAME__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "manual.hpp"
#include "objects.hpp"

using namespace std;

class Game
{
public:
    Game(vector<FloorData> &floorData, Coordinate &portalCor,
         vector<Coordinate> &babyTurtlesCor, vector<Coordinate> &starsCor,
         vector<Coordinate> &diamondCor, vector<Coordinate> &enemyOneCor,
         vector<Coordinate> &enemyTwoCor, int indexRound);

    void start(int round, bool &gameOverFlag);
    void set_view();
    float set_coordinate_due_to_nearest_floor(Coordinate coordinate);
    bool turtle_is_dead() { return turtle->get_lives() == 0; }
    bool won_the_round();
    void check_if_won_the_round(int round);
    void tick(bool freezing);
    void draw();
    void check_if_baby_must_come_free();
    void check_if_star_collected();
    void check_if_diamond_collected();
    void intersection_with_enemy1(sf::Clock &intersection, bool &hitForbidden);
    void intersection_with_enemy2(bool isFreezed);
    void check_if_baby_went_into_portal();
    int get_score() { return score; }

private:
    void create_background_sprite();
    void handle_events(sf::Event event);
    void hadle_pressed_key(sf::Keyboard::Key);
    sf::RenderWindow *window;
    sf::Sprite backgroundSprite;
    int score = 0;
    int babiesInPortal = 0;
    int initialBabiesCount;
    bool roundLost = false;
    bool roundWon = false;
    sf::View view;
    Turtle *turtle;
    vector<BabyTurtle *> babies;
    vector<EnemyOne *> enemyOnes;
    vector<EnemyTwo *> enemyTwos;
    Portal *portal;
    vector<Star *> stars;
    vector<Diamond *> diamonds;
    vector<Floor *> floors;
};

#endif