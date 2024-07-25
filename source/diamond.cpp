#if !defined(__DIAMOND__)
#define __DIAMOND__
#include "manual.hpp"
#include "objects.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void Diamond::draw(sf::RenderWindow *window)
{
    window->draw(sprite);
}

#endif
