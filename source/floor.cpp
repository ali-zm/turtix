#if !defined(__FLOOR__)
#define __FLOOR__
#include "manual.hpp"
#include "objects.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void Floor::draw(sf::RenderWindow *window)
{
    window->draw(shape);
}

#endif