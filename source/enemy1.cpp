#if !defined(__ENEMY1__)
#define __ENEMY1__
#include "manual.hpp"
#include "objects.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void EnemyOne::draw(sf::RenderWindow *window)
{
    window->draw(sprite);
}

void EnemyOne::move(vector <Floor*> floors)
{
    int dx = (float)delta_x[this -> direction] * STEP_SIZE;
    this->sprite.move(sf::Vector2f(dx,0.f));
    this->coordinate.x = sprite.getPosition().x;
    make_coor_valid(floors);
    sprite.setPosition(this->coordinate.x,this->coordinate.y);
}

void EnemyOne::make_coor_valid(vector <Floor*> floors)
{
    if (this->coordinate.x < 0)
    {
        this->direction = Direction::RIGHT;
        this->coordinate.x = -this->coordinate.x;
    }
    if (this->coordinate.x >= WINDOW_HOR_SIZE)
    {
        this->direction = Direction::LEFT;
        this->coordinate.x = 2 * WINDOW_HOR_SIZE - this->coordinate.x;
    }
    if(reach_left_edge_of_floor(floors))
    {
        this->direction = Direction::RIGHT;
        this->coordinate.x += delta_x[Direction::RIGHT] * STEP_SIZE;
    }
    if(reach_right_edge_of_floor(floors))
    {
        this->direction = Direction::LEFT;
        this->coordinate.x += delta_x[Direction::LEFT] * STEP_SIZE;
    }
}

bool EnemyOne::reach_right_edge_of_floor(vector <Floor*> floors)
{
    float objectWidth = this->sprite.getGlobalBounds().width;
    for(auto floor:floors)
    {
        FloorData curFloorData = floor->get_fData();
        float endOfFloorCoor = curFloorData.coordinate.x+curFloorData.length;
        if(is_on_cur_floor(curFloorData))
        {
            if(endOfFloorCoor < this->coordinate.x + objectWidth)
                return true;
        }
    }
    return false;
}

bool EnemyOne::reach_left_edge_of_floor(vector <Floor*> floors)
{
    for(auto floor:floors)
    {
        FloorData curFloorData = floor->get_fData();
        if(is_on_cur_floor(curFloorData))
            if(curFloorData.coordinate.x > this->sprite.getPosition().x)
                return true;
    }
    return false;
}

bool EnemyOne::is_on_cur_floor(FloorData fData)
{
    float objectWidth = this->sprite.getGlobalBounds().width;
    float objectHeight = this->sprite.getGlobalBounds().height;
    float endOfFloorCoor = fData.coordinate.x+fData.length;
    float beginingOfFloorCoor = fData.coordinate.x;
    if(fData.coordinate.y == objectHeight + this->coordinate.y + 1)
        if((this->coordinate.x < endOfFloorCoor + objectWidth) && (this->coordinate.x > beginingOfFloorCoor - objectWidth))
            return true;
    return false;
}

#endif
