#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>

enum GameState
{
    MENU,
    PLAY,
    WIN,
    GAMEOVER,
    QUIT
};

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE,
    HOLD
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
    int delayVal = 50;
    sf::RenderWindow win(sf::VideoMode(xres, yres), "SNAKE? SNAAAAAAAAAAAAAKE!!!", sf::Style::Default, settings);
    win.setFramerateLimit(240);

    int gridX = 20;
    int gridY = 20;

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
    Segment defaultPos[gridX * gridY]{{10, 10}, {9, 10}, {8, 10}};
    Segment snake[gridX * gridY]{{10, 10}, {9, 10}, {8, 10}};

    int snakeLength = 3;

    int delay = delayVal;

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
    playButton.setOrigin(menuFontSize, round(menuFontSize/2.0));
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
                playButton.setFillColor(sf::Color::Red);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    gameState = PLAY;
            }
            else
                playButton.setFillColor(sf::Color::White);

            quitUpperBound = quitButton.getPosition().y - (quitButton.getLocalBounds().height/2.5);
            quitLowerBound = quitButton.getPosition().y + (quitButton.getLocalBounds().height/1.5);
            if(mousePos.x > menuLeftBound && mousePos.x < menuRightBound && mousePos.y > quitUpperBound && mousePos.y < quitLowerBound)
            {
                quitButton.setFillColor(sf::Color::Red);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    return 0;
                }
            }
            else
                quitButton.setFillColor(sf::Color::White);


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
            while(!food.active)
            {
                food.active = true;
                food.xpos = getRandomNumber(0, gridX-1);
                food.ypos = getRandomNumber(0, gridY-1);

                for(int i = 0; i < snakeLength; ++i)
                {
                    if(snake[i].xpos == food.xpos && snake[i].ypos == food.ypos)
                    {
                        food.active = false;
                        break;
                    }
                }
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
            static Direction dir = HOLD;
            static Direction prevDir = HOLD;

            if(getdir() != NONE)
            {
                dir = getdir();
            }

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

                if(dir != HOLD)
                {
                    for(int i = snakeLength; i > 0; --i)
                        snake[i] = snake[i-1];
                }

                prevDir = dir;

                switch(dir)
                {
                    case UP:    snake[0].ypos -= 1; break;
                    case DOWN:  snake[0].ypos += 1; break;
                    case LEFT:  snake[0].xpos -= 1; break;
                    case RIGHT: snake[0].xpos += 1; break;
                    default:    break;
                }

                delay = delayVal;
            }

            if(snake[0].xpos >= gridX || snake[0].xpos < 0 || snake[0].ypos >= gridY || snake[0].ypos < 0)
            {
                dir = HOLD;
                prevDir = HOLD;
                gameState = GAMEOVER;
            }

            for(int i = 1; i < snakeLength; ++i)
            {
                if(snake[0].xpos == snake[i].xpos && snake[0].ypos == snake[i].ypos)
                {
                    dir = HOLD;
                    prevDir = HOLD;
                    gameState = GAMEOVER;
                }
            }

            if(snake[0].xpos == food.xpos && snake[0].ypos == food.ypos)
            {
                ++snakeLength;
                snake[snakeLength-1] = snake[snakeLength-2];
                food.active = false;
            }

            if(snakeLength == gridX * gridY)
                gameState = WIN;

            win.display();
        }

        while(gameState == WIN)
        {
            while(win.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    win.close();
                    gameState = QUIT;
                }
            }

            win.clear();

            sf::Text youWin;
            youWin.setString("YOU WIN!");
            youWin.setFont(inconsolata);
            youWin.setCharacterSize(xres/ 10);
            youWin.setOrigin((xres/ 10)*2, (yres/10)/2);
            youWin.setPosition(xres/2, yres/2);
            youWin.setFillColor(sf::Color::White);
            win.draw(youWin);
            win.display();

            sf::Vector2i mousePos = sf::Mouse::getPosition(win);
            if(mousePos.x >= 0 && mousePos.x < xres && mousePos.y >= 0 && mousePos.y < yres)
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    gameState = MENU;
        }

        while(gameState == GAMEOVER)
        {
            while(win.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    win.close();
                    gameState = QUIT;
                }
            }

            win.clear();

            sf::Text youLose;
            youLose.setString("GAME OVER!");
            youLose.setFont(inconsolata);
            youLose.setCharacterSize(xres/ 10);
            youLose.setOrigin((xres/ 10)*2, (yres/10)/2);
            youLose.setPosition(xres/2, yres/2);
            youLose.setFillColor(sf::Color::White);
            win.draw(youLose);
            win.display();

            sf::Vector2i mousePos = sf::Mouse::getPosition(win);
            if(mousePos.x >= 0 && mousePos.x < xres && mousePos.y >= 0 && mousePos.y < yres)
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    gameState = MENU;
        }

        for(int i = 0; i < 3; ++i)
        {
            snake[i] = defaultPos[i];
        }
        snakeLength = 3;
    }

    return 0;
}
