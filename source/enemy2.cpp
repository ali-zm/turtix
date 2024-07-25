#if !defined(__ENEMY2__)
#define __ENEMY2__
#include "manual.hpp"
#include "objects.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void EnemyTwo::draw(sf::RenderWindow *window)
{
    window->draw(sprite);
}

void EnemyTwo::move(vector <Floor*> floors,bool freezed)
{
    int dx = (float)delta_x[this -> direction] * STEP_SIZE;
    this->sprite.move(sf::Vector2f(dx,0.f));
    this->coordinate.x = sprite.getPosition().x;
    make_coor_valid(floors);
    change_color_if_freezed(freezed);
    sprite.setPosition(this->coordinate.x,this->coordinate.y);
}

void EnemyTwo::change_color_if_freezed(bool freezed)
{
    if(freezed && (sprite.getColor()==sf::Color::White))
    {
        sprite.setColor(sf::Color(233,127,127));
    }
    if(!freezed && (sprite.getColor()!=sf::Color::White))
    {
        sprite.setColor(sf::Color::White);
    }
}

void EnemyTwo::make_coor_valid(vector <Floor*> floors)
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

bool EnemyTwo::reach_right_edge_of_floor(vector <Floor*> floors)
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

bool EnemyTwo::reach_left_edge_of_floor(vector <Floor*> floors)
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

bool EnemyTwo::is_on_cur_floor(FloorData fData)
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
