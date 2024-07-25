#if !defined(__OBJECTS__)
#define __OBJECTS__
#include "manual.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
using namespace std;

class Floor;

class Turtle
{
public:
    Turtle(Coordinate c) : coordinate(c)
    {
        if (!texture.loadFromFile("inputs/turtle.png"))
        {
            cerr << "No texture found!" << endl;
            abort();
        }
        sprite.setTexture(texture);
        sprite.setScale(TURTLE_SCALE, TURTLE_SCALE);
        sprite.setPosition(this->coordinate.x, this->coordinate.y);
        this->lives = TOTAL_AMOUNT_OF_TURTLE_LIVES;
        this->direction = Direction::DOWN;
    }
    bool get_jumping_status() { return isJumping; }
    void move();
    void draw(sf::RenderWindow *window);
    void set_floors(vector<Floor *> floorsToBeSet) { floors = floorsToBeSet; }
    void set_direction(Direction newD) { direction = newD; }
    sf::Sprite get_sprite() { return sprite; }
    sf::Vector2f get_position() { return sprite.getPosition(); }
    bool check_if_hits_the_wall(float corIndex);
    bool check_if_intersects_with_any_floor(int new_y);
    float analyze_if_hits_the_floor(float Y, float X);
    void reset_specs_to_defualt();
    void reflect_the_move();
    bool check_if_on_floor();
    void check_if_maximum_jumping_height_is_reached();
    bool check_if_hits_floor_by_the_side();
    void handle_collision_by_side();
    void implement_gravity();
    void change_lives(int delta) { lives += delta; }
    int get_lives() { return lives; }
    float get_ver_speed() { return curVerSpeed; }
    void set_sprite(float x, float y);

private:
    int lives;
    int jumpCount = 0;
    float curVerSpeed = 0;
    bool isJumping = true;
    bool isFalling = true;
    bool inPortal;
    Direction direction = Direction::DOWN;
    Coordinate coordinate;
    sf::Texture texture;
    sf::Sprite sprite;
    vector<Floor *> floors;
};

class BabyTurtle
{
public:
    BabyTurtle(Coordinate c) : coordinate(c)
    {
        if (!texture.loadFromFile("inputs/babyturtle.png"))
        {
            cerr << "No texture found!" << endl;
            abort();
        }
        sprite.setTexture(texture);
        sprite.setScale(OBJECT_SCALE, OBJECT_SCALE);
        this->locked = true;
        this->direction = Direction::RIGHT;
    }
    void set_y_of_coordinate(float y) { coordinate.y = y; }
    void set_x_of_coordinate(float x) { coordinate.x = x; }
    float get_height_of_sprite() { return sprite.getGlobalBounds().height; }
    bool get_locked() { return locked; }
    bool is_on_cur_floor(FloorData fData);
    bool is_freed(Turtle *t);
    void set_locked(bool l) { locked = l; }
    sf::Sprite get_sprite() { return sprite; }
    void set_texture(sf::Texture t) { texture = t; }
    void set_position() { sprite.setPosition(this->coordinate.x, this->coordinate.y); }
    void move(vector<Floor *> floors, sf::Sprite portal);
    void draw(sf::RenderWindow *window);
    void implement_gravity();
    float analyze_if_hits_the_floor(float upcomingCorY, float upComingCorX, vector<Floor *> floors);
    bool check_if_in_the_portal(sf::Sprite portal);
    bool check_portal_status() { return inPortal; }

private:
    Direction direction;
    Coordinate coordinate;
    bool locked;
    bool inPortal = false;
    float curVerSpeed = 0;
    bool isFalling = false;
    sf::Texture texture;
    sf::Sprite sprite;
    void make_coor_valid();
};

class EnemyOne
{
public:
    EnemyOne(Coordinate c) : coordinate(c)
    {
        if (!texture.loadFromFile("inputs/enemy1.png"))
        {
            cerr << "No texture found!" << endl;
            abort();
        }
        sprite.setTexture(texture);
        sprite.setScale(OBJECT_SCALE, OBJECT_SCALE);
        this->lives = TOTAL_AMOUNT_OF_ENEMY_LIVES;
        this->direction = Direction::RIGHT;
    }
    void move(vector<Floor *> floors);
    void set_y_of_coordinate(float y) { coordinate.y = y; }
    void set_x_of_coordinate(float x) { coordinate.x = x; }
    void set_position() { sprite.setPosition(this->coordinate.x, this->coordinate.y); }
    float get_height_of_sprite() { return sprite.getGlobalBounds().height; }
    void draw(sf::RenderWindow *window);
    sf::Sprite get_sprite() { return sprite; }
    float get_Y() { return coordinate.y; }
    void change_lives(int delta) { lives += delta; }
    int get_lives() { return lives; }
    void set_color(sf::Color color) { sprite.setColor(color); }

private:
    Direction direction;
    Coordinate coordinate;
    int lives;
    sf::Texture texture;
    sf::Sprite sprite;
    void make_coor_valid(vector<Floor *> floors);
    bool reach_right_edge_of_floor(vector<Floor *> floors);
    bool reach_left_edge_of_floor(vector<Floor *> floors);
    bool is_on_cur_floor(FloorData fd);
};

class EnemyTwo
{
public:
    EnemyTwo(Coordinate c) : coordinate(c)
    {
        if (!texture.loadFromFile("inputs/enemy2.png"))
        {
            cerr << "No texture found!" << endl;
            abort();
        }
        sprite.setTexture(texture);
        sprite.setScale(OBJECT_SCALE, OBJECT_SCALE);
        this->alive = true;
        this->locked = false;
        this->direction = Direction::RIGHT;
    }
    void set_y_of_coordinate(float y) { coordinate.y = y; }
    void set_x_of_coordinate(float x) { coordinate.x = x; }
    void set_position() { sprite.setPosition(this->coordinate.x, this->coordinate.y); }
    float get_height_of_sprite() { return sprite.getGlobalBounds().height; }
    void move(vector<Floor *> floors, bool freezed);
    void draw(sf::RenderWindow *window);
    sf::Sprite get_sprite() { return sprite; }
    float get_Y() { return coordinate.y; }

private:
    Direction direction;
    Coordinate coordinate;
    bool alive;
    bool locked;
    sf::Texture texture;
    sf::Sprite sprite;
    void make_coor_valid(vector<Floor *> floors);
    bool reach_right_edge_of_floor(vector<Floor *> floors);
    bool reach_left_edge_of_floor(vector<Floor *> floors);
    bool is_on_cur_floor(FloorData fd);
    void change_color_if_freezed(bool freezed);
};

class Star
{
public:
    Star(Coordinate c) : coordinate(c)
    {
        if (!texture.loadFromFile("inputs/star.png"))
        {
            cerr << "No texture found!" << endl;
            abort();
        }
        sprite.setTexture(texture);
        sprite.setPosition(this->coordinate.x, this->coordinate.y);
        sprite.setScale(OBJECT_SCALE, OBJECT_SCALE);
    }
    void draw(sf::RenderWindow *window);
    float get_height_of_sprite() { return sprite.getGlobalBounds().height; }
    sf::Sprite get_sprite() { return sprite; }

private:
    Coordinate coordinate;
    sf::Texture texture;
    sf::Sprite sprite;
};

class Diamond
{
public:
    Diamond(Coordinate c) : coordinate(c)
    {
        if (!texture.loadFromFile("inputs/diamond.png"))
        {
            cerr << "No texture found!" << endl;
            abort();
        }
        sprite.setTexture(texture);
        sprite.setPosition(this->coordinate.x, this->coordinate.y);
        sprite.setScale(OBJECT_SCALE, OBJECT_SCALE);
    }
    void draw(sf::RenderWindow *window);
    float get_height_of_sprite() { return sprite.getGlobalBounds().height; }
    sf::Sprite get_sprite() { return sprite; }

private:
    Coordinate coordinate;
    sf::Texture texture;
    sf::Sprite sprite;
};

class Portal
{
public:
    Portal(Coordinate c) : coordinate(c)
    {
        if (!texture.loadFromFile("inputs/portal.png"))
        {
            cerr << "No texture found!" << endl;
            abort();
        }
        sprite.setTexture(texture);
        sprite.setPosition(this->coordinate.x, this->coordinate.y);
        sprite.setScale(WINDOW_ASPECT_SCALE, WINDOW_ASPECT_SCALE);
    }
    void draw(sf::RenderWindow *window);
    float get_height_of_sprite() { return sprite.getGlobalBounds().height; }
    float get_width() { return sprite.getGlobalBounds().width; }
    void increase_babies_went_through() { wentThroughBabies++; }
    int get_babies_count() { return wentThroughBabies; }
    Coordinate get_cor() { return coordinate; }
    sf::Sprite get_sprite() { return sprite; }

private:
    Coordinate coordinate;
    sf::Texture texture;
    sf::Sprite sprite;
    int wentThroughBabies = 0;
};

class Floor
{
public:
    Floor(FloorData f) : floorData(f)
    {
        shape.setSize(sf::Vector2f((const float)floorData.length, 20.f));
        shape.setFillColor(brown);
        shape.setPosition(floorData.coordinate.x, floorData.coordinate.y);
    }
    FloorData get_fData() { return floorData; }
    void draw(sf::RenderWindow *window);
    float get_vertical_coordinate() { return shape.getPosition().y; }
    sf::RectangleShape get_shape() { return shape; }

private:
    FloorData floorData;
    sf::RectangleShape shape;
};

#endif