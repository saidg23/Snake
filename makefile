snake: snake.cpp
	g++ -std=c++17 snake.cpp -o snake -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: clean

clean:
	ls | grep -Ev '(snake.cpp|makefile|Inconsolata.ttf)' | xargs rm
