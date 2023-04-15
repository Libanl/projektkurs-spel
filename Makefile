
INCLUDE = C:\msys64\mingw64\include
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows -lm

spelare: main.o zombie.o spelare.o
	gcc -o spelare main.o zombie.o spelare.o $(LDFLAGS)

main.o: ./src/main.c
	gcc -c ./src/main.c

zombie.o: ./src/zombie.c ./includes/zombie.h
	gcc -c ./src/zombie.c

spelare.o: ./src/spelare.c ./includes/spelare.h
	gcc -c ./src/spelare.c

clean:
	rm *.o
	rm *.exe

