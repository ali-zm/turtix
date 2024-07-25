#if !defined(__GAMECODE__)
#define __GAMECODE__

#include <string>
#include "objects.hpp"
#include "game.hpp"
#include "manual.hpp"

using namespace std;

Game::Game(vector<FloorData> &floorData, Coordinate &portalCor,
           vector<Coordinate> &babyTurtlesCor, vector<Coordinate> &starsCor,
           vector<Coordinate> &diamondCor, vector<Coordinate> &enemyOneCor,
           vector<Coordinate> &enemyTwoCor, int indexRound)
{
    initialBabiesCount = babyTurtlesCor.size();
    portal = new Portal(portalCor);
    Coordinate turtleCor = {portalCor.x + (portal->get_width() / 2), portalCor.y};
    turtle = new Turtle(turtleCor);
    for (int i = 0; i < floorData.size(); i++)
    {
        Floor *floor = new Floor(floorData[i]);
        floors.push_back(floor);
    }
    for (int i = 0; i < babyTurtlesCor.size(); i++)
    {
        BabyTurtle *baby = new BabyTurtle(babyTurtlesCor[i]);
        baby->set_y_of_coordinate(set_coordinate_due_to_nearest_floor(babyTurtlesCor[i]) - baby->get_height_of_sprite());
        baby->set_position();
        babies.push_back(baby);
    }
    for (int i = 0; i < starsCor.size(); i++)
    {
        Star *star = new Star(starsCor[i]);
        stars.push_back(star);
    }
    for (int i = 0; i < diamondCor.size(); i++)
    {
        Diamond *diamond = new Diamond(diamondCor[i]);
        diamonds.push_back(diamond);
    }
    for (int i = 0; i < enemyOneCor.size(); i++)
    {
        EnemyOne *en1 = new EnemyOne(enemyOneCor[i]);
        en1->set_y_of_coordinate(set_coordinate_due_to_nearest_floor(enemyOneCor[i]) - en1->get_height_of_sprite());
        en1->set_position();
        enemyOnes.push_back(en1);
    }
    for (int i = 0; i < enemyTwoCor.size(); i++)
    {
        EnemyTwo *en2 = new EnemyTwo(enemyTwoCor[i]);
        en2->set_y_of_coordinate(set_coordinate_due_to_nearest_floor(enemyTwoCor[i]) - en2->get_height_of_sprite());
        en2->set_position();
        enemyTwos.push_back(en2);
    }
}

void Game::check_if_won_the_round(int round)
{
    if (babiesInPortal == initialBabiesCount && turtle->get_sprite().getGlobalBounds().intersects(portal->get_sprite().getGlobalBounds()))
    {
        roundWon = true;
        cout << "Congrats! Level " << round + 1 << " passed!" << endl;
    }
}

float Game::set_coordinate_due_to_nearest_floor(Coordinate coordinate)
{
    for (auto floor : floors)
    {
        if (floor->get_vertical_coordinate() > coordinate.y)
        {
            return coordinate.y = floor->get_vertical_coordinate() - 1;
        }
    }
    return NO_FLOOR_UNDER_INDEX;
}

void Game::set_view()
{
    sf::Vector2f player_pos = turtle->get_position();
    sf::Vector2f view_center = view.getCenter();
    bool changedFlag = false;
    view.zoom(1 / ZOOM_SCALE);
    if (player_pos.y - (VIEW_HEIGHT / 2) < 0) // check to see if viewport y is less than 0 on y axis
    {
        view_center.y = VIEW_HEIGHT / 2;
        view_center.x = player_pos.x;
        view.setCenter(view_center);
        changedFlag = true;
    }

    if (player_pos.y + (VIEW_HEIGHT / 2) > WINDOW_VER_SIZE) // check to see if viewport y is greater than height.
    {
        view_center.y = WINDOW_VER_SIZE - (VIEW_HEIGHT / 2);
        view_center.x = player_pos.x;
        view.setCenter(view_center);
        changedFlag = true;
    }

    if (player_pos.x - (VIEW_WIDTH / 2) < 0) // check to see if viewport  x will be less than 0 on x axis
    {
        view_center.x = VIEW_WIDTH / 2;
        view_center.y = player_pos.y;
        view.setCenter(view_center);
        changedFlag = true;
    }

    if (player_pos.x + (VIEW_WIDTH / 2) > WINDOW_HOR_SIZE) // check to see if viewport x is greater than map size 2115
    {
        cout << "here";
        view_center.x = WINDOW_HOR_SIZE - (VIEW_WIDTH / 2);
        view_center.y = player_pos.y;
        view.setCenter(view_center);
        changedFlag = true;
    }
    
    if (!changedFlag)
    {
        view.setCenter(turtle->get_position());
    }
    view.zoom(ZOOM_SCALE);
}

void Game::check_if_baby_went_into_portal()
{
    for (int index = 0; index < babies.size(); ++index)
    {
        if (babies[index]->check_portal_status())
        {
            delete babies[index];
            babies.erase(babies.begin() + index);
            cout << "A baby was savd and went in to the portal!" << endl;
            babiesInPortal++;
        }
    }
}

void Game::start(int round, bool &gameOverFlag)
{
    string windowName = "LEVEL " + to_string(round + 1);
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_HOR_SIZE, WINDOW_VER_SIZE), windowName);
    window->setFramerateLimit(180);
    sf::Clock ticktok;
    sf::Clock freezeEnemyClock;
    sf::Clock intersectionClock;
    sf::Texture background;
    background.loadFromFile("inputs/jungle.png");
    backgroundSprite.setTexture(background);
    backgroundSprite.setScale(WINDOW_HOR_SIZE / backgroundSprite.getGlobalBounds().width, WINDOW_VER_SIZE / backgroundSprite.getGlobalBounds().height);
    bool IsFreezed = false;
    bool hitForbidden = false;
    turtle->set_floors(floors);
    view.setCenter(turtle->get_position());
    view.setSize(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));
    view.zoom(ZOOM_SCALE);
    while (this->window->isOpen() && roundWon == false)
    {
        // set_view();
        sf::Event event;
        while (this->window->pollEvent(event))
        {
            this->handle_events(event);
        }
        if (this->turtle_is_dead())
        {
            gameOverFlag = true;
            cout << "GAME OVER!" << endl;
            break;
        }
        turtle->move();
        check_if_baby_must_come_free();
        check_if_star_collected();
        check_if_diamond_collected();
        intersection_with_enemy1(intersectionClock, hitForbidden);
        intersection_with_enemy2(IsFreezed);
        if (ticktok.getElapsedTime() >= sf::milliseconds(50))
        {
            if (IsFreezed != true && freezeEnemyClock.getElapsedTime() >= sf::milliseconds(FREEZE_TIME) && freezeEnemyClock.getElapsedTime() < sf::milliseconds(UNFREEZE_TIME))
                IsFreezed = true;
            if (IsFreezed != false && freezeEnemyClock.getElapsedTime() >= sf::milliseconds(UNFREEZE_TIME))
            {
                freezeEnemyClock.restart();
                IsFreezed = false;
            }
            if (hitForbidden && (intersectionClock.getElapsedTime() >= sf::milliseconds(HIT_FORBIDDEN_TIME)))
            {
                hitForbidden = false;
            }
            this->tick(IsFreezed);
            ticktok.restart();
        }
        check_if_baby_went_into_portal();
        check_if_won_the_round(round);
        if (roundWon == true)
            window->close();
        this->draw();
        view.setCenter(turtle->get_position());
        window->setView(view);
        window->display();
    }
}

void Game::draw()
{
    this->window->clear(paleGreen);
    window->draw(backgroundSprite);
    portal->draw(window);
    turtle->draw(window);
    for (auto baby : babies)
        baby->draw(this->window);
    for (auto diamond : diamonds)
        diamond->draw(this->window);
    for (auto star : stars)
        star->draw(this->window);
    for (auto enemy1 : enemyOnes)
        enemy1->draw(this->window);
    for (auto enemy2 : enemyTwos)
        enemy2->draw(this->window);
    for (auto floor : floors)
        floor->draw(this->window);
}

void Game::hadle_pressed_key(sf::Keyboard::Key keyPressed)
{
    if (keyPressed == sf::Keyboard::Left)
        turtle->set_direction(Direction::LEFT);

    if (keyPressed == sf::Keyboard::Right)
        turtle->set_direction(Direction::RIGHT);

    if (keyPressed == sf::Keyboard::Up)
        turtle->set_direction(Direction::UP);
}

void Game::handle_events(sf::Event event)
{
    if (event.type == sf::Event::Closed)
        window->close();

    if (event.type == sf::Event::KeyPressed)
        hadle_pressed_key(event.key.code);
}

void Game::tick(bool freezed)
{
    for (auto enemy1 : enemyOnes)
        enemy1->move(floors);
    for (auto enemy2 : enemyTwos)
        enemy2->move(floors, freezed);
    for (auto baby : babies)
        if (!baby->get_locked())
            baby->move(floors, portal->get_sprite());
}

void Game::check_if_baby_must_come_free()
{
    for (auto baby : babies)
    {
        if (baby->is_freed(turtle))
        {
            baby->set_locked(false);
            sf::Texture newTexture;
            if (!newTexture.loadFromFile("inputs/newturtle.png"))
            {
                cerr << "No texture found!" << endl;
                abort();
            }
            baby->set_texture(newTexture);
            baby->get_sprite().setTexture(newTexture);
            break;
        }
    }
}

void Game::check_if_star_collected()
{
    for (int i = 0; i < stars.size(); i++)
    {
        if (turtle->get_sprite().getGlobalBounds().intersects(stars[i]->get_sprite().getGlobalBounds()))
        {
            this->score += STAR_BONUS;
            delete stars[i];
            cout << "You earned a Star! (+" << STAR_BONUS << ")" << endl;
            stars.erase(stars.begin() + i);
            return;
        }
    }
}

void Game::check_if_diamond_collected()
{
    for (int i = 0; i < diamonds.size(); i++)
    {
        if (turtle->get_sprite().getGlobalBounds().intersects(diamonds[i]->get_sprite().getGlobalBounds()))
        {
            this->score += DIAMOND_BONUS;
            delete diamonds[i];
            diamonds.erase(diamonds.begin() + i);
            cout << "You earned a Diamond! (+" << DIAMOND_BONUS << ")" << endl;
            return;
        }
    }
}

void Game::intersection_with_enemy1(sf::Clock &intersection, bool &hitForbidden)
{
    float turtleHeight = turtle->get_sprite().getGlobalBounds().height;
    float turtleY = turtle->get_sprite().getPosition().y;
    if (!hitForbidden)
    {
        for (int index = 0; index < enemyOnes.size(); ++index)
        {
            if (turtle->get_sprite().getGlobalBounds().intersects(enemyOnes[index]->get_sprite().getGlobalBounds()))
            {
                if (enemyOnes[index]->get_Y() >= turtleY + turtleHeight - turtle->get_ver_speed())
                {
                    hitForbidden = true;
                    intersection.restart();
                    enemyOnes[index]->change_lives(-1);
                    if (enemyOnes[index]->get_lives() == 0)
                    {
                        delete enemyOnes[index];
                        enemyOnes.erase(enemyOnes.begin() + index);
                        return;
                    }
                    enemyOnes[index]->set_color(grey);
                }
                else
                {
                    turtle->change_lives(-1);
                    cout << "Watchout! An Enemy just hit you!" << endl;
                    cout << "Lives Left: " << turtle->get_lives() << "!" << endl;
                    if (turtle->get_lives() == 0)
                        roundLost = true;
                    else
                        turtle->set_sprite(portal->get_cor().x, portal->get_cor().y);
                    return;
                }
            }
        }
    }
}

void Game::intersection_with_enemy2(bool isFreezed)
{
    float turtleHeight = turtle->get_sprite().getGlobalBounds().height;
    float turtleY = turtle->get_sprite().getPosition().y;
    for (int index = 0; index < enemyTwos.size(); ++index)
    {
        if (isFreezed)
        {
            if (turtle->get_sprite().getGlobalBounds().intersects(enemyTwos[index]->get_sprite().getGlobalBounds()))
            {
                turtle->change_lives(-1);
                cout << "Watchout! An Enemy just hit you!" << endl;
                cout << "Lives Left: " << turtle->get_lives() << "!" << endl;
                if (turtle->get_lives() == 0)
                    roundLost = true;
                else
                    turtle->set_sprite(portal->get_cor().x, portal->get_cor().y);
                return;
            }
        }
        else
        {
            if (turtle->get_sprite().getGlobalBounds().intersects(enemyTwos[index]->get_sprite().getGlobalBounds()))
            {
                if (enemyTwos[index]->get_Y() >= turtleY + turtleHeight - turtle->get_ver_speed())
                {
                    delete enemyTwos[index];
                    enemyTwos.erase(enemyTwos.begin() + index);
                    return;
                }
                else
                {
                    turtle->change_lives(-1);
                    cout << "Watchout! An Enemy just hit you!" << endl;
                    cout << "Lives Left: " << turtle->get_lives() << "!" << endl;
                    if (turtle->get_lives() == 0)
                        roundLost = true;
                    else
                        turtle->set_sprite(portal->get_cor().x, portal->get_cor().y);
                    return;
                }
            }
        }
    }
}

#endif
