#if !defined(__PORTAL__)
#define __PORTAL__
#include "manual.hpp"
#include "objects.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void Portal::draw(sf::RenderWindow *window)
{
    window->draw(sprite);
}

#endif
