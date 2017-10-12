#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>

enum GameState
{
    MENU,
    PLAY,
    GAMEOVER,
    QUIT,
};

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

Direction getdir()
{
    Direction dir;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        dir = LEFT;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        dir = RIGHT;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        dir = UP;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        dir = DOWN;
    else
        dir = NONE;

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
    win.setFramerateLimit(240);

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

    int delay = 30;

    bool menuFlag = true;

    sf::Font inconsolata;
    inconsolata.loadFromFile("Inconsolata.ttf");

    int titleProportion = 10;
    int titleFontSize = xres / titleProportion;
    sf::Text title;
    title.setFont(inconsolata);
    title.setString("SNAKE");
    title.setCharacterSize(titleFontSize);
    title.setFillColor(sf::Color::White);
    title.setOrigin((titleFontSize*2.5)/2, titleFontSize/2);
    title.setPosition(xres/2.0, yres/5.0);

    int menuProportion = 15;
    int menuFontSize = xres / menuProportion;
    sf::Text playButton;
    playButton.setFont(inconsolata);
    playButton.setString("Play");
    playButton.setCharacterSize(menuFontSize);
    playButton.setFillColor(sf::Color::White);
    playButton.setOrigin((menuFontSize*2)/2, round(menuFontSize/2.0));
    playButton.setPosition(xres/2, yres/2);
    int menuLeftBound;
    int menuRightBound;
    int playUpperBound;
    int playLowerBound;

    sf::Text quitButton;
    quitButton.setFont(inconsolata);
    quitButton.setString("Quit");
    quitButton.setCharacterSize(menuFontSize);
    quitButton.setFillColor(sf::Color::White);
    quitButton.setOrigin((menuFontSize*2)/2, round(menuFontSize/2.0));
    quitButton.setPosition(xres/2, yres/1.7);
    int quitUpperBound;
    int quitLowerBound;

    GameState gameState = MENU;

    while(win.isOpen())
    {
        sf::Event event;

        while(gameState == MENU)
        {
            while(win.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    win.close();
                    gameState = QUIT;
                }
            }

            win.clear(sf::Color(81, 81, 81));
            win.draw(title);
            win.draw(playButton);
            win.draw(quitButton);

            sf::Vector2i mousePos = sf::Mouse::getPosition(win);
            menuLeftBound = playButton.getPosition().x - (playButton.getLocalBounds().width / 2);
            menuRightBound = playButton.getPosition().x + (playButton.getLocalBounds().width / 2);

            playUpperBound = playButton.getPosition().y - (playButton.getLocalBounds().height/2.5);
            playLowerBound = playButton.getPosition().y + (playButton.getLocalBounds().height/1.5);
            if(mousePos.x > menuLeftBound && mousePos.x < menuRightBound && mousePos.y > playUpperBound && mousePos.y < playLowerBound)
            {
                playButton.setColor(sf::Color::Red);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    gameState = PLAY;
            }
            else
                playButton.setColor(sf::Color::White);

            quitUpperBound = quitButton.getPosition().y - (quitButton.getLocalBounds().height/2.5);
            quitLowerBound = quitButton.getPosition().y + (quitButton.getLocalBounds().height/1.5);
            if(mousePos.x > menuLeftBound && mousePos.x < menuRightBound && mousePos.y > quitUpperBound && mousePos.y < quitLowerBound)
                quitButton.setColor(sf::Color::Red);
            else
                quitButton.setColor(sf::Color::White);


            win.display();
        }

        while(gameState == PLAY)
        {
            while(win.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    win.close();
                    gameState = QUIT;
                }
            }
            --delay;

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
            static Food food;
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
            static Direction dir = RIGHT;
            static Direction prevDir = RIGHT;
            dir = getdir();

            if(dir == NONE)
                dir = prevDir;

            if(delay == 0)
            {
                if(prevDir == UP && dir == DOWN)
                    dir = prevDir;
                else if(prevDir == DOWN && dir == UP)
                    dir = prevDir;
                else if(prevDir == LEFT && dir == RIGHT)
                    dir = prevDir;
                else if(prevDir == RIGHT && dir == LEFT)
                    dir = prevDir;

                for(int i = snakeLength; i > 0; --i)
                    snake[i] = snake[i-1];

                prevDir = dir;

                switch(dir)
                {
                    case UP:    snake[0].ypos -= 1; break;
                    case DOWN:  snake[0].ypos += 1; break;
                    case LEFT:  snake[0].xpos -= 1; break;
                    case RIGHT: snake[0].xpos += 1; break;
                    default:    snake[0].xpos += 1; break;
                }

                delay = 30;
            }

            if(snake[0].xpos >= gridX || snake[0].xpos < 0 || snake[0].ypos >= gridY || snake[0].ypos < 0)
            {
                dir = RIGHT;
                prevDir = RIGHT;
                gameState = GAMEOVER;
            }

            for(int i = 1; i < snakeLength; ++i)
            {
                if(snake[0].xpos == snake[i].xpos && snake[0].ypos == snake[i].ypos)
                {
                    dir = RIGHT;
                    prevDir = RIGHT;
                    gameState = GAMEOVER;
                }
            }

            if(snake[0].xpos == food.xpos && snake[0].ypos == food.ypos)
            {
                ++snakeLength;
                snake[snakeLength-1] = snake[snakeLength-2];
                food.active = false;
            }

            win.display();
        }

        while(gameState == GAMEOVER)
        {
            snake[0] = {10, 10};
            snake[1] = {9, 10};
            snake[2] = {8, 10};

            snakeLength = 3;
            gameState = MENU;
        }
    }

    return 0;
}
