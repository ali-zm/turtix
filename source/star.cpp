#if !defined(__STAR__)
#define __SATR__
#include "manual.hpp"
#include "objects.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void Star::draw(sf::RenderWindow *window)
{
    window->draw(sprite);
}

#endif
