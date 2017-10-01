#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

struct Segment
{
    int xpos;
    int ypos;
};

struct Food
{
    int xpos;
    int ypos;
    bool active = false;
};

int getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

Direction getdir(bool &inputLogged, Direction dir, Direction prevDir)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        inputLogged = true;
        dir = UP;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        inputLogged = true;
        dir = DOWN;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        inputLogged = true;
        dir = LEFT;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        inputLogged = true;
        dir = RIGHT;
    }

    if(dir == UP && prevDir == DOWN)
        return prevDir;
    else if(dir == DOWN && prevDir == UP)
        return prevDir;
    else if(dir == LEFT && prevDir == RIGHT)
        return prevDir;
    else if(dir == RIGHT && prevDir == LEFT)
        return prevDir;
    else
        return dir;
}

int main()
{
    srand(time(0));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    int xres = 800;
    int yres = 600;

    sf::RenderWindow win(sf::VideoMode(xres, yres), "SNAKE? SNAAAAAAAAAAAAAKE!!!", sf::Style::Default, settings);
    win.setFramerateLimit(60);

    int gridX = 30;
    int gridY = 30;

    int tileSize = yres/gridY;
    int centerOffset = (xres - (tileSize * gridX))/2;
    sf::RectangleShape grid[gridX][gridY];

    for(int iX = 0; iX < gridX; ++iX)
    {
        for(int iY = 0; iY < gridY; ++iY)
        {
            grid[iX][iY].setSize(sf::Vector2f(tileSize, tileSize));
            grid[iX][iY].setFillColor(sf::Color(81, 81, 81));
            grid[iX][iY].setPosition(iX*tileSize + centerOffset, iY*tileSize);
            grid[iX][iY].setOutlineThickness(2);
            grid[iX][iY].setOutlineColor(sf::Color(58, 232, 75));
        }
    }

    Segment snake[gridX * gridY]{{10, 10}, {9, 10}, {8, 10}};
    int snakeLength = 3;

    bool inputLogged = false;
    Direction prevDir;
    Direction dir = RIGHT;

    Food food;

    int delay = 10;
    while(win.isOpen())
    {
        --delay;
        sf::Event event;
        while(win.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                win.close();
        }

        //clears screen and grid
        win.clear(sf::Color(81, 81, 81));
        for(int iX = 0; iX < gridX; ++iX)
        {
            for(int iY = 0; iY < gridY; ++iY)
            {
                grid[iX][iY].setFillColor(sf::Color(81, 81, 81));
            }
        }

        //places food in random place
        if(!food.active)
        {
            food.xpos = getRandomNumber(0, gridX-1);
            food.ypos = getRandomNumber(0, gridY-1);
            food.active = true;
        }
        grid[food.xpos][food.ypos].setFillColor(sf::Color::Blue);

        //draws snake on the grid
        for(int i = 0; i < snakeLength; ++i)
        {
            grid[snake[i].xpos][snake[i].ypos].setFillColor(sf::Color::White);
        }

        //draws the grid
        for(int iX = 0; iX < gridX; ++iX)
        {
            for(int iY = 0; iY < gridY; ++iY)
            {
                win.draw(grid[iX][iY]);
            }
        }

        //gets user input and checks valid direction
        if(!inputLogged)
        {
            prevDir = dir;
            dir = getdir(inputLogged, dir, prevDir);
        }

        if(delay == 0)
        {
            //
            for(int i = snakeLength-1; i > 0; --i)
            {
                snake[i] = snake[i-1];
            }

            switch(dir)
            {
                case UP:    snake[0].ypos -= 1; break;
                case DOWN:  snake[0].ypos += 1; break;
                case LEFT:  snake[0].xpos -= 1; break;
                case RIGHT: snake[0].xpos += 1; break;
                default:    snake[0].xpos += 1; break;
            }

            //Out of bounds detection
            if(snake[0].xpos >= gridX || snake[0].xpos < 0 || snake[0].ypos >= gridY || snake[0].ypos < 0)
                return 0;

            delay = 10;
            inputLogged = false;
        }

        if(snake[0].xpos == food.xpos && snake[0].ypos == food.ypos)
        {
            ++snakeLength;
            snake[snakeLength-1] = snake[snakeLength-2];
            food.active = false;
        }

        win.display();
    }

    return 0;
}
