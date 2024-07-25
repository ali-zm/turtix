#if !defined(__FROG__)
#define __FROG__
#include "objects.hpp"
#include "manual.hpp"

using namespace std;

struct FloorInfo
{
    float _x;
    float _y;
    float width;
    float height;
};

void Turtle::draw(sf::RenderWindow *window)
{
    window->draw(sprite);
}

bool Turtle::check_if_hits_the_wall(float new_x)
{
    if (new_x >= 0 && new_x <= WINDOW_HOR_SIZE)
        return true;
    return false;
}

bool Turtle::check_if_intersects_with_any_floor(int new_y)
{
    float prevY = sprite.getPosition().y;
    sprite.setPosition(sprite.getPosition().x, new_y);
    for (auto floor : floors)
        if (sprite.getGlobalBounds().intersects(floor->get_shape().getGlobalBounds()))
        {
            sprite.setPosition(sprite.getPosition().x, prevY);
            return false;
        }
    sprite.setPosition(sprite.getPosition().x, prevY);
    return true;
}

void Turtle::check_if_maximum_jumping_height_is_reached()
{
    if (curVerSpeed >= 0)
    {
        isFalling = true;
        direction = Direction::DOWN;
    }
}

void Turtle::implement_gravity()
{
    curVerSpeed += GRAVITY;
}

bool Turtle::check_if_on_floor()
{
    for (auto floor : floors)
        if (sprite.getPosition().x >= floor->get_shape().getPosition().x - this->sprite.getGlobalBounds().width && sprite.getPosition().x <= floor->get_shape().getPosition().x + floor->get_shape().getGlobalBounds().width && sprite.getPosition().y + sprite.getGlobalBounds().height == floor->get_shape().getPosition().y - 1)
            return true;
    return false;
}

float Turtle::analyze_if_hits_the_floor(float upcomingCorY, float upComingCorX)
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

void Turtle::reset_specs_to_defualt()
{
    isJumping = false;
    isFalling = false;
    direction = Direction::NONE;
    curVerSpeed = INITIAL_VER_SPEED;
}

float multiply_by_negative(float speed)
{
    return (-1) * speed;
}

void Turtle::reflect_the_move()
{
    direction = Direction::DOWN;
    curVerSpeed = 0;
}

FloorInfo store_floor_info(Floor *floor)
{
    FloorInfo floorInfo;
    floorInfo._x = floor->get_shape().getPosition().x;
    floorInfo._y = floor->get_shape().getPosition().y;
    floorInfo.width = floor->get_shape().getGlobalBounds().width;
    floorInfo.height = floor->get_shape().getGlobalBounds().height;
    return floorInfo;
}

void Turtle::handle_collision_by_side()
{
    if (direction == Direction::RIGHT)
    {
        sprite.move(-SIDE_COLLISION_SPEED, 0.0f);
        coordinate.x = sprite.getPosition().x;
    }
    if (direction == Direction::LEFT)
    {
        sprite.move(SIDE_COLLISION_SPEED, 0.0f);
        coordinate.x = sprite.getPosition().x;
    }
    direction = Direction::DOWN;
}

bool Turtle::check_if_hits_floor_by_the_side()
{
    Floor *possibleToHit = NULL;
    for (auto floor : floors)
    {
        FloorInfo floorInfo = store_floor_info(floor);
        if (coordinate.y >= floorInfo._y - sprite.getGlobalBounds().height && coordinate.y <= floorInfo._y + floorInfo.height)
        {
            possibleToHit = floor;
            break;
        }
    }
    if (possibleToHit != NULL)
    {
        FloorInfo foundFloor = store_floor_info(possibleToHit);
        if (direction == Direction::RIGHT && coordinate.x + INITIAL_HOR_SPEED_WHILE_IN_AIR >= foundFloor._x)
        {
            return false;
        }
        if (direction == Direction::LEFT && coordinate.x - INITIAL_HOR_SPEED_WHILE_IN_AIR <= foundFloor._x + foundFloor.width)
        {
            return false;
        }
    }
    return true;
}

void Turtle::move()
{
    if (!isJumping)
    {
        if (direction == Direction::LEFT && check_if_hits_the_wall(coordinate.x - INITIAL_HOR_SPEED))
        {
            sprite.move(-INITIAL_HOR_SPEED, 0.0f);
            coordinate.x = sprite.getPosition().x;
            if (check_if_on_floor())
                direction = Direction::NONE;
            else
            {
                direction = Direction::DOWN;
                isJumping = true;
                isFalling = true;
                curVerSpeed = 0;
            }
        }
        if (direction == Direction::RIGHT && check_if_hits_the_wall(coordinate.x + sprite.getGlobalBounds().width + INITIAL_HOR_SPEED))
        {
            sprite.move(INITIAL_HOR_SPEED, 0.0f);
            coordinate.x = sprite.getPosition().x;
            if (check_if_on_floor())
            {
                direction = Direction::NONE;
            }
            else
            {
                direction = Direction::DOWN;
                isJumping = true;
                isFalling = true;
                curVerSpeed = 0;
            }
        }
        if (direction == Direction::UP && check_if_intersects_with_any_floor(coordinate.y + INITIAL_VER_SPEED))
        {
            sprite.move(0.0f, INITIAL_VER_SPEED);
            coordinate.y = sprite.getPosition().y;
            isJumping = true;
        }
    }
    else
    {
        implement_gravity();
        if (direction == Direction::UP)
        {
            if (check_if_intersects_with_any_floor(coordinate.y + INITIAL_VER_SPEED))
            {
                sprite.move(0.0f, curVerSpeed);
                coordinate.y = sprite.getPosition().y;
                check_if_maximum_jumping_height_is_reached();
            }
            if (!check_if_intersects_with_any_floor(coordinate.y + INITIAL_VER_SPEED))
            {
                coordinate.y = sprite.getPosition().y;
                reflect_the_move();
            }
        }
        if (direction == Direction::RIGHT)
        {
            if (check_if_hits_floor_by_the_side() && check_if_hits_the_wall(coordinate.x + sprite.getGlobalBounds().width + INITIAL_HOR_SPEED))
            {
                sprite.move(INITIAL_HOR_SPEED_WHILE_IN_AIR, 0.0f);
                coordinate.x = sprite.getPosition().x;
                if (isFalling)
                    direction = Direction::DOWN;
                else
                    direction = Direction::UP;
            }
            else
                handle_collision_by_side();
        }
        if (direction == Direction::LEFT)
        {
            if (check_if_hits_floor_by_the_side() && check_if_hits_the_wall(coordinate.x - INITIAL_HOR_SPEED))
            {
                sprite.move(-INITIAL_HOR_SPEED_WHILE_IN_AIR, 0.0f);
                coordinate.x = sprite.getPosition().x;
                if (isFalling)
                    direction = Direction::DOWN;
                else
                    direction = Direction::UP;
            }
            else
                handle_collision_by_side();
        }
        if (direction == Direction::DOWN)
        {
            if (analyze_if_hits_the_floor(coordinate.y + curVerSpeed, coordinate.x) == NOT_HIITTING_INDEX)
            {
                sprite.move(0.0f, curVerSpeed);
                coordinate.y = sprite.getPosition().y;
            }
            else
            {
                float distanceLeft = analyze_if_hits_the_floor(coordinate.y + curVerSpeed, coordinate.x);
                sprite.move(0.0f, distanceLeft);
                coordinate.y = sprite.getPosition().y;
                reset_specs_to_defualt();
            }
        }
    }
}

void Turtle::set_sprite(float x, float y)
{
    sprite.setPosition(x, y);
    coordinate.x = x;
    coordinate.y = y;
}

#endif