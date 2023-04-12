
INCLUDE = C:\msys64\mingw64\include\SDL2
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows -lm

rocket: main.o rocket.o stars.o asteroid.o text.o
	gcc -o rocket main.o rocket.o stars.o asteroid.o text.o $(LDFLAGS)

main.o: ./src/main.c
	gcc -c -I$(INCLUDE) ./src/main.c

rocket.o: ./src/rocket.c ./include/rocket.h
	gcc -c -I$(INCLUDE) ./src/rocket.c

stars.o: ./src/stars.c ./include/stars.h
	gcc -c -I$(INCLUDE) ./src/stars.c

asteroid.o: ./src/asteroid.c ./include/asteroid.h
	gcc -c -I$(INCLUDE) ./src/asteroid.c

text.o: ./src/text.c ./include/text.h
	gcc -c -I$(INCLUDE) ./src/text.c
