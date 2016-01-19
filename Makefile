all:game

game:game.cpp glad.c
	g++ -o game game.cpp glad.c -lGL -lGLU -ldl -I/usr/local/include -L/usr/local/lib -lglfw

clean:
	rm game
