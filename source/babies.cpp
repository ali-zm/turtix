#if !defined(__BABIES__)
#define __BABIES__
#include "manual.hpp"
#include "objects.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void BabyTurtle::draw(sf::RenderWindow *window)
{
    window->draw(sprite);
}

void BabyTurtle::implement_gravity()
{
    curVerSpeed += GRAVITY;
}

bool BabyTurtle::check_if_in_the_portal(sf::Sprite portal)
{
    return sprite.getGlobalBounds().intersects(portal.getGlobalBounds());
}

float BabyTurtle::analyze_if_hits_the_floor(float upcomingCorY, float upComingCorX, vector<Floor *> floors)
{
    float nextCorY = upcomingCorY + sprite.getGlobalBounds().height;
    float corY = coordinate.y + sprite.getGlobalBounds().height;
    for (int index = 0; index < floors.size(); ++index)
    {
        if (upComingCorX >= floors[index]->get_shape().getPosition().x - this->sprite.getGlobalBounds().width && upComingCorX <= floors[index]->get_shape().getPosition().x + floors[index]->get_shape().getGlobalBounds().width && corY < floors[index]->get_shape().getPosition().y && nextCorY >= floors[index]->get_shape().getPosition().y)
            return floors[index]->get_shape().getPosition().y - 1 - corY;
    }
    return NOT_HIITTING_INDEX;
}

void BabyTurtle::move(vector<Floor *> floors, sf::Sprite portal)
{
    for (auto floor : floors)
    {
        if (is_on_cur_floor(floor->get_fData()) && !isFalling)
        {
            int dx = (float)delta_x[this->direction] * STEP_SIZE;
            this->sprite.move(sf::Vector2f(dx, 0.f));
            this->coordinate.x = sprite.getPosition().x;
            make_coor_valid();
            sprite.setPosition(this->coordinate.x, this->coordinate.y);
        }
        else
        {
            implement_gravity();
            isFalling = true;
            if (analyze_if_hits_the_floor(coordinate.y + curVerSpeed, coordinate.x, floors) == NOT_HIITTING_INDEX)
            {
                sprite.move(0.0f, curVerSpeed);
                coordinate.y = sprite.getPosition().y;
            }
            else
            {
                float distanceLeft = analyze_if_hits_the_floor(coordinate.y + curVerSpeed, coordinate.x, floors);
                sprite.move(0.0f, distanceLeft);
                coordinate.y = sprite.getPosition().y;
                isFalling = false;
            }
        }
        if (check_if_in_the_portal(portal))
        {
            inPortal = true;
        }
    }
}

bool BabyTurtle::is_on_cur_floor(FloorData fData)
{
    float objectWidth = this->sprite.getGlobalBounds().width;
    float objectHeight = this->sprite.getGlobalBounds().height;
    float endOfFloorCoor = fData.coordinate.x + fData.length;
    float beginingOfFloorCoor = fData.coordinate.x;
    if (fData.coordinate.y == objectHeight + this->coordinate.y + 1)
        if ((this->coordinate.x < endOfFloorCoor + objectWidth) && (this->coordinate.x > beginingOfFloorCoor - objectWidth))
            return true;
    return false;
}

void BabyTurtle::make_coor_valid()
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
}

bool BabyTurtle::is_freed(Turtle *turtle)
{
    float turtleHeight = turtle->get_sprite().getGlobalBounds().height;
    float turtleY = turtle->get_sprite().getPosition().y;
    if (turtle->get_sprite().getGlobalBounds().intersects(this->sprite.getGlobalBounds()))
        if (this->get_sprite().getPosition().y >= turtleY + turtleHeight - turtle->get_ver_speed())
            return true;
    return false;
}

#endif
